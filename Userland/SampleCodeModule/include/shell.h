#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdint.h>
#include <user_syscalls.h>
#include <tron.h>
#include <time.h>
#include <funclib.h>
#include <tests.h>
#include <beeper.h>

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
#endif