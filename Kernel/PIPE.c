#include <pipe.h>

typedef struct {
  int r_index;
  int w_index;
  int sem_read; // id del sem para leer en este pipe
  char sem_read_name[MAX_LEN];
  char buffer[BUFFER_SIZE];
  char name[MAX_LEN];
  int amount_processes; // cantidad de procesos que estan usando el pipe
} pipe_t;

typedef struct {
  pipe_t pipe;
  int available;
} space;

int sem_pipe_manager; // id del sem para controlar el acceso al vetor de pipes.

space pipes[MAX_PIPES];

int index_valid(int pipe_index);
int create_pipe(char *name);
int find_pipe(char *name);
int find_available_space();
int write_char(int pipe_index, char c);
void print_pipe(pipe_t pipe);
void print_sems_invovled(pipe_t pipe);
void print_processes(pipe_t pipe);

void print(char *to_print) { print_string(to_print, 50); }

int init_pipes() {
  if ((sem_pipe_manager = create_sem(1, "Pipe Manager")) == -1) {
    print("Error en init_pipe");
    return -1;
  }

  for (int i = 0; i < MAX_PIPES; i++) {
    pipes[i].available = true;
  }
  return 0;
}

// Retorna -1 en caso de error
int pipe_open(char *name) {
  if (!sem_wait(sem_pipe_manager)) {
    print("Error sem_wait en pipe_open\n");
    return -1;
  }
  // Busco si existe un pipe en nuestra estructura de datos

  int id = find_pipe(name);
  if (id == 0) {
    // Si no existe un pipe con ese nombre
    id = create_pipe(name);
  }
  if (id == -1) {
    print("Error en pipeOpen, id=-1\n");
    sem_post(sem_pipe_manager);
    return -1;
  }
  pipes[id - 1].pipe.amount_processes++;
  if (!sem_post(sem_pipe_manager)) {
    print("Error semPost en pipe_close\n");
    return -1;
  }
  return id;
}

int pipe_close(int pipe_index) {
  if (!index_valid(pipe_index))
    return -1;

  if (sem_wait(sem_pipe_manager) == -1) {
    print("Error sem_wait en pipe_close\n");
    return -1;
  }

  int close_read = sem_close(pipes[pipe_index - 1].pipe.sem_read_name);

  if (close_read == -1) {
    print("pipe_close: Error en los sem close del pipe\n");
    return -1;
  }
  pipes[pipe_index - 1].available = true;

  if (sem_post(sem_pipe_manager) == -1) {
    print("Error sem_post en pipe_close\n");
    return -1;
  }
  return 1;
}

int write_pipe(int pipe_index, char *string) {
  if (!index_valid(pipe_index))
    return -1;

  int write_count = 0;

  while (*string != 0) {
    write_count++;
    if ((write_char(pipe_index, *string++)) == -1)
      return -1;
  }
  return write_count;
}

int write_char(int pipe_index, char c) {
  if (!index_valid(pipe_index))
    return -1;

  pipe_t *pipe = &pipes[pipe_index - 1].pipe;

  pipe->buffer[pipe->w_index % BUFFER_SIZE] = c;
  pipe->w_index++;
  if (sem_post(pipe->sem_read) == -1) {
    print("Error sem_post en write_char\n");
    return -1;
  }
  return 1;
}

char read_pipe(int pipe_index) {
  if (!index_valid(pipe_index))
    return -1;

  pipe_t *pipe = &pipes[pipe_index - 1].pipe;
  if (sem_wait(pipe->sem_read) == -1) {
    print("Error sem_wait en read_pipe\n");
    return -1;
  }
  char c = pipe->buffer[pipe->r_index % BUFFER_SIZE];
  pipe->r_index++;

  return c;
}

int find_pipe(char *name) {

  for (int i = 0; i < MAX_PIPES; i++) {
    if (pipes[i].available == false &&
        string_equals(name, pipes[i].pipe.name)) {
      return i + 1;
    }
  }
  return 0;
}

int find_available_space() {
  for (int i = 0; i < MAX_PIPES; i++) {
    if (pipes[i].available == true) {
      pipes[i].available = false;
      return i;
    }
  }
  return -1; // No hay mas espacio en el vector para crear otro pipe
}

int create_pipe(char *name) {
  int len = str_length(name);
  if (len <= 0 || len >= MAX_PIPE_NAME - 1) {
    print("create_pipe: Nombre demasiado largo\n");
    return -1;
  }
  uint64_t pos;
  if ((pos = find_available_space()) != -1) {
    pipe_t *new_pipe = &pipes[pos].pipe;
    // Inicializamos la estructura
    str_cpy(new_pipe->name, name);
    new_pipe->r_index = 0;
    new_pipe->w_index = 0;

    str_cpy(new_pipe->sem_read_name, name);
    new_pipe->sem_read_name[len] = 'R';
    new_pipe->sem_read_name[len + 1] = 0;
    uint64_t sem_read = create_sem(0, new_pipe->sem_read_name);

    if (sem_read == -1) {
      print("Create_pipe: Error en los sem del pipe");
      return -1;
    }
    new_pipe->sem_read = sem_read;
  }
  return pos + 1;
}

int index_valid(int pipe_index) {
  return !((pipe_index == 0 || pipe_index > MAX_PIPES) &&
           pipes[pipe_index - 1].available == FALSE);
}