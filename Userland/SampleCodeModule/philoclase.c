// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/philoclase.h"
#include "include/funclib.h"
#include "include/stdio.h"
#include "include/user_syscalls.h"
#include <stdio.h>

#define R_W_SEMNAME "r_w_sem"

#define THINKING 0
#define HUNGRY 1
#define EATING 2

#define MAX_PHILOS 20

int state[MAX_PHILOS];
int mutex_id;
int philo_sems[MAX_PHILOS]; // indexes of each philosophers sem
int philo_pids[MAX_PHILOS]; // pids of each philosopher
int waiting_list[MAX_PHILOS];
int waiting_list_size = 0;

int num_philos = 0;
int r_w_sem_index;

// Starting function
void philo(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: PHYLO <number of initial philosophers>\n");
    return;
  }

  num_philos = string_to_int(argv[1]);
  printf("number of philos: %d\n", num_philos);
  if (num_philos > 20 || num_philos < 2) {
    printf(
        "There must be at least 2 philosophers and at most 20 philosophers!\n");
    return;
  }

  r_w_sem_index = call_to_sem_open(R_W_SEMNAME);

  mutex_id = call_to_create_sem(1, "philo_mutex");

  // Initialize STATE and waiting list
  for (int i = 0; i < num_philos; i++) {
    philo_sems[i] = call_to_create_sem(0, "philo_sem");
    state[i] = THINKING;
    waiting_list[i] = -1;
  }

  // Print menu
  printf("\n\t\t\t\t\t\t\tWelcome to the philosopher's dilemma\n");
  printf("\t\t\t\t\t\tYou have chosen %d initial philosophers\n",
         num_philos);
  printf("\nPress 'Q' at any time to quit the demo\n");
  printf("Press 'A' to add a philosopher, or 'R' to remove one\n");

  call_to_set_size(1);

  // Create philosopher processes
  char *num = NULL;
  for (int i = 0; i < num_philos; i++) {
    num = int_to_string(i, num, 10);
    char *args[3] = {"philo", num, "&"};
    philo_pids[i] = call_to_create_process(3, args, &philosopher, NULL);
  }

  printf("\n");
  print_state();

  char c;
  while ((c = get_char()) != 'Q') {
    if (c == 'A') {
      if (num_philos < MAX_PHILOS) {
        num = int_to_string(num_philos, num, 10);
        char *args[3] = {"philo", num, "&"};
        philo_pids[num_philos] =
            call_to_create_process(3, args, &philosopher, NULL);
        num_philos++;
        printf("\nAdded philosopher\n");
      }
    } else if (c == 'R') {
      if (remove_philosopher() == -1) { // RACE CONDITION !!!
        printf("\nFailed to remove philosopher: there must always be at least "
               "one philosopher\n");
      } else {
        printf("\nRemoved philosopher\n");
      }
    } else {
      print_state();
    }
  }
  printf("\n Terminating the demo ...\n");

  // kill processes and semaphores
  for (int i = 0; i < num_philos; i++) {
    // printf("killing philo %d\n", i);
    call_to_pkill_process(philo_pids[i]);
    call_to_destroy_sem(philo_sems[i]);
  }

  call_to_destroy_sem(mutex_id);

  call_to_set_size(2);

  call_to_pkill_process(call_to_getpid());
}

// remove last philosopher added
int remove_philosopher() {
  /*if(waiting_list_size > 0) {
    // Remove philosopher last philosopher from waiting list
    int last_philosopher = waiting_list[waiting_list_size - 1];
    call_to_pkill_process(philo_pids[last_philosopher]);
    call_to_destroy_sem(philo_sems[last_philosopher]);
    waiting_list_size--;
    num_philos--;
    return 1;
  } else*/
  if (num_philos > 1) { // remove last philosopher added
    call_to_pkill_process(philo_pids[num_philos - 1]);
    call_to_destroy_sem(philo_sems[num_philos - 1]);
    philo_pids[num_philos - 1] = -1; // Set the removed pid to -1
    philo_sems[num_philos - 1] = -1; // Set the removed semaphore to -1
    num_philos--;
    return 1;
  } else {
    return -1;
  }
}

void philosopher(int argc, char **argv) {
  int i = string_to_int(argv[1]); // their philosopher index

  while (1) {
    think();
    take_forks(i); // Acquire two forks
    eat();
    put_forks(i); // Put both forks down

    printf("\n\t\t\t\t\tPress Q to end simulation\n");
  }
}

void take_forks(int i) {
  call_to_sem_wait(mutex_id); // Enter critical region
  state[i] = HUNGRY;
  call_to_sem_post(r_w_sem_index);

  waiting_list[waiting_list_size++] = i; // Add philosopher to waiting list
  test(i);                              // Try to acquire 2 forks

  if (state[i] != EATING) {
    call_to_sem_post(mutex_id);      // Exit critical region
    call_to_sem_wait(philo_sems[i]); // Block if forks weren't acquired
  } else {
    waiting_list_size--;        // Remove philosopher from waiting list
    call_to_sem_post(mutex_id); // Exit critical region
  }
}

void put_forks(int i) {
  call_to_sem_wait(mutex_id); // Enter critical region
  state[i] = THINKING;
  call_to_sem_post(r_w_sem_index);

  // Check waiting list and unblock philosophers in order
  for (int j = 0; j < waiting_list_size; j++) {
    int philosopher = waiting_list[j];
    if (state[philosopher] != EATING && state[left(philosopher)] != EATING &&
        state[right(philosopher)] != EATING) {
      state[philosopher] = EATING;

      call_to_sem_post(philo_sems[philosopher]);

      // Remove philosopher from waiting list
      for (int k = j; k < waiting_list_size - 1; k++) {
        waiting_list[k] = waiting_list[k + 1];
      }
      waiting_list_size--;
      j--; // Adjust index to process the next philosopher at the current
           // position
    }
  }

  test(left(i));  // Check if left neighbor can now eat
  test(right(i)); // Check if right neighbor can now eat

  call_to_sem_post(mutex_id); // Exit critical region
}

void test(int i) {
  if (state[i] == HUNGRY && state[left(i)] != EATING &&
      state[right(i)] != EATING) {
    state[i] = EATING;
    call_to_sem_post(r_w_sem_index);

    call_to_sem_post(philo_sems[i]);
  }
}

int left(int i) { return ((i + num_philos - 1) % num_philos); }

int right(int i) { return ((i + 1) % num_philos); }

void think() { sleep(3); }

void eat() { sleep(3); }

void print_state() {
  printf("\nEATING: ");
  for (int i = 0; i < num_philos; i++) {
    if (state[i] == EATING)
      printf("%d ", i);
  }
  printf("\nTHINKING: ");
  for (int i = 0; i < num_philos; i++) {
    if (state[i] == THINKING)
      printf("%d ", i);
  }
  printf("\nHUNGRY: ");
  for (int i = 0; i < num_philos; i++) {
    if (state[i] == HUNGRY)
      printf("%d ", i);
  }
  printf("\n<-------------------------Press Q to quit at any time------>\n");
}
