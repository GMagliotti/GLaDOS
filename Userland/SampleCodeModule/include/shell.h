#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>
#include "user_syscalls.h"
#include "tron.h"
#include "time.h"
#include "funclib.h"
#include "tests.h"
#include <beeper.h>
#include <phylo.h>
#include "../programs/include/process_info.h"
#include "../programs/include/ipc_processes.h"
#include "../tests/test_util.h"
#include <stdbool.h>

#define MAX_PARAMS 8

#define DEFAULT_TEXT_SIZE 2

#define FOREGROUND 0
#define BACKGROUND 1

void setTextBufferActive(int active);

void saveChar(char c);
void checkBuffer();

void shell();
void setBash();
void hvdClear();
void help();
void show_arqui_commands();
void clearCommandBuffer();
void setSize(int argc, char ** argv);
void printRegisters();                 // it must do something...

extern void testInvalidExc();
extern void test0Div();
void returnToShell();

int get_params(char* str, char* params[], int max_params);

#endif