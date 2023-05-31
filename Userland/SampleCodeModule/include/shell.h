#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>
#include "user_syscalls.h"
#include "tron.h"
#include "time.h"
#include "funclib.h"
#include "tests.h"
#include <beeper.h>
#include "../programs/include/process_info.h"

#define DEFAULT_TEXT_SIZE 2

void setTextBufferActive(int active);

void saveChar(char c);
void checkBuffer();

void shell();
void setBash();
void hvdClear();
void help();
void clearCommandBuffer();
void setSize(int argc, char ** argv);
void printRegisters();                 // it must do something...

extern void testInvalidExc();
extern void test0Div();
void returnToShell();

int get_params(char* str, char* params[], int max_params);

#endif