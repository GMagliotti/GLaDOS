#include "include/process.h"
#include <stdio.h>

//array con todos los procesos (organizado por pid)
process_ptr process_array[MAX_PROCESS_AMOUNT];
int current_pid = 0;
int foreground_process_pid = 0;
int process_count = 0;
int alive_process_count = 0;

int print_mode = false;

static const uint32_t colors[5] = {
        0xFF0000,   // Red
        0x00FF00,   // Green
        0x0000FF,   // Blue
        0xFFFF00,   // Yellow
        0xFF00FF    // Magenta
    };

process_ptr initialize_idle(void (*idle_fn)(int, char **)) {
    char * argv[1] = { "Idle" };
    process_ptr idle = create_process(1, argv, idle_fn, NULL);
    idle->priority = -1;
    return idle;
}

//free de shell y idle
void uninitialize(void) {
    for(int i = 1; i < MAX_PROCESS_AMOUNT; i++) {
        free_process(i);
    }
    free_shell();
}


/**
 * @brief Create a process object
 * 
 * @param argc number of arguments the process must receive
 * @param argv array of arguments the process receives
 * @param fn process' function that receives argc and argv
 * @param fd process' array of file descriptors, read and write
 * @return process_ptr: returns the created process, or NULL if unsuccessful
 */
process_ptr create_process(int argc, char** argv, void (*fn)(int, char **), int fd[2]) {

    process_ptr current_proc = get_process(current_pid);

    int pid = get_free_pid();
    if(pid == ERROR) {
        return NULL;
    }

    process_ptr new_process = (process_ptr)sys_malloc(sizeof(process) + STACK_SIZE);  //definir STACK SIZE !!
    if(new_process == NULL) {
        sys_free(new_process);
        return NULL;
    }

    //aloco espacio para los argumentos
    char** args = (char **)sys_malloc(sizeof(char *) * argc);

    if(args == NULL) {
        sys_free(new_process);
        return NULL;
    }

    copy_args(args, argv, argc);

    if(current_proc != NULL) {
        current_proc->children[current_proc->children_count++] = pid;
    }
    new_process->children_count = 0;

    new_process->name = (char *)sys_malloc(strLength(argv[0]));
    if(new_process->name == NULL) {
        free_args(args, argc);
        sys_free(new_process);
        return NULL;
    }

    strCpy(new_process->name, argv[0]);


    if (current_proc == NULL) {
        new_process->visibility = FOREGROUND;
        foreground_process_pid = pid;
    } else {
        new_process->visibility = BACKGROUND;
    }

    new_process->priority = 1;
    new_process->ppid = current_pid;  

    new_process->rbp = (uint64_t)new_process + sizeof(process) + STACK_SIZE - sizeof(uint64_t);
    new_process->rsp = (uint64_t)(new_process->rbp - sizeof(registerBackup) - 0x100);
    new_process->pid = pid;
    new_process->status = ALIVE;
    new_process->adopted = false;

    if( fd != NULL) {
        new_process->fd_r = fd[0];
        new_process->fd_w = fd[1];
    } else {
        new_process->fd_r = 0;
        new_process->fd_w = 0;
    }

    initialize_stack(new_process->rsp, args, argc, fn, init);
    process_array[pid] = new_process;
    process_count++;

    //char pid_string[4] = { 0 };
    //int_to_string(new_process->pid, pid_string, 10);
    char * semnem = "the_sem";
    new_process->done_sem = create_sem(0, semnem/*strCat("sem_", pid_string)*/);

    return new_process;
}

void init(int argc, char** argv, void (*fn)(int, char **)) {

    process_ptr proc = get_process(current_pid);

    char arr[0];
    arr[0] = '&';
    int amp_found = stringEquals(argv[argc-1], arr);
    if(amp_found == 0) {
        
        proc->og_visibility = FOREGROUND;
    } else {
        
        proc->og_visibility = BACKGROUND;
    }

    if (process_array[proc->ppid]->visibility == FOREGROUND) {
        if (amp_found) {
            proc->visibility = BACKGROUND;
            process_array[proc->ppid]->visibility = FOREGROUND;
            foreground_process_pid = proc->ppid;
        } else {
            proc->visibility = FOREGROUND;
            process_array[proc->ppid]->visibility = BACKGROUND;
            foreground_process_pid = current_pid;
        }
    }

    if (amp_found) {
        argv[argc-1] = NULL;
        argc--;
    }

    fn(argc, argv);
    
    proc->status = FINISHED;
    proc->ret_value = FINISHED;

    force_timer();

    while(1) {
        printString("Force timer more like fuck off\n", 0xFFFFFFFFFFFFFFFF);
        sleep(1);
    }
    return ;
}

//ps: imprime todos los procesos con sus propiedades (nombre, id, prioridad, SP, BP, foreground, etc)
void ps(void) {
    int printed_count = 0; //trackea cuantos procesos se accedieron realmente

    for(int i = 0; i < MAX_PROCESS_AMOUNT && printed_count <= process_count; i++) {
        process_ptr current_process;
        if((current_process = process_array[i]) != NULL) {
            //con funciones de textDriver.c:
            printColorString("\n|| ", strLength("\n||"), colors[i % 5]);
            printColorString("Process Name: ", strLength("Process Name: "), colors[i%5]);
            printString(current_process->name, strLength(current_process->name)); 


            printColorString("\n|| ", strLength("\n||"), colors[i%5]);
            printColorString("PID: ", strLength("PID: "), colors[i%5]);
            printNumber(current_process->pid, 10); //base 10
            printColorString("\t\t\t PPID: ", strLength("\t\t\t PPID: "), colors[i%5]);
            printNumber(current_process->ppid, 10); //base 10
            printColorString("\t\t\t Priority: ", strLength("\t\t\t Priority: "), colors[i%5]);
            current_process->priority == -1? printString("-1", 2) : printNumber(current_process->priority, 10);


            printColorString("\n|| ", strLength("\n||"), colors[i%5]);
            printColorString("Status: ", strLength("Status: "), colors[i%5]);
            printString(get_process_status(current_process->status), strLength(get_process_status(current_process->status)));
            printColorString("\t\t\t Visibility: ", strLength("\t\t\t Visibility: "), colors[i%5]);
            current_process->visibility == FOREGROUND? printString("Foreground", strLength("Foreground")) : printString("Background", strLength("Background"));
            printChar('\n');
            printChar('\n');
            
            printed_count++;
        }
    }
}

//kill: sets process status to KILLED -> will be freed on next encounter with scheduler
int kill_process(int pid) {

    print_mode = false;

    if (pid == 0 || pid == 1) return 0;

    if (!process_exists(pid)) {
        return ERROR;
    }

    process_array[pid]->status = KILLED;
    process_array[pid]->ret_value = KILLED;

    if(current_pid == pid) {
        //forzar timer tick
        force_timer();
    }
    return 1;
}

int kill_current_process() {
    return kill_process(current_pid);
}

//removes process from list and frees its memory.
int free_process(int pid) {
    if(!process_exists(pid))
        return -1;
    // if (pid <= 1) return 0;

    process_ptr proc = get_process(pid);
    process_ptr grandpa = get_process(proc->ppid);

    int found = 0;
    for(int i = 0; !found && i < grandpa->children_count ; i++) {
        if(grandpa->children[i] == pid) {
            grandpa->children[i] = grandpa->children[grandpa->children_count-1];
            grandpa->children[grandpa->children_count-1] = (uint64_t) NULL;
            grandpa->children_count--;
            found = 1;
        }
    }

    process_count--;
    process_array[pid] = NULL;
    sys_free(proc->name);
    registerBackup* stack = (registerBackup *) proc->rsp;
    free_args((char **)stack->rsi, stack->rdi);

    sys_free(proc);

    return 1;
}

//nice: cambia la prioridad de un proceso dado su PID y una nueva prioridad
int nice_process(int pid, int priority) {
    if(pid == 0 || !process_exists(pid) || priority < 0 || priority > 5) //errores: proceso shell, no existe proceso, nivel de prioridad incorrecto
        return -1;

    if(process_array[pid]->priority == priority) //misma prioridad que ya tenia
        return 0;

    //cambiamos la prioridad, se cambiarán sus vidas la proxima vez que se le entregue un quantum
    // scheduler_dequeue_process(process_array[pid]);
    process_array[pid]->priority = priority;
    // scheduler_enqueue_process(process_array[pid]); 

    return 1;
}

//block: cambia el estado de un proceso entre bloqueado y listo dado su pid
int block_process(int pid) {
    if (pid == 0) return 0;

    if(!process_exists(pid))
        return ERROR;


    process_array[pid]->status = BLOCKED;
    // if (foreground_process_pid == pid && pid != 1) {
    //     process_array[pid]->visibility = BACKGROUND;
    //     foreground_process_pid = process_array[pid]->ppid;
    //     process_array[foreground_process_pid]->visibility = FOREGROUND;
    // }

    //if the process blocked was the one running, force a timer tick
    if(current_pid == pid) {
        //forzar un timer tick
        // printf("Timer was forced\n");
        force_timer();
    }
    return 0;
}

int unblock_process(int pid) {
    if(!process_exists(pid) || pid == 0)
        return ERROR;
    process_array[pid]->status = READY;
    return 0;
}

//get_process: retorna un proceso dado su PID
process_ptr get_process(int pid) {
    return process_array[pid];
}

int get_foreground_process() {
    return foreground_process_pid;
}

bool current_is_foreground() {
    return get_process(current_pid)->visibility == FOREGROUND;
}


int process_exists(int pid) {
    return !(pid < 0 || pid >= MAX_PROCESS_AMOUNT || process_array[pid] == NULL);
}

int get_free_pid(void) {
    for(int i = 0; i < MAX_PROCESS_AMOUNT; i++) {
        if(process_array[i] == NULL)
            return i;
    }
    return ERROR;
}

/**
 * @brief 
 * 
 *
 * 
 * @param destination arguments destination
 * @param source arguments source
 * @param amount amount of arguments to be copied
 */
void copy_args(char** destination, char** source, int amount) {

    if (destination == NULL || source == NULL || amount <= 0) return;

    for(int i = 0; i < amount; i++) {
        destination[i] = (char**)sys_malloc(sizeof(char) * (strLength(source[i]) + 1));
        if(destination[i] == NULL) return; 
        // strCpy(destination[i], source[i], strLength(source[i]));
        strCpy(destination[i], source[i]);  
    }
}

//iterates through arguments array and frees each one
void free_args(char** args, int argc) {
    for(int i = 0; i < argc; i++) {
        sys_free(args[i]);
    }
    sys_free(args);
}

void force_timer(void) {
    forceTimer();
}

void set_current_process(int new_pid) {
    current_pid = new_pid;
}

void print_current_process() {
    process_ptr proc = get_process(current_pid);

    printColorString("Name: ", 32, colors[proc->pid % 5]); printString(proc->name, 16);
    printColorString(" - PID: ", 32, colors[proc->pid % 5]); printNumber(proc->pid, 10);
    printColorString(" - Prio: ", 32, colors[proc->pid % 5]); proc->priority < 0 ? printString("-1", 4) : printNumber(proc->priority, 10);
    printColorString(" - Remaining lives: ", 32, colors[proc->pid % 5]); printNumber(proc->currentLives, 10);
    printString("\n", 2);
}

bool wants_to_run(process_ptr process) {        // probs hayan otras condiciones a tener en cuenta
    return (process->status != BLOCKED ) && process->priority != -1;
}

void save_children(int pid) {
    process_ptr proc = get_process(pid);
    process_ptr grandpa = get_process(proc->ppid);

    for(int i = 0; i < proc->children_count; i++) {
        process_ptr child = process_array[proc->children[i]];
        child->ppid = proc->ppid;
        grandpa->children_count++;
        grandpa->children[grandpa->children_count-1] = child->pid;
        child->adopted = true;
    }

    return;
}

void free_shell() {
    process_ptr shell = process_array[0];
    sys_free(shell->name);
    registerBackup* stack = (registerBackup *) shell->rsp;
    free_args((char **)stack->rsi, stack->rdi);
    sys_free(shell);
}

char * get_process_status(int status) {
    if (status == ALIVE) return "Alive";
    if (status == READY) return "Ready";
    if (status == BLOCKED) return "Blocked";
    if (status == ZOMBIE) return "Zombie";
    if (status == FINISHED) return "Finished";
    if (status == KILLED) return "Killed";

    return "WOOOT??";
}

void set_print_mode() {
    print_mode = true;
}

bool on_print_mode() {
    return print_mode;
}

int waitpid(int pid) {
    if(pid == 0 || pid == 1 || !process_exists(pid)) {
        printString("ERROR: pid invalido\n", 50);
        return ERROR;
    }

    process_ptr current_proc = get_process(current_pid);

    process_ptr proc = process_array[pid];

    sem_wait(proc->done_sem);
    destroy_sem(proc->done_sem);

    int aux = proc->ret_value;

    //parent has done waitpid for this process, it can now be freed
    //destroy_sem(proc->done_sem);
    free_process(pid);

    return aux;
}

//waitpid for orphan processes it has inherited (receives parent pid)
void free_adopted_zombies(int pid) {
    for (int i = 0; process_array[pid]->children[i] != NULL; i++) {
        process_ptr child = process_array[process_array[pid]->children[i]];
        if (child->status == ZOMBIE) {
            // waitpid(child->pid);
            free_process(child->pid);
        }
    }
}

//sets process status as zombie and posts the done_sem, will be freed once the parent executes waitpid
void set_zombie(int pid) {
    process_ptr proc = process_array[pid];

    proc->status = ZOMBIE;

    save_children(pid);

    //if killed process was in fg, give parent process fg:
    if (proc->visibility == FOREGROUND) {
        proc->visibility = BACKGROUND;
        process_ptr parent = get_process(proc->ppid);
        if (parent != NULL) {
            parent->visibility = FOREGROUND;
            foreground_process_pid = parent->pid;
        }
    }

    
    //post done_sem so parent can collect my return value and i can be freed (parent was currently blocked at waitpid)
    //sem_post(process_array[proc->ppid]->done_sem);'
    sem_post(proc->done_sem);
}
