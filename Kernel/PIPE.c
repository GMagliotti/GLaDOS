// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pipe.h>

typedef struct {
  int r_index;
  int w_index;
  int sem_read; // id of the reading sem for this pipe
  char sem_read_name[MAX_PIPE_NAME];
  char buffer[BUFFER_SIZE];
  char name[MAX_PIPE_NAME];
  int amount_processes; // amount of processes using the pipe
} pipe_t;

typedef struct {
  pipe_t pipe;
  int available;
} space;

int sem_pipe_manager; // id of sem to control access to the array of pipes

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

// Returns -1 in case of error
int pipe_open(char *name) {
  if (!sem_wait(sem_pipe_manager)) {
    print("Error sem_wait en pipe_open\n");
    return -1;
  }

  // Search for that pipe
  int id = find_pipe(name);
  if (id == 0) {
    // A pipe of that name does not exist
    id = create_pipe(name);
  }
  if (id == -1) {
    print("Error in pipe_open, id=-1\n");
    sem_post(sem_pipe_manager);
    return -1;
  }
  pipes[id - 1].pipe.amount_processes++;
  if (!sem_post(sem_pipe_manager)) {
    print("Error sem_post in pipe_close\n");
    return -1;
  }
  return id;
}

int pipe_close(int pipe_index) {
  if (!index_valid(pipe_index))
    return -1;

  if (sem_wait(sem_pipe_manager) == -1) {
    print("Error sem_wait in pipe_close\n");
    return -1;
  }

  int close_read = sem_close(pipes[pipe_index - 1].pipe.sem_read_name);

  if (close_read == -1) {
    print("pipe_close: Error in the sem closes of pipe\n");
    return -1;
  }
  pipes[pipe_index - 1].available = true;

  if (sem_post(sem_pipe_manager) == -1) {
    print("Error sem_post in pipe_close\n");
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
  return -1; // No more space in array to create another pipe
}

int create_pipe(char *name) {
  int len = str_length(name);
  if (len <= 0 || len >= MAX_PIPE_NAME - 1) {
    print("create_pipe: Nombre demasiado largo\n");
    return -1;
  }
  int pos;
  if ((pos = find_available_space()) != -1) {
    pipe_t *new_pipe = &pipes[pos].pipe;
    // Initializing the structure
    str_cpy(new_pipe->name, name);
    new_pipe->r_index = 0;
    new_pipe->w_index = 0;

    str_cpy(new_pipe->sem_read_name, name);
    new_pipe->sem_read_name[len] = 'R';
    new_pipe->sem_read_name[len + 1] = 0;
    int sem_read = create_sem(0, new_pipe->sem_read_name);

    if (sem_read == -1) {
      print("Create_pipe: Error en los sem del pipe");
      return -1;
    }
    new_pipe->sem_read = sem_read;
  }
  return pos + 1;
}

int index_valid(int pipe_index) {
  return !(pipe_index == 0 || (pipe_index > MAX_PIPES && pipes[pipe_index - 1].available == FALSE));
}