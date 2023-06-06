#ifndef PIPE_H
#define PIPE_H

#define BUFFER_SIZE 1024
#define MAX_PIPES 10
#define MAX_PIPE_NAME 32

#define TRUE 1
#define FALSE 0

#include "../Semaphore/include/semaphore.h"
#include "defs.h"
#include "lib.h"
#include "textDriver.h"
#include <stdbool.h>

int init_pipes();
int pipe_open(char *name);
int pipe_close(int pipe_index);
int write_pipe(int pipe_index, char *string);
char read_pipe(int pipe_index);
void pipe();

#endif
