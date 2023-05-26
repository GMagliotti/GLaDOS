#include "include/scheduler.h"

rr_queue_ptr rr_scheduler;  

// newScheduler: crea un scheduler de tipo RoundRobinWithPriority
rr_queue_ptr create_scheduler(void (*fn)(int, char **)) {
    rr_scheduler = create_new_round_robin(initialize_idle(fn));
    // scheduler_enqueue_process(initialize_shell());
    
    return rr_scheduler;
}


//para que se pueda acceder al current_process estando en process
process_ptr current_process(void) {
    return get_current_process(rr_scheduler);
}


// función que se llama cada vez que ocurre el timertick, o termina un proceso.
void * scheduler(void * rsp) { 
    uint64_t * to_ret_rsp = (uint64_t *)rsp; 

    process_ptr current_process = get_current_process(rr_scheduler);

    set_current_process(current_process->pid);

    if (current_process->status == FINISHED || current_process->status == KILLED) {

        set_current_process(current_process->ppid);

        if (current_process->pid != 0 && current_process->priority != -1) {

            process_ptr aux = current_process;

            scheduler_dequeue_process(current_process); 

            free_process(aux->pid);

            *to_ret_rsp = context_switch(current_process);
        }
    } else if(current_process->status == BLOCKED) {
        
        current_process = next_process(rr_scheduler);

        *to_ret_rsp = context_switch(current_process);
    } else {
        process_ptr incoming_process = next_tick(rr_scheduler);
        
        if(incoming_process != current_process) { //context switch necessary
            *to_ret_rsp = context_switch(incoming_process);
        }

    }
    return to_ret_rsp;
}

// Agregado de proceso al scheduler
int scheduler_enqueue_process(process_ptr p) {
    enqueue_process(rr_scheduler, p);
    return 69;
}

int scheduler_create_process(char* name, int argc, char** argv, void (*fn)(int, char **), int visibility, int fd[2]) {
    scheduler_enqueue_process(create_process(name, argc, argv, fn, visibility, fd));
    return 69;
}


// Borrado de proceso al scheduler (lo deberiamos de poder llamar nosotros) ¿que pasa cuando un proceso termina?
int scheduler_dequeue_process(process_ptr p) {
    dequeue_process(rr_scheduler, p);
    return 69;
}


uint64_t context_switch(process_ptr process) {
    return process->rsp;
}


void save_rsp(process_ptr process, uint64_t * to_save) {
    process->rsp = *to_save;
}


char * getProcessStatus(int status) {
    if (status == ALIVE) return "Alive";
    if (status == READY) return "Ready";
    if (status == BLOCKED) return "Blocked";
    if (status == ZOMBIE) return "Zombie";
    if (status == FINISHED) return "Finished";

    return "WOOOT??";
}


void free_scheduler(void) {
    uninitialize();
    free_round_robin(rr_scheduler);
}


// #include <stdio.h>
// #include <string.h>

// int main(int argc, char * argv[]) {

//     uint64_t the_process = 0;

//     rr_scheduler = create_scheduler();

//     char * procNames[20] = { 
//         "Sneezy Boogle",
//         "Giggles McWiggle",
//         "Wobble Doodle",
//         "Snickerdoodle",
//         "Bumble Whimsy",
//         "Ziggy Zoodle",
//         "Flibbertigibbet",
//         "Quibble Quackington",
//         "Wobblefluff",
//         "Jellybean McSquish",
//         "Noodle Nutter",
//         "Guffaw Snickerbottom",
//         "Whimsical Wobblebottom",
//         "Chuckletwist",
//         "Doodlebug McSillypants",
//         "Gigglefritz",
//         "Fizzbang McGiggle",
//         "Mirthful McSprinkle",
//         "Teehee Wobblekins",
//         "Guffaw Snickerdoodle" };

//     int fd[2] = { 10, 10};

//     char input[100];

//     int count = 0;

//     while (1) {
//         process_ptr proc = (process_ptr)(rr_scheduler->list->data);

//         printf("TheProcess: %lx\n", the_process);

//         printf("\nRRLIST\n");
//         printRobin(rr_scheduler->list);
//         printf("-- Por ejecutar: %s - Process status: %s, RSP: %lx, Remaining lives: %i --\n", proc->name, getProcessStatus((int)proc->status), the_process, proc->currentLives);
//         printf("-- Tiene %i hijos: ", proc->children_count);

//         for (int i = 0; i< proc->children_count; i++) {
//             process_ptr child = get_process(proc->children[i]);
//             printf("%i - %s, ", child->pid, child->name);
//         }


//         printf("\nEnter an operation [ 'next', 'kill', 'block', 'finished', 'add', 'print', 'end' ]: ");
//         fgets(input, sizeof(input), stdin);
//         input[strcspn(input, "\n")] = '\0';

//         if (strcmp(input, "end") == 0 || strcmp(input, "e") == 0) {
//             break;  // Exit the loop if "end" is entered
//         } else if ( strcmp(input, "next") == 0 || strcmp(input, "n") == 0 || strcmp(input, "") == 0 ) {
            
//         } else if (strcmp(input, "kill") == 0 || strcmp(input, "k") == 0) {
//             if (proc->priority != -1 && proc->pid != 0) {
//                 kill_process(proc->pid);            
//             }
//         } else if (strcmp(input, "block") == 0 || strcmp(input, "b") == 0) {
//             if (proc->priority != -1 && proc->pid != 0) {
//                 proc->status = BLOCKED;
//             }
//         } else if (strcmp(input, "finished") == 0 || strcmp(input, "f") == 0) {
//             if (proc->priority != -1 && proc->pid != 0) {
//                 proc->status = FINISHED;
//             }
//         } else if (strcmp(input, "add") == 0 || strcmp(input, "a") == 0) {
//             scheduler_create_process(procNames[count++ % 20], 0, NULL, NULL, FOREGROUND, fd);
//         } else if (strcmp(input, "print") == 0 || strcmp(input, "p") == 0) {
//             ps();
//         }

//         uint64_t rsp = proc->rsp;

//         the_process = *(uint64_t *)(scheduler(&rsp));
        
//     }

//     free_scheduler();

//     return 0;
// }
