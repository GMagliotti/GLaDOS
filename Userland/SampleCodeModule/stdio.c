// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <user_syscalls.h>

#define BUFFER_SIZE 64
#define STDOUT 1
#define STDERR 2

char buffer[BUFFER_SIZE] = {0};
uint16_t buffer_pos = 0;
uint64_t uint_to_base(uint64_t value, char *buffer, uint32_t base);

int base_to_uint(char *str, uint8_t base);
int vscanf(const char *format, va_list args);

void clear_buffer() {
  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = 0;
  }
}

void vprintf(char *fmt, va_list ap) {
  uint32_t length = 0;
  while (*fmt && length < BUFFER_SIZE) {
    if (*fmt == '%') {
      fmt++;
      switch (*fmt) {
      case 'd': {
        int i = va_arg(ap, int);
        length += uint_to_base(i, &(buffer[length]), 10);
      } break;
      case 's': {
        char *s = va_arg(ap, char *);
        while (*s != '\0') {
          buffer[length++] = *s;
          s++;
        }
      } break;
      case 'x': {
        int i = va_arg(ap, int);
        length += uint_to_base(i, &(buffer[length]), 16);
      } break;
      case 'o': {
        int i = va_arg(ap, int);
        length += uint_to_base(i, &(buffer[length]), 8);
      } break;
      case 'b': {
        int i = va_arg(ap, int);
        length += uint_to_base(i, &(buffer[length]), 2);
      } break;
      case 'c': {
        char *c = va_arg(ap, char *);
        if (*c >= 0x10)
          buffer[length++] = *c;
        else
          buffer[length++] = ' ';
      } break;
      default:
        buffer[length++] = *fmt;
        break;
      }
    } else {
      buffer[length++] = *fmt;
    }
    fmt++;
  }
  call_to_sys_write(buffer, length, STDOUT);
}

void printf(char *fmt, ...) {
  clear_buffer();
  va_list ap;
  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);
}

void putc(char character) { call_to_sys_write(&character, 1, STDOUT); }

uint64_t uint_to_base(uint64_t value, char *buffer, uint32_t base) {
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

char get_char() {
  uint8_t c;

  call_to_sys_read((uint8_t)STDIN, &c, (uint8_t)1);

  return c;
}