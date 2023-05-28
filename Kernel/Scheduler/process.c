#include "include/process.h"


//array con todos los procesos (organizado por pid)
process_ptr process_array[MAX_PROCESS_AMOUNT];
int current_pid = 0;
int foreground_process_pid = 0;
int process_count = 0;
int alive_process_count = 0;

process_ptr initialize_shell(void) {
    int fd[2] = { 0, 1 };
    return create_process("Shell", 0, NULL, NULL, FOREGROUND, fd);
}

process_ptr initialize_idle(void (*idle_fn)(int, char **)) {
    int fd[2] = { 0, 1 };
    process_ptr idle = create_process("Idle", 0, NULL, idle_fn, FOREGROUND, fd);
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
 * @param name process name
 * @param argc number of arguments the process must receive
 * @param argv array of arguments the process receives
 * @param fn process' function that receives argc and argv
 * @param visibility foreground or background
 * @param fd the 2 file descriptors the process has
 * @return process_ptr: returns the created process, or NULL if unsuccessful
 */
process_ptr create_process(char* name, int argc, char** argv, void (*fn)(int, char **), int visibility, int fd[2]) {
    //if current process (father) is in bg, cant create fg process ()

    process_ptr current_proc = get_process(current_pid);

    int pid = get_free_pid();
    if(pid == ERROR) {
        return NULL;
    }

    if( current_proc != NULL && visibility == FOREGROUND ) {
        // if (current_proc->visibility == FOREGROUND) {            // mientras no tengamos todo en userland (asi hay mas de 1 proc en fg)
            current_proc->visibility = BACKGROUND;
            foreground_process_pid = pid;
        // } else {
            // return NULL;
        // }
    }

    process_ptr new_process = sys_malloc(sizeof(process) + STACK_SIZE);  //definir STACK SIZE !!
    if(new_process == NULL) {
        sys_free(new_process);
        return NULL;
    }
    //aloco espacio para los argumentos
    char** args = sys_malloc(sizeof(char *) * argc);
    if(args == NULL) {
        sys_free(new_process);
        return NULL;
    }

    copy_args(args, argv, argc);

    if(current_proc != NULL) {
        current_proc->children[current_proc->children_count++] = pid;
    }
    new_process->children_count = 0;

    new_process->name = sys_malloc(strLength(name));
    if(new_process->name == NULL) {
        free_args(args, argc);
        sys_free(new_process);
        return NULL;
    }

    strCpy(new_process->name, name);

    new_process->visibility = visibility;
    new_process->priority = 1;
    new_process->ppid = current_pid;  
    new_process->fd_r = fd[0];
    new_process->fd_w = fd[1];
    new_process->rbp = (uint64_t)new_process + sizeof(process) + STACK_SIZE - sizeof(uint64_t);
    new_process->rsp = (uint64_t)(new_process->rbp - sizeof(registerBackup));
    new_process->pid = pid;
    new_process->status = ALIVE;

    initialize_stack(new_process->rsp, args, argc, fn);
    process_array[pid] = new_process;
    process_count++;
    return new_process;
}

//ps: imprime todos los procesos con sus propiedades (nombre, id, prioridad, SP, BP, foreground, etc)
void ps(void) {
    int printed_count = 0; //trackea cuantos procesos se accedieron realmente

    for(int i = 0; i < MAX_PROCESS_AMOUNT && printed_count <= process_count; i++) {
        process_ptr current_process;
        if((current_process = process_array[i]) != NULL) {
            //con funciones de textDriver.c:
            printString("|| Process Name: ", strLength("|| Process Name: "));
            printString(current_process->name, strLength(current_process->name)); 
            printChar('\n');
            printString("|| PID: ", strLength("|| PID: "));
            printNumber(current_process->pid, 10); //base 10
            printChar('\n');
            printString("|| PPID: ", strLength("|| PPID: "));
            printNumber(current_process->ppid, 10); //base 10
            printChar('\n');
            printString("|| Priority: ", strLength("|| Priority: "));
            current_process->priority == -1? printString("-1", 2) : printNumber(current_process->priority, 10);
            printChar('\n');
            printString("|| Status: ", strLength("|| Status: "));
            printString(get_process_status(current_process->status), strLength(get_process_status(current_process->status)));
            printChar('\n');
            printString("|| Visibility: ", strLength("|| Visibility: "));
            current_process->visibility == FOREGROUND? printString("Foreground", strLength("Foreground")) : printString("Background", strLength("Background"));
            printChar('\n');
            printChar('\n');

            // printf("|| Process Name: %s\n", current_process->name);
            // printf("||\tPID: %d\n", current_process->pid);
            // printf("||\tPPID: %d\n", current_process->ppid);
            // printf("||\tPriority: %d\n", current_process->priority);
            // printf("||\tStatus: %d\n", current_process->status);
            // printf("||\tVisibility: %s\n", current_process->visibility == FOREGROUND? "Foreground" : "Background");
            // printf("||\t\tSP: %lx \n||\t\tBP: %lx \n\n", current_process->rsp, current_process->rbp);
            
            printed_count++;
        }
    }
}

//loop: imprime su pid con un saludo cada ms milisegundos 
void loop_process(int pid, int ms) {
    int max_prints = 40;
    int i = 0;
    while( i < max_prints) {
        printString("Hello there! General PID:", 99);
        printNumber(pid, 10);
        printChar('\n');
        // printNumber(pid);
        sleepms(ms);
        i++;
    }
}

//kill: sets process status to KILLED -> will be freed on next encounter with scheduler
int kill_process(int pid) {
    if (pid == 0 || pid == 1) return 0;

    if (!process_exists(pid)) return ERROR;

    process_array[pid]->status = KILLED;
    process_ptr current_proc = get_process(current_pid);
    if(current_proc != NULL && current_proc->pid == pid) {
        //forzar timer tick
        force_timer();
    }
    return 1;
}

//removes process from list and frees its memory.
int free_process(int pid) {

    if(!process_exists(pid))
        return -1;
    // if (pid <= 1) return 0;

    save_children(pid);

    process_ptr proc = process_array[pid];
    //give parent process foreground:
    if (proc->visibility == FOREGROUND) {
        process_ptr parent = get_process(proc->ppid);
        if (parent != NULL) {
            parent->visibility = FOREGROUND;
            foreground_process_pid = parent->pid;
        }
    }
    
    //set process as ZOMBIE
    proc->status = ZOMBIE;

    //wait for parents to waitpid ......

    //parent has done waitpid for this process, can now be freed
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
    if (pid == 0 || pid == 1) return 0;

    if(!process_exists(pid))
        return ERROR;

    process_array[pid]->status = BLOCKED;

    //if the process blocked was the one running, force a timer tick
    process_ptr current_proc = get_process(current_pid);
    if(current_proc != NULL && current_proc->pid == pid) {
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

// void initialize_stack(process_ptr process, char** argv, int argc, void (*fn)(int, char **)) {
//     registerBackup * stack = (registerBackup *) process->rsp; 

//     stack->r15  = 0x001;
//     stack->r14  = 0x002;
//     stack->r13  = 0x003;
//     stack->r12  = 0x004;
//     stack->r11  = 0x005;
//     stack->r10  = 0x006;
//     stack->r9   = 0x007;
//     stack->r8   = 0x008;

//     //info necesaria
//     stack->rsi  = (uint64_t) argv;

//     stack->rdi  = argc;
//     stack->rbp  = process->rbp; //para testeo
//     stack->rdx  = 0;
//     stack->rcx  = 0;
//     stack->rbx  = 0;
//     stack->rax  = 0;
//     stack->rip  = fn;
//     stack->cs   = 0x8;
//     stack->rflags = 0x202;
//     stack->rsp = process->rsp; //para testeo
//     stack->ss = 0x0;
// }

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
    for(int i = 0; i < amount; i++) {
        if(destination[i] == NULL)
            return; 
        destination[i] = sys_malloc(sizeof(char) * (strLength(source[i] + 1)));
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
    return;
}

void set_current_process(int new_pid) {
    current_pid = new_pid;
}

bool wants_to_run(process_ptr process) {        // probs hayan otras condiciones a tener en cuenta
    return (process->status == READY || process->status == ALIVE) && process->priority != -1;
}

void save_children(int pid) {
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

    for(int i = 0; i < proc->children_count; i++) {
        process_ptr child = process_array[proc->children[i]];
        child->ppid = proc->ppid;
        grandpa->children_count++;
        grandpa->children[grandpa->children_count-1] = child->pid;
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