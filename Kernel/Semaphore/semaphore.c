// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/semaphore.h"
#include <textDriver.h>
typedef struct
{
    sem_t sem;
    uint64_t available;
} space;
static space sem_spaces[MAX_SEM];

// Is there a race condition here?
static int find_available_semaphore() {
    for (int i = 0; i < MAX_SEM; i++) {
        if (sem_spaces[i].available == TRUE) {
            sem_spaces[i].available = FALSE;
            return i;
        }
    }
    return -1;
}

//sets all semaphores status as available
void init_sem() {
    for (int i = 0; i < MAX_SEM; i++) {
        sem_spaces[i].available = TRUE;
    }
}

uint64_t create_sem(uint64_t initial_value, char * name) {
    int sem_index = find_available_semaphore();
    if(sem_index == -1 || strLength(name) >= MAX_NAME )
        return -1;
    sem_t sem = sem_spaces[sem_index].sem;
    if (initial_value == 0) sem.lock = 1;
    strCpy(sem.name, name);
    sem.value = initial_value;
    sem.size = 1; //process creating sem is the only one using it initially
    sem.size_list = 0;
    sem.first_process = NULL;
    sem.last_process = NULL;
    
    return sem_index;
}

//sets available condition of semaphore to true
void destroy_sem(int sem_index) {
    sem_spaces[sem_index].available = TRUE;
}

/**
* @brief Increases the semaphore value by one
* @param sem_idx The index of the semaphore
* @return 1 if sem_idx is not valid, 0 otherwise
*/
uint64_t sem_post(uint64_t sem_idx) {
    if (sem_idx <= 0 || sem_idx >= MAX_SEM) return 1;
    uint64_t * lock_addr = &(sem_spaces[sem_idx].sem.lock);
    if (sem_spaces[sem_idx].sem.value != 0) enter_region(lock_addr);
    sem_spaces[sem_idx].sem.value++;
    leave_region(lock_addr);
    return 0;
}

/**
* @brief Decreases the value of the semaphore by one, or blocks the process if the value is zero.
* @param sem_idx The index of the semaphore
* @return 1 if sem_idx is not valid, 0 otherwise
*/
uint64_t sem_wait(uint64_t sem_idx) {
    if (sem_idx <= 0 || sem_idx >= MAX_SEM) return 0xFFFFFFFFFFFFFFFF;
    uint64_t * lock_addr = &(sem_spaces[sem_idx].sem.lock);
    enter_region(lock_addr);
    sem_spaces[sem_idx].sem.value--;
    if (sem_spaces[sem_idx].sem.value != 0) leave_region(lock_addr);
    return 0;
}

//returns pid of next process in list
int sem_dequeue_process(int sem_index) {
    space sem_space = sem_spaces[sem_index];
    if(sem_space.available == TRUE) { //space doesnt exist
        return -1;
    }
    sem_t sem = sem_space.sem;
    sem_process_t * current_process = sem.first_process;
    if(current_process == NULL) {
        return -1;
    }

    //remove process from sem list
    int current_pid = current_process->pid;
    sem.first_process = current_process->next;
    if(sem.first_process == NULL) { //we just removed its last process
        sem.last_process == NULL;
    }
    sem.size_list--;

    return current_pid; 
}

//returns 0 if successful, -1 if not
int sem_enqueue_process(int sem_index, int pid) {
    space sem_space = sem_spaces[sem_index];
    if(sem_space.available == TRUE || pid == NULL) { //space doesnt exist
        return -1;
    }
    sem_t sem = sem_space.sem;

    sem_process_t * created_process = sys_malloc(sizeof(sem_process_t));
    if(created_process == NULL) {
        return -1;
    }

    created_process->pid = pid;
    if(sem.size_list == 0) { //adding first process in list
        sem.first_process = created_process;
        sem.last_process = created_process;
    }
    else {
        sem.last_process->next = created_process; //ex-last process now in front of created_process
        sem.last_process = created_process; //created process now the last one in the list
    }
    created_process->next = NULL;
    sem.size_list++;

    return 0;
}

int find_sem_index(char* name) {
    for(int i = 0; i < MAX_SEM; i++) {
        if(sem_spaces[i].available == FALSE && (stringEquals(sem_spaces[i].sem.name, name) == 0)) {
            return i;
        }
    }
    return -1;
}

//searches for sem with that name, returns its index (does not create sem if not found, returns -1)
int sem_open(char *name) {
    return find_sem_index(name);
}


int sem_close(char *name) {
    int sem_index = find_sem_index(name);
    if(sem_index == -1) {
        return -1;
    }
    sem_t sem = sem_spaces[sem_index].sem;
    while(sem.value > 0);
    destroy_sem(sem_index);
    return 0;
}

//TODO: hacer que el dequeue de un proceso chequee si contiene un sem -> si tiene uno y es el unico que lo accede -> destroy_sem