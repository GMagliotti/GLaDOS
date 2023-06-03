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
char *strCat(char *destination, const char *source);
void numToStr(int num, char *string, int len);
uint32_t uintToBase2(uint64_t value, char *buffer, uint32_t base);
int atoi2(char *str);
int strLength(char* str);

#endif