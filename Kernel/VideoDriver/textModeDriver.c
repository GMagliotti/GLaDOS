// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <textModeDriver.h>

static uint32_t uint_to_base(uint64_t value, char *buffer, uint32_t base);

static uint8_t *const video = (uint8_t *)0xB8000;
static uint8_t *current_video = (uint8_t *)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25;

void hvd_print(char *string, char attribute) {
  int i;

  for (i = 0; string[i] != 0; i++)
    hvd_put_char(string[i], attribute);
}

void hvd_put_char(char character, char attribute) {
  *current_video = character;
  *(current_video + 1) = attribute;
  current_video += 2;
}

void hvd_newline() {
  do {
    hvd_put_char(' ', BLACK_BG | BLACK_LT);
  } while ((uint64_t)(current_video - video) % (width * 2) != 0);
}

void hvd_print_dec(uint64_t value, uint8_t attribute) {
  hvd_print_base(value, 10, attribute);
}

void hvd_print_hex(uint64_t value, uint8_t attribute) {
  hvd_print_base(value, 16, attribute);
}

void hvd_print_base(uint64_t value, uint32_t base, uint8_t attribute) {

  uint8_t buffer[64] = {0};

  uint_to_base(value, (char *)buffer, base);
  hvd_print((char *)buffer, attribute);
}

uint32_t uint_to_base(uint64_t value, char *buffer, uint32_t base) {
  char *p = buffer;
  char *p1, *p2;
  uint32_t digits = 0;

  // Calculate characters for each digit
  do {
    uint32_t remainder = value % base;
    *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
    digits++;
  } while (value /= base);

  // Terminate string in buffer.
  *p = 0;

  // Reverse string in buffer.
  p1 = buffer;
  p2 = p - 1;
  while (p1 < p2) {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    p1++;
    p2--;
  }

  return digits;
}