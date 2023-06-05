#include <stdio.h>
#include <user_syscalls.h>

#define BUFFER_SIZE 64
#define STDOUT 1
#define STDERR 2

char buffer[BUFFER_SIZE] = {0};
uint16_t buffer_pos = 0;
uint64_t uint_to_base(uint64_t value, char *buffer, uint32_t base);
void get_input();
int base_to_int(uint32_t base, uint8_t *err_flag);
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
        if (*c <= 0x7F && *c >= 0x10)
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

// int scanf(const char *format, ...) {
//   clear_buffer();
//   int ret;
//   va_list args;
//   va_start(args, format);
//   ret = vscanf(format, args);
//   va_end(args);
//   return ret;
// }

// int vscanf(const char *format, va_list args) {
//   int ret = 0;
//   uint8_t flag = 0;
//   while (*format != '\0') {
//     if (buffer_pos == BUFFER_SIZE) {
//       clear_buffer();
//     }
//     if (*format == '%') {
//       ret++;
//       format++;
//       switch (*format) {
//       case 'd': {
//         int *i;
//         i = va_arg(args, int *);
//         get_input();
//         *i = base_to_int(10, &flag);
//       } break;
//       case 'i': {
//         int *i;
//         i = va_arg(args, int *);
//         get_input();
//         *i = base_to_int(10, &flag);
//       } break;
//       case 'u': {
//         int *i;
//         i = va_arg(args, int *);
//         get_input();
//         *i = base_to_uint(buffer, 10);
//         if (*i < 0) {
//           flag = 1;
//         }
//       } break;
//       case 'x': {
//         int *i;
//         i = va_arg(args, int *);
//         get_input();
//         *i = base_to_uint(buffer, 16);
//         if (*i < 0) {
//           flag = 1;
//         }
//       } break;
//       case 'X': {
//         int *i;
//         i = va_arg(args, int *);
//         get_input();
//         *i = base_to_uint(buffer, 16);
//         if (*i < 0) {
//           flag = 1;
//         }
//       } break;
//       case 'o': {
//         int *i;
//         i = va_arg(args, int *);
//         get_input();
//         *i = base_to_uint(buffer, 8);
//         if (*i < 0) {
//           flag = 1;
//         }
//       } break;
//       case 'c': {
//         char *c;
//         c = va_arg(args, char *);
//         get_input();
//         *c = buffer[buffer_pos++];
//       } break;
//       default:
//         break;
//       }
//     } else {
//       get_input();
//       while (*format != '%' && *format != 0) {
//         putc(buffer[buffer_pos]);
//         if (buffer[buffer_pos++] != *format) {
//           return ret;
//         }
//         format++;
//       }

//       while (buffer[buffer_pos] == '\n') {
//         buffer_pos++;
//       }

//       if (*format == 0 ||
//           buffer[buffer_pos] != 0) { // if buffer is not finished, or if
//           there
//                                      // is no more format to read, return
//         return ret;
//       }

//       clear_buffer();
//       format--;
//     }
//     if (flag == 1) {
//       return ret;
//     }
//     format++;
//   }
//   return ret;
// }

// uses get_char for input and returns the number inputted
void get_input() {
  int i = buffer_pos;
  char c;
  do {
    buffer[i] = get_char();
    if (buffer[i] != 0) {
      c = buffer[i++];
    }
  } while (c != '\n' && i < BUFFER_SIZE);

  if (i == BUFFER_SIZE) {
    buffer[i - 1] = '\0';
    call_to_sys_write("\nMAXIMO DE CARACTERES ALCANZADO\n", 32, STDERR);
  } else {
    buffer[i] = '\0';
  }
}

// function that converts a string to a signed integer
int32_t base_to_int(uint32_t base, uint8_t *err_flag) {
  char *str = buffer;
  int sign = 1;
  int32_t num = 0;
  if (str[0] == '-') {
    sign = -1;
    str += 1;
  }
  num = base_to_uint(str, base);
  if (sign == -1) {
    buffer_pos++;
  }
  if (num == -1) {
    *err_flag = 1;
  } else {
    *err_flag = 0;
  }
  return num * sign;
}

// function that converts a string to an unsigned integer
int32_t base_to_uint(char *str, uint8_t base) {
  int32_t ret = 0;
  int i = buffer_pos;
  while (str[i] != '\0' && str[i] != '\n') {
    if (str[i] >= '0' && str[i] <= '9') {
      ret = ret * base + (str[i] - '0');
    } else if (str[i] >= 'A' && str[i] <= 'F' && base == 16) {
      ret = ret * base + (str[i] - 'A' + 10);
    } else if (str[i] >= 'a' && str[i] <= 'f' && base == 16) {
      ret = ret * base + (str[i] - 'a' + 10);
    } else {
      call_to_sys_write("\nERROR: CARACTER NO VALIDO\n", 27, STDERR);
      call_to_sleep(3);
      buffer_pos = i;
      return ret == 0 && i != 0 ? -1 : ret;
    }
    i++;
  }
  buffer_pos = i;
  return ret;
}
