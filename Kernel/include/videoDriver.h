#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <interrupts.h>
#include <keyboardDriver.h>
#include <lib.h>
#include <naiveConsole.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <textDriver.h>
#include <time.h>

void debug_print_grid();
uint8_t scroll_if_full_screen(int pen_ptr_y);

void hvd_clear();
char get_char();
void clear_buffer();
uint64_t uint_to_base(uint64_t value, char *buffer, uint32_t base);

void set_bash();

void put_char(uint8_t key);
void print_string(char *string, uint64_t str_length);

void put_pixel(uint32_t x, uint32_t y, uint32_t color);
void fill_rect(uint16_t x, uint16_t y, uint32_t rgb_value, uint16_t w,
               uint16_t h);

uint32_t get_screen_width();
uint32_t get_screen_height();

#endif