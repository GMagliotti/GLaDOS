#ifndef FUNCLIB_H
#define FUNCLIB_H

#include <stdint.h>
#include <user_syscalls.h>
#include <stdio.h>

int stringEquals(char* buf, char* arr);
int strLength(char* str);
int beginsWith(char * container, char * contains);
int checkNumWithBase(char val, int base);
int toNumWithBase(char * buffer, int from, int base);

void sleeps(int seconds);
void sleep(int seconds);
void sleepms(int milliseconds);

void printMemoryAt(int dir);

#endif