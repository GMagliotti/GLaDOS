// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <bitmap.h>

#define AVAILABLE_SPACE 0x10000

typedef struct bitmap {
  uint8_t *bitmap_array;
  uint16_t size;
} bitmap;

bitmap the_bitmap;

/**
 * @brief Initializes the bitmap with sizeInBits spaces
 *
 * @param start_address The address from which the bitmap will be stored
 * @param size_in_bits The amount of BITS/PAGES to initialize
 * @return false if sizeInBits is 0 or larger than what's available, true if
 * bitmap was correctly initialized
 */
bool bmp_initialize(uint8_t *start_address, bit_amount size_in_bits) {
  if (size_in_bits == 0 || size_in_bits > AVAILABLE_SPACE * BITS_PER_SLOT)
    return false;
  the_bitmap.bitmap_array = start_address;
  the_bitmap.size = (size_in_bits % BITS_PER_SLOT == 0)
                        ? size_in_bits / BITS_PER_SLOT
                        : (size_in_bits / BITS_PER_SLOT) + 1;
  memset(the_bitmap.bitmap_array, 0, the_bitmap.size);
  return true;
}

/**
 * @brief Sets bits_to_set bits to 1, starting from start_bit
 *
 * @param bits_to_set The amount of bits to set in the bitmap
 * @param start_bit The starting position in the bitmap
 */
void bmp_set_on(bit_amount bits_to_set, bit_amount start_bit) {
  int32_t current_subindex = start_bit % BITS_PER_SLOT,
          current_index = start_bit / BITS_PER_SLOT, total_bits_set = 0;
  for (; total_bits_set < bits_to_set; current_index++) {
    for (; total_bits_set < bits_to_set && current_subindex < BITS_PER_SLOT;
         current_subindex++) {
      the_bitmap.bitmap_array[current_index] =
          the_bitmap.bitmap_array[current_index] | (0x01U << current_subindex);
      total_bits_set++;
    }
    current_subindex = 0;
  }
}

/**
 * @brief Sets bits_to_set bits to 0, starting from start_bit
 *
 * @param bits_to_set The amount of bits to set in the bitmap
 * @param start_bit The starting position in the bitmap
 */
void bmp_set_off(bit_amount bits_to_set, bit_amount start_bit) {
  int32_t current_subindex = start_bit % BITS_PER_SLOT,
          current_index = start_bit / BITS_PER_SLOT, total_bits_set = 0;
  for (; total_bits_set < bits_to_set; current_index++) {
    for (; total_bits_set < bits_to_set && current_subindex < BITS_PER_SLOT;
         current_subindex++) {
      // Essentially:
      // 1. Take the least significant bit and shift it n bits left, assuming
      // n=2  0b0001 turns into 0b0100
      // 2. Flip all bits, in our example: 0b0100 turns into 0b1011
      // 3. In the array, make it so all bits except for the one that is in the
      // same position as the
      //    last 0, remain the same whilst that one changes to 0.
      //    If the number was 0b1110, then after this the number will be 0b1010
      the_bitmap.bitmap_array[current_index] =
          the_bitmap.bitmap_array[current_index] &
          (~(0x01U << current_subindex));
      total_bits_set++;
    }
    current_subindex = 0;
  }
}

/**
 * @brief Finds the first consecutive slots that fits the size_in_bits specified
 * in the bitmap
 *
 * @param size_in_bits The amount of consecutive free bits that are needed
 * @param index An address to a WORD, the index of the bitmap array will be
 * returned here
 * @param bit An address to a BYTE, the bit subindex of the bitmap array will be
 * returned here
 * @return bit_amount = QWORD - The starting bit of the position that fits the
 * object in the bitmap, -1 if no fitting space was found
 */
bit_amount bmp_find(bit_amount size_in_bits, slot_index *slot, bit_index *bit) {
  // TODO cambiar por un algoritmo mas rapido, este es muy naive
  bit_amount bits_traveled = 0, consecutive_free_bits = 0,
             consecutive_start_bit = 0;
  const bit_amount max_bits = ((bit_amount)the_bitmap.size) * BITS_PER_SLOT;
  for (slot_index i = 0; bits_traveled < max_bits; i++) {
    for (bit_index j = 0; bits_traveled < max_bits && j < BITS_PER_SLOT; j++) {
      if (((the_bitmap.bitmap_array[i] >> j) & 0x01U) == 0) {
        consecutive_free_bits++;
        if (consecutive_free_bits == 1) {
          *slot = i;
          *bit = j;
          consecutive_start_bit = bits_traveled;
        }
        if (consecutive_free_bits == size_in_bits) {
          return consecutive_start_bit;
        }
      } else {
        consecutive_free_bits = 0;
      }
      bits_traveled++;
    }
  }
  return -1;
}

void print_header(int slot) {
  print_string("Showing pages 0x", 0x100);
  print_number(slot * 8, 16);
  print_string(" - 0x", 0x100);
  print_number((slot + 128) * 8 - 1, 16);
  print_string(" (32 per line)\n", 0x100);
  print_string(
      "---------------------------------------------------------------\n", 64);
}

void print_bitmap() {
  uint64_t total_bits = the_bitmap.size * BITS_PER_SLOT;
  uint64_t slot = 0, subindex = 0;

  print_header(slot);
  for (slot = 0; slot * BITS_PER_SLOT + subindex < total_bits; slot++) {
    if (slot % 128 == 0 && slot != 0) {
      print_string(
          "---------------------------------------------------------------\n",
          64);
      print_color_string("Bitmap state - 0 FREE - 1 OCCUPIED\nEach bit "
                         "represents a page, PAGESIZE = 4096B\n",
                         0x100, 0x00FF00);
      sleep(5);
      hvd_clear();
      print_header(slot);
    } else if (slot % 4 == 0 && slot != 0) {
      print_char('\n');
    }
    for (subindex = 0; subindex < BITS_PER_SLOT; subindex++) {
      print_number((the_bitmap.bitmap_array[slot] >> subindex) & 0x1, 10);
      print_char(' ');
    }
    subindex = 0;
  }
}

