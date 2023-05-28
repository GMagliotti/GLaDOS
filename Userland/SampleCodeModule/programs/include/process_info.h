#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include "../../include/user_syscalls.h"
#include "../include/stdio.h"

void create_process();

void getpid();
void ps();
void loop_process();
void kill_process(int pid);
void nice_process(int pid);
void block_process(int pid);

#endif