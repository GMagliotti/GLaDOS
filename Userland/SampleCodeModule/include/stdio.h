#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h>
#include <stdint.h>
#include <user_syscalls.h>

void printf(char *fmt, ...);
int scanf(const char *format, ...);
char get_char();
void putc(char character);
void clear_buffer();
char *str_cat(char *destination, const char *source);
void num_to_str(int num, char *string, int len);
uint32_t uint_to_base2(uint64_t value, char *buffer, uint32_t base);
int atoi2(char *str);
int str_length(char *str);

#endif