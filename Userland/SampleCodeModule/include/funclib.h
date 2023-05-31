#ifndef FUNCLIB_H
#define FUNCLIB_H

#include <stdint.h>
#include <user_syscalls.h>
#include <stdio.h>
#include <stdbool.h>

int stringEquals(char* buf, char* arr);
int strLength(char* str);
int beginsWith(char * container, char * contains);

int checkNumWithBase(char val, int base);
int toNumWithBase(char * buffer, int from, int base);
int string_to_int(char * str);

void sleeps(int seconds);
void sleep(int seconds);
void sleepms(int milliseconds);

bool is_space(char c);
int is_only_space(char * str);

void printMemoryAt(int argc, char** argv);

#endif