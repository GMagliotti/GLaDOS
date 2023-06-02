#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include "../../include/user_syscalls.h"
#include "../include/stdio.h"
#include <funclib.h>

void getpid();
void ps();
void loop_process(int argc, char** argv);
void kill_process(int argc, char** argv);
void nice_process(int argc, char** argv);
void block_process(int argc, char** argv);
void unblock_process(int argc, char** argv);


#endif