#ifndef FUNCLIB_H
#define FUNCLIB_H

#include "user_syscalls.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int string_equals(char *buf, char *arr);
int str_length(char *str);
int begins_with(char *container, char *contains);

int check_num_with_base(char val, int base);
int to_num_with_base(char *buffer, int from, int base);
int string_to_int(char *str);

void sleeps(int seconds);
void sleep(int seconds);
void sleepms(int milliseconds);

bool is_space(char c);
int is_only_space(char *str);

void print_memory_at(int argc, char **argv);

char *str_cat(char *destination, const char *source);
void num_to_str(int num, char *string, int len);
uint32_t uint_to_base2(uint64_t value, char *buffer, uint32_t base);
int atoi2(char *str);
void reverse(char str[], int length);
char *int_to_string(int num, char *str, int base);

#endif