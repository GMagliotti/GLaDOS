// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <funclib.h>

// funcion auxiliar que me retorna la resta de dos strings (sus chars)
int string_equals(char *buf, char *arr) {
  int len = str_length(buf); // length del buffer
  if (len != str_length(arr))
    return 0;
  for (int j = 0; j < len; j++) {
    if (buf[j] != arr[j])
      return 0;
  }
  return 1;
}

int str_length(char *str) {
  int i = 0;
  while (str[i] != 0) {
    i++;
  }
  return i;
}

// int begins_with(char *container, char *contains) {
//   int len = str_length(contains); // length del buffer
//   if (len > str_length(container)) {
//     return -1;
//   }
//   int j = 0;
//   for (; j < len; j++) {
//     if (container[j] != contains[j])
//       return -1;
//   }
//   return j;
// }

int check_num_with_base(
    char val, int base) { // verifica si es un numero, si lo es, lo retorna
  switch (base) {
  case (2):
    if (val >= '0' && val <= '1') {
      return val - '0';
    }
    return -1;
    break;
  case (10):
    if (val >= '0' && val <= '9') {
      return val - '0';
    }
    return -1;
    break;
  case (16):
    if (val >= '0' && val <= '9') {
      return val - '0';
    } else if (val >= 'A' && val <= 'F') {
      return val - 'A' + 10;
    }
    return -1;
    break;
  default:
    return -1;
  }
}

int to_num_with_base(char *buffer, int from, int base) {
  int to_return = 0;
  for (int i = from; buffer[i] != 0; i++) {
    int currentNum = check_num_with_base(buffer[i], base);
    if (currentNum != -1) {
      to_return *= base;
      to_return += currentNum;
    } else {
      if (buffer[i] == ' ') {
        return -2;
      }
      return -1;
    }
  }
  return to_return;
}

int string_to_int(char *str) {
  int num = -1;
  if (str[0] == '0') {
    if (str[1] == 'X') {
      num = to_num_with_base(str, 2, 16);
    } else if (str[1] == 'B') {
      num = to_num_with_base(str, 2, 2);
    } else {
      num = to_num_with_base(str, 0, 10);
    }
  } else {
    num = to_num_with_base(str, 0, 10);
  }
  return num;
}

void sleeps(int seconds) { call_to_sleep(seconds * 1000); }

void sleep(int seconds) { sleeps(seconds); }

void sleepms(int milliseconds) { call_to_sleep(milliseconds); }

bool is_space(char c) { return c == ' ' || c == '\t' || c == '\n'; }

int is_only_space(char *str) {
  for (int i = 0; str[i] != 0; i++) {
    if (!is_space(str[i])) {
      return 0;
    }
  }
  return 1;
}

void print_memory_at(int argc, char **argv) {

  if (argc < 2) {
    printf("Invalid arguments, usage: mem <direction>\n");
    return;
  }

  int dir = string_to_int(argv[1]);

  if (dir < 0) { // habria que poner un maximo?
    printf("Invalid direction format, accepted bases: 2, 10, 16\n");
    return;
  }

  int aligneddir = dir & 0xFFFFFFFFFFFFFFF0; // Stack alignment

  printf("Displaying memory at 0x%x (Aligned format)\n", dir);
  for (int i = 0; i < 32; i += 4) {
    uint8_t b1 = call_to_memoryAt(aligneddir + i),
            b2 = call_to_memoryAt(aligneddir + i + 1),
            b3 = call_to_memoryAt(aligneddir + i + 2),
            b4 = call_to_memoryAt(aligneddir + i + 3);
    printf("%x | %x %x %x %x | %c %c %c %c\n", aligneddir + i, b1, b2, b3, b4,
           (char)b1, (char)b2, (char)b3, (char)b4);
    sleepms(250);
  }
}

// char *str_cat(char *destination, const char *source) {
//   // make `ptr` point to the end of the destination string
//   char *ptr = destination + str_length(destination);

//   // appends characters of the source to the destination string
//   while (*source != '\0') {
//     *ptr++ = *source++;
//   }

//   // null terminate destination string
//   *ptr = '\0';

//   // the destination is returned by standard `str_cat()`
//   return destination;
// }

// Retorna como parametro de entrada salida un string del numero ingresado.
// void num_to_str(int num, char *string, int len) {
//   string[len] = 0;
//   for (int i = len - 1; i >= 0; i--) {
//     string[i] = (num % 10) + '0';
//     num /= 10;
//   }
// }

// uint32_t uint_to_base2(uint64_t value, char *buffer, uint32_t base) {
//   char *p = buffer;
//   char *p1, *p2;
//   uint32_t digits = 0;

//   // Calculate characters for each digit
//   do {
//     uint32_t remainder = value % base;
//     *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
//     digits++;
//   } while (value /= base);

//   // Terminate string in buffer.
//   *p = 0;

//   // Reverse string in buffer.
//   p1 = buffer;
//   p2 = p - 1;
//   while (p1 < p2) {
//     char tmp = *p1;
//     *p1 = *p2;
//     *p2 = tmp;
//     p1++;
//     p2--;
//   }

//   return digits;
// }

// int atoi2(char *str) {
//   // Initialize result
//   int res = 0;

//   // Iterate through all characters
//   // of input string and update result
//   // take ASCII character of corosponding digit and
//   // subtract the code from '0' to get numerical
//   // value and multiply res by 10 to shuffle
//   // digits left to update running total
//   int flag = (str[0] == '-' ? -1 : 1);
//   if (flag == -1) {
//     str++;
//   }
//   for (int i = 0; str[i] != '\0'; ++i)
//     res = res * 10 + str[i] - '0';

//   // return result.
//   return res * flag;
// }

void reverse(char str[], int length) {
  int start = 0;
  int end = length - 1;
  while (start < end) {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    end--;
    start++;
  }
}

char *int_to_string(int num, char *str, int base) {
  int i = 0;

  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return str;
  }

  // Process individual digits
  while (num != 0) {
    int rem = num % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num = num / base;
  }

  str[i] = '\0';

  reverse(str, i);
  return str;
}
