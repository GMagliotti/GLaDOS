#include "include/user_syscalls.h"
#include "include/philoclase.h"
#include "include/stdio.h"
#include "include/funclib.h"

#define THINKING    0
#define HUNGRY      1
#define EATING      2

#define MAX_PHILOS 20

int state[MAX_PHILOS];
int mutex_id;
int philo_sems[MAX_PHILOS];     //indexes of each philosophers sem
int philo_pids[MAX_PHILOS];     //pids of each philosopher
int waiting_list[MAX_PHILOS];   
int waiting_list_size = 0;

int num_philos = 0;

// Starting function
void philo2(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: PHYLO <number of philosophers>\n");
        return;
    }

    num_philos = string_to_int(argv[1]);
    printf("number of philos: %d\n", num_philos);
    if (num_philos > 20 || num_philos < 2) {
        printf("There must be at least 2 philosophers and at most 20 philosophers!\n");
        return;
    }
    

    mutex_id = call_to_create_sem(1, "philo_mutex");
    
    // Initialize STATE and waiting list
    for (int i = 0; i < num_philos; i++) {
        philo_sems[i] = call_to_create_sem(0, "philo_sem");
        state[i] = THINKING; 
        waiting_list[i] = -1;
    }

    // Print menu
    printf("\n\t\t\t\tWelcome to the philosopher's dilemma\n");
    printf("You have chosen %d philosophers to take turns eating/thinking!\n", num_philos);
    printf("Press 'Q' at any time to quit the simulation\n");
    call_to_setSize(1);

    // Create philosopher processes
    for (int i = 0; i < num_philos; i++) {
        char* num;
        num = int_to_string(i, num, 10);
        char* args[3] = {"philo", num, NULL};
        philo_pids[i] = call_to_create_process(2, args, &philosopher2, NULL);
    }
    printf("\n");
    print_state2();

    while(getChar() != 'Q');

    //kill processes and semaphores
    for(int i = 0; i < num_philos; i++) {
        // printf("killing philo %d\n", i);
        call_to_pkill_process(philo_pids[i]);
        call_to_destroy_sem(philo_sems[i]);
    }

    call_to_destroy_sem(mutex_id);

    call_to_setSize(2);

    call_to_pkill_process(call_to_getpid()); 
}

void philosopher2(int argc, char** argv) {
    int i = string_to_int(argv[1]); //their philosopher index

    while (1) {
        think2();
        take_forks2(i);      // Acquire two forks
        eat2();
        put_forks2(i);       // Put both forks down

        printf("\n\t\t\t\t\tPress Q to end simulation\n");
    }
}

void take_forks2(int i) {
    call_to_sem_wait(mutex_id);          // Enter critical region
    state[i] = HUNGRY;
    print_state2();
    waiting_list[waiting_list_size++] = i; // Add philosopher to waiting list
    test2(i);                             // Try to acquire 2 forks

    if (state[i] != EATING) {
        call_to_sem_post(mutex_id);       // Exit critical region
        call_to_sem_wait(philo_sems[i]);  // Block if forks weren't acquired
    } else {
        waiting_list_size--;              // Remove philosopher from waiting list
        call_to_sem_post(mutex_id);       // Exit critical region
    }
}

void put_forks2(int i) {
    call_to_sem_wait(mutex_id);          // Enter critical region
    state[i] = THINKING;
    print_state2();

    // Check waiting list and unblock philosophers in order
    for (int j = 0; j < waiting_list_size; j++) {
        int philosopher = waiting_list[j];
        if (state[philosopher] != EATING && state[left2(philosopher)] != EATING && state[right2(philosopher)] != EATING) {
            state[philosopher] = EATING;
            print_state2();
            call_to_sem_post(philo_sems[philosopher]);

            // Remove philosopher from waiting list
            for (int k = j; k < waiting_list_size - 1; k++) {
                waiting_list[k] = waiting_list[k + 1];
            }
            waiting_list_size--;
            j--;  // Adjust index to process the next philosopher at the current position
        }
    }

    test2(left2(i));                        // Check if left neighbor can now eat
    test2(right2(i));                       // Check if right neighbor can now eat

    call_to_sem_post(mutex_id);         // Exit critical region
}



void test2(int i) {
    if (state[i] == HUNGRY && state[left2(i)] != EATING && state[right2(i)] != EATING) {
        state[i] = EATING;
        print_state2();
        call_to_sem_post(philo_sems[i]);
    }
}

int left2(int i) {
    return ((i + num_philos - 1) % num_philos);
}

int right2(int i) {
    return ((i + 1) % num_philos);
}

void think2() {
    sleep(1);
}

void eat2() {
    sleep(1);
}

void print_state2() {
    printf("EATING: ");
    for(int i = 0; i < num_philos; i++) {
        if(state[i] == EATING)
            printf("%d ", i);
    }
    printf("\nTHINKING: ");
    for(int i = 0; i < num_philos; i++) {
        if(state[i] == THINKING)
            printf("%d ", i);
    }
    printf("\nHUNGRY: ");
    for(int i = 0; i < num_philos; i++) {
        if(state[i] == HUNGRY)
            printf("%d ", i);
    }
    printf("\n--------------------------------\n");
}
