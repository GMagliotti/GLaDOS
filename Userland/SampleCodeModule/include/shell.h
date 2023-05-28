#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdint.h>
#include "user_syscalls.h"
#include "tron.h"
#include "time.h"
#include "funclib.h"
#include "tests.h"
#include <beeper.h>
#include "../programs/include/process_info.h"
#include <stdbool.h>

#define DEFAULT_TEXT_SIZE 2

void setTextBufferActive(int active);

void saveChar(uint8_t c);
void checkBuffer();

void shell();
void setBash();
void hvdClear();
void time();
void help();
void clearCommandBuffer();
void setSize(int newSize);
void printRegisters();                 // it must do something...

extern void testInvalidExc();
extern void test0Div();
void returnToShell();
int commandBufferIsOnlySpaces();

void get_params(char* str, char* params[], int max_params);
int find_command(char * command, char * validCommands[16][2], void(*commandFunctions[16])(void));
int string_to_int(char * str);
bool is_space(char c);
int is_only_space(char * str);

#endif