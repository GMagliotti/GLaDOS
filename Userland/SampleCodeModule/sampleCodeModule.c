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

// debug //
#include "tests/test_mm.h"

void help();

int main() {
	hvdClear();
	// test_mm();
	shell();	
	return 0;
}
