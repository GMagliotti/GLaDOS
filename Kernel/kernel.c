#include <stdint.h>
#include <videoDriver.h>
#include <lib.h>
#include <idtLoader.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <interrupts.h>
#include <MemoryManager.h>
#include "./Scheduler/include/scheduler.h"
#include "include/lib.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

MemoryManagerADT the_memory_manager = NULL;
rr_queue_ptr the_scheduler = NULL;
static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

void idle(int, char **);

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
	// memset(&bss, 0, &endOfBinary - &bss);
}

void * getStackBase() {
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary() {
	char buffer[10];

	

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	memset(&bss, 0, &endOfKernel - &bss);
	// The &endOfKernel variable above is, for some reason, the address 0x409000 
	// Instead of the expected 0x109000
	// This completely breaks the OS since all memory from BSS (0x108000) all the way past the
	// userland (0x409000) gets completely wiped back to 0
	// The solution was to hardcode the .bss size, we do however need to find why
	// the address is so off the mark, it is the same file since the cloning after all.
	// In addition, the clearBSS clearly overwrites the code (hence, the invalid opcode thrown at
	// 0x400000, the start of a function which works) but this makes more questions pop up 
	// since the code at 0x400000 is in section .text which should be WRITE PROTECTED

	// clearBSS(&bss, 0x1000);	

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

extern void testInvalidExc();
uint64_t test_sync(uint64_t argc, char* argv[]);


int main() {	
	hvdClear();
	the_memory_manager = createMemoryManager((void *)0x50000, (void *)0x1000000);
	the_scheduler = create_scheduler(idle, sampleCodeModuleAddress);
	scheduler_create_process("test", 0, NULL, test_sync, FOREGROUND);
	load_idt();

	char * arr[3] = {"1", "2", "3"};

	while(1) {
		printString("Mistakes were made...", 30);
		_hlt();
	}
	return 0;
}

void idle(int argc, char ** argv) {
	while (1) {
		printColorString("This is truly a runescape crocodile moment", 0xFFFFFFFFFFFFFFFF, 0x0000FF);
		printChar('\n');
		_hlt();
	}
}

// /*** TESTING AREA ***/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "Semaphore/include/semaphore.h"

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  sleep(1);
  // my_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]) {
  uint64_t n = 100;
  int8_t inc = 1;
  int8_t use_sem = create_sem(1, "thesem");

  if (argc != 3)
    return -1;

//   if ((n = satoi(argv[0])) <= 0)
//     return -1;
//   if ((inc = satoi(argv[1])) == 0)
//     return -1;
//   if ((use_sem = satoi(argv[2])) < 0)
//     return -1;

  if (use_sem)
    if (!sem_open(SEM_ID)) {
      printColorString("test_sync: ERROR opening semaphore\n", 0xFFFFF, 0xFFFFFF);
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      sem_wait(SEM_ID);
	  printColorString("I'm in", 7, 0xFF0000);
	  printChar('\n');
    slowInc(&global, inc);
    if (use_sem)
	  printColorString("I'm out", 7, 0x00FF00);
	  printChar('\n');
      sem_post(SEM_ID);
  }

  if (use_sem)
    sem_close(SEM_ID);

  return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 2)
    return -1;

  char *argvDec[] = {argv[0], "-1", argv[1], NULL};
  char *argvInc[] = {argv[0], "1", argv[1], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    int fd[2] = {0, 1};
    pids[i] = create_process("my_process_inc", 3, argvDec, my_process_inc , FOREGROUND, fd);
    pids[i + TOTAL_PAIR_PROCESSES] = create_process("my_process_inc", 3, argvInc, my_process_inc, FOREGROUND, fd);
  }

//   for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
//     my_wait(pids[i]);
//     my_wait(pids[i + TOTAL_PAIR_PROCESSES]);
//   }

  while(1);

  return 0;
}
