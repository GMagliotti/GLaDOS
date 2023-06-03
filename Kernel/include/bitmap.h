#ifndef _BITMAP_H
#define _BITMAP_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <textDriver.h>
#define BITS_PER_SLOT 8

typedef uint8_t bit_index;
typedef uint16_t slot_index;
typedef int64_t bit_amount;

bool bmp_initialize(uint8_t *start_address, bit_amount size_in_bits);
void bmp_set_on(bit_amount bits_to_set, bit_amount start_bit);
void bmp_set_off(bit_amount bits_to_set, bit_amount start_bit);
bit_amount bmp_find(bit_amount size_in_bits, slot_index *slot, bit_index *bit);
void print_bitmap();

#endif
