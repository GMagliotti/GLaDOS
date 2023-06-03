#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

uint8_t *get_current_video();
void nc_print(const char *string);
void ncput_char(char character);
void nc_newline();
void nc_print_dec(uint64_t value);
void nc_print_hex(uint64_t value);
void nc_print_bin(uint64_t value);
void nc_print_base(uint64_t value, uint32_t base);
void nc_clear();

#endif