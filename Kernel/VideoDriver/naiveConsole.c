#include <naiveConsole.h>

static uint32_t uint_to_base(uint64_t value, char *buffer, uint32_t base);

static char buffer[64] = {[0 ... 63] = '0'}; // pvs warning fix
static uint8_t *const video = (uint8_t *)0xB8000;
static uint8_t *current_video = (uint8_t *)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25;

// uint8_t *get_current_video() { return current_video; }

void nc_print(const char *string) {
  int i;

  for (i = 0; string[i] != 0; i++)
    ncput_char(string[i]);
}

void ncput_char(char character) {
  *current_video = character;
  current_video += 2;
}

void nc_newline() {
  do {
    ncput_char(' ');
  } while ((uint64_t)(current_video - video) % (width * 2) != 0);
}

// void nc_print_dec(uint64_t value) { nc_print_base(value, 10); }

void nc_print_hex(uint64_t value) { nc_print_base(value, 16); }

// void nc_print_bin(uint64_t value) { nc_print_base(value, 2); }

void nc_print_base(uint64_t value, uint32_t base) {
  uint_to_base(value, buffer, base);
  nc_print(buffer);
}

// void nc_clear() {
//   int i;

//   for (i = 0; i < height * width; i++)
//     video[i * 2] = ' ';
//   current_video = video;
// }

static uint32_t uint_to_base(uint64_t value, char *buffer, uint32_t base) {
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
