#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h>
#include <stdint.h>
#include <user_syscalls.h>

void printf(char * fmt, ...);
int scanf(const char *format, ...);
char getChar();
void putc(char character);
void clearBuffer();

#endif