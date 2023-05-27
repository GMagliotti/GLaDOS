#include <stdint.h>
#include <videoDriver.h>
#include <lib.h>
#include <idtLoader.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <interrupts.h>
#include <MemoryManager.h>
#include "./Scheduler/include/scheduler.h"

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
void p1(int, char **);
void p2(int, char **);
void p3(int, char **);
void p4(int, char **);

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
	// memset(&bss, 0, &endOfBinary - &bss);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
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

int main()
{	
	int fd[2] = { 0, 1 };
	hvdClear();
	the_memory_manager = createMemoryManager((void *)0x50000, (void *)0x1000000);
	the_scheduler = create_scheduler(idle);
	scheduler_create_process("p1", 0, NULL, p1, FOREGROUND, fd);
	scheduler_create_process("p2", 0, NULL, p2, FOREGROUND, fd);
	scheduler_create_process("p3", 0, NULL, p3, FOREGROUND, fd);
	scheduler_create_process("p4", 0, NULL, p4, FOREGROUND, fd);
	load_idt();
	// ((EntryPoint)sampleCodeModuleAddress)();
	printColorString("Scheduler creado", 0xFFFFFFFFFFFFFFFF, 0x00FF00);
	
	

	while(1) {
		printNumber(69, 10);
		_hlt();
	}
	return 0;
}

void idle(int argc, char ** argv) {
	while (1) {
		printColorString("This is truly a runescape crocodile moment", 0xFFFFFFFFFFFFFFFF, 0x0000FF);
		_hlt();
	}
}

void p1(int argc, char ** argv) {
	while (1) {
		printColorString("And a one ", 0xFFFFFFFFFFFFFFFF, 0xFFFFFF);
		printChar('\n');
		_hlt();
	}
}

void p2(int argc, char ** argv) {
	while (1) {
		printColorString("And a two ", 0xFFFFFFFFFFFFFFFF, 0x0000FF);
		printChar('\n');
		_hlt();
	}
}

void p3(int argc, char ** argv) {
	while (1) {
		printColorString("And a one, two, three, four ", 0xFFFFFFFFFFFFFFFF, 0x00FF00);
		printChar('\n');
		_hlt();
	}
}

void p4(int argc, char ** argv) {
	while (1) {
		printColorString("I AM ROCK FUCKING HARD AAAAAAA", 0xFFFFFFFFFFFFFFFF, 0xFF0000);
		printChar('\n');
		_hlt();
	}
}