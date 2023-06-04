/* sampleCodeModule.c */
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
  shell();
  return 0;
}
