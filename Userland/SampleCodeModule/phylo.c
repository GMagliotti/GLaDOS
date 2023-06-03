#include "include/phylo.h"
#include "include/user_syscalls.h"

#define MAX_PHYL 15
#define MIN_PHYL 2
#define INIT_PHYL 5
#define SEM_PHYL "sem_phylos"
#define LEN 10
#define QUANTUM 3

typedef struct {
    uint64_t pid;
    int state;
    uint64_t sem_index;
    char sem_name[LEN];
} phylo_t;

enum state {
    THINKING,
    EATING,
    WAITING
};

static phylo_t phylos[MAX_PHYL];
static int seated;
uint64_t sem_index;



static const uint32_t colors[5] = {
        0xFF0000,   // Red
        0x00FF00,   // Green
        0x0000FF,   // Blue
        0xFFFF00,   // Yellow
        0xFF00FF    // Magenta
    };

// Runs philosophers dilemma
void phylo() {
    printf("WELCOME PHILOSOPHER!\n");
    
    if ((sem_index = call_to_create_sem(1, SEM_PHYL)) == -1) {
        printf("Error opening main semaphore in phylo.\n");
        return;
    }
    
    seated = 0;
    // call_to_sem_post(sem_index);
    for (int i = 0; i < INIT_PHYL; i++) {
        printf("adding philosopher: %d\n", i);

        if (add_phylo(i) == -1) {
            printf("Error adding initial philosophers.\n");
        }
        call_to_ps();
    }
    call_to_ps();
    printf("Press 'a' to add or 'r' to remove a philosopher. Press 'q' to exit.\n");
    char c;
    while ((c = getChar()) != 0) {
        switch (c) {
        case 'a':
        case 'A':
            if (add_phylo(seated) == -1) {
                printf("Error adding philosopher\n");
            }
            break;
        case 'r':
        case 'R':
            if (remove_phylo(seated - 1) == -1) {
                printf("Error removing philosopher\n");
            }
            break;
        case 'q':
        case 'Q':
            end_table();
            if (call_to_sem_close(SEM_PHYL) == -1) {          //LE PODRIA PASAR SEM_INDEX !!! probar dsps
                printf("Error closing main sem in phylo\n");
            }
            // _unblock(2);
            return;
        }
    }
}

/**
 * Se suma un filosofo a la mesa, prepara sus cosas para comer.
 * Retorna -1 en caso de error.
 */
static int add_phylo(int p_index) {
    if (p_index >= MAX_PHYL || p_index < 0) {
        return -1;
    }
    printf("philo: %d is about to enter region\n", p_index);
    call_to_sem_wait(sem_index);    //pido acceso para crear/modificar filosofos
    
    sleep(2);
    seated++;
    char sem_name[LEN] = "phyl";
    numToStr(p_index, phylos[p_index].sem_name, LEN);
    strCat(phylos[p_index].sem_name, sem_name);
    if ((phylos[p_index].sem_index = call_to_create_sem(1, phylos[p_index].sem_name)) == -1) {
        printf("Error opening sem in add_phylo.\n");
        return -1;
    }
    //hardcode porque no se inicia en 1:
    // call_to_sem_post(phylos[p_index].sem_index);


    char curr_seated[LEN];
    uintToBase2(seated, curr_seated, 10);
    char *argv[] = {"phi", curr_seated, NULL};
    phylos[p_index].state = THINKING; //argc, params, fn
    if ((phylos[p_index].pid = call_to_create_process(2, argv, &phylo_process, NULL)) == 0) { //TODO Cambiar al implementar pipes
        printf("Error creating philosopher process");
        return -1;
    }

    printf("philo: %d is about to leave region\n", p_index);
    call_to_sem_post(sem_index); //libero acceso a creacion de otros filosofos
    return 0;
}
void do_nothing() {
    return;
}
/**
 * Proceso que hace que los filosofos lleguen a la mesa,
 * busquen sus chopsticks y se sienten, si no pueden,
 * deben esperar a su turno.
 */
void phylo_process(int argc, char **argv) {
    int index = atoi2(argv[1]);
    while (1) {
        sleep(QUANTUM);
        // Busca chopsticks ya que tiene hambre.
        printf("philo #%d about to take chopstick\n", index);
        take_chopstick(index);
        sleep(QUANTUM);
        // Termino de comer, deja sus chopsticks.
        printf("philo #%d about to leave chopstick\n", index);
        put_chopstick(index);
    }
}

/**
 * El filosofo desea comer, toma unos palillos y espera su turno. 
 */
void take_chopstick(int p_index) {
    call_to_sem_wait(sem_index);
    phylos[p_index].state = WAITING;
    update(p_index);
    call_to_sem_post(sem_index);
    call_to_sem_wait(phylos[p_index].sem_index);
}

/**
 *  El filosofo piensa y se ve junto a sus vecinos, quienes se preguntan si pueden comer.
 */
void put_chopstick(int p_index) {
    call_to_sem_wait(sem_index);
    phylos[p_index].state = THINKING;
    update(left(p_index));
    update(right(p_index));
    call_to_sem_post(sem_index);
}

// Devuelve el vecino izquierdo del indice.
int left(int p_index) {
    return (p_index == 0) ? seated - 1 : p_index - 1;
}

// Devuelve el vecino derecha del indice.
int right(int p_index) {
    return (p_index == seated - 1) ? 0 : p_index + 1;
}

// Actualiza la mesa con los filosofos que desean y se encuentran comiendo.
void update(int p_index) {
    if (phylos[p_index].state == WAITING && phylos[left(p_index)].state != EATING && phylos[right(p_index)].state != EATING) {
        phylos[p_index].state = EATING;
        print_state();
        call_to_sem_post(phylos[p_index].sem_index);
    }
}

/**
 * El filosofo se retira de la mesa, no quiere comer más entonces
 * su lugar es liberado y se reordena el resto.
 */
static int remove_phylo(int p_index) {
    if (p_index >= MAX_PHYL || p_index <= MIN_PHYL) {
        printf("philo #%d DNE, returning -1", p_index);
        return -1;
    }
    call_to_sem_wait(sem_index);
    seated--;
    int eats = (phylos[p_index].state == EATING);
    if (call_to_sem_close(phylos[p_index].sem_name) == -1)
        printf("Error in closing phylo's sem.\n");
    if (call_to_pkill_process(phylos[p_index].pid) == -1)
        printf("Error in killing phylo process.\n");
    if (eats) {
        update(p_index - 1);
        update(0);
    }
    call_to_sem_post(sem_index);
    return 0;
}

/**
 * Se cierra la mesa, todo filosofo es echado.
 */
void end_table() {
    while (seated > 0) {
        if (call_to_sem_close(phylos[seated - 1].sem_name) == -1) {
            printf("Error in closing phylo's sem while ending.\n");
            return;
        } 
        if (call_to_pkill_process(phylos[seated - 1].pid) == -1) {
            printf("Error in killing phylo process while ending.\n");
            return;
        }
        seated--;
    }
}

static void print_state() {
    for (int i = 0; i < seated; i++) {
        (phylos[i].state == EATING) ? printf(" E ") : printf(" . ");
    }
    putc('\n');
}