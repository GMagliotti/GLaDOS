#ifndef _TEXTMODE_DRIVER_H
#define _TEXTMODE_DRIVER_H

#include "lib.h"
#include <stdint.h>

#define BLACK_LT 0x00
#define BLUE_LT 0x01
#define GREEN_LT 0x02
#define CYAN_LT 0x03
#define RED_LT 0x04
#define PURPLE_LT 0x05
#define BROWN_LT 0x06
#define GRAY_LT 0x07
#define DARK_GRAY_LT 0x08
#define LIGHT_BLUE_LT 0x09
#define LIGHT_GREEN_LT 0x0A
#define LIGHT_CYAN_LT 0x0B
#define LIGHT_RED_LT 0x0C
#define LIGHT_PURPLE_LT 0x0D
#define YELLOW_LT 0x0E
#define WHITE_LT 0x0F

#define BLACK_BG 0x00
#define BLUE_BG 0x10
#define GREEN_BG 0x20
#define CYAN_BG 0x30
#define RED_BG 0x40
#define PURPLE_BG 0x50
#define BROWN_BG 0x60
#define GRAY_BG 0x70
#define DARK_GRAY_BG 0x80
#define LIGHT_BLUE_BG 0x90
#define LIGHT_GREEN_BG 0xA0
#define LIGHT_CYAN_BG 0xB0
#define LIGHT_RED_BG 0xC0
#define LIGHT_PURPLE_BG 0xD0
#define YELLOW_BG 0xE0
#define WHITE_BG 0xF0

void hvd_print(char *string, char attribute);
void hvd_put_char(char character, char attribute);
void hvd_erase_char();
void hvd_newline();
void hvd_print_dec(uint64_t value, uint8_t attribute);
void hvd_print_hex(uint64_t value, uint8_t attribute);
void hvd_print_bin(uint64_t value, uint8_t attribute);
void hvd_print_base(uint64_t value, uint32_t base, uint8_t attribute);
void hvd_erase_line();
void text_clear();

#endif