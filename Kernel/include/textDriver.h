#ifndef _TEXT_DRIVER_H
#define _TEXT_DRIVER_H
#include <keyboardDriver.h>
#include <stdarg.h>
#include <stdint.h>
#include <videoDriver.h>

#define DEFAULT_FONT_HEIGHT 8
#define DEFAULT_FONT_WIDTH 8
#define DEFAULT_FONT_COLOR 0xFFFFFF
#define RED 0xFF0000

void set_font_type(int type);
void set_font_size(int size);
void set_bash();

extern uint8_t get_key();
void save_key(uint8_t c);

void set_ptrx(int num);
void set_ptry(int num);

void print_char(uint8_t key_code);
void print_string(char *string, uint64_t str_length);
void print_color_string(char *string, uint64_t str_length, uint32_t color);
void print_number(int value, int base);

int string_equals(char *buf, char *arr);
int str_length(char *str);
void str_cpy(char *dest, char *src);

int get_buffer_pos();
char buffer_at(int n);

char *str_cat(char *destination, const char *source);
char *int_to_string(int num, char *str, int base);

#endif