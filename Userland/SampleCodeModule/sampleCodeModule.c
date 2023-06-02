/* sampleCodeModule.c */
#include <stdio.h>
#include <user_syscalls.h>
#include <stdint.h>
#include <user_syscalls.h>
#include <shell.h>
#include <beeper.h>
#include <time.h>
#include <tests.h>
#include <funclib.h>

void help();

int main() {
	hvdClear();
	shell();	
	return 0;
}
