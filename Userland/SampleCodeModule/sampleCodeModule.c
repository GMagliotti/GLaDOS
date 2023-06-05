// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* sampleCodeModule.c */
#include "include/beeper.h"
#include <beeper.h>
#include <funclib.h>
#include <shell.h>
#include <stdint.h>
#include <stdio.h>
#include <tests.h>
#include <time.h>
#include <user_syscalls.h>

void help();

int main() {
  hvd_clear();
  boot_sound();
  shell();
  return 0;
}
