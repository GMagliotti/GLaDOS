// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/ipc_processes.h"

void cat(int argc, char *argv[]) {
  char c;
  while ((c = get_char()) != 0)
    putc(c);
  putc('\n');
}

void wc(int argc, char *argv[]) {
  int lines = 0;
  char c;
  while ((c = get_char()) != 0) {
    if (c == '\n') {
      lines++;
      printf("Line count: %d\n", lines);
    }
  }
  putc('\n');
}

int is_vowel(char c) {
  return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
          c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}

void filter(int argc, char *argv[]) {
  char c;
  while ((c = get_char()) != 0) {
    if (!is_vowel(c)) {
      putc(c);
    }
  }
  putc('\n');
  return;
}