#include <videoDriver.h>
#include <textDriver.h>
#include <registers.h>

#define ZERO_EXCEPTION_ID 0
#define IOPCODE_EXCEPTION_ID 6
#define GENERAL_PROTECTION_EXCEPTION_ID 13
#define PG_FAULT_EXCEPTION_ID 14

static void zero_division();
static void invalid_opcode();
static void general_protection_exc();
static void page_fault();
void guruMeditation();
void terminate();

void exceptionDispatcher(int exception, registerStructT * dumpedRegisters) {	
	switch (exception) {
		case ZERO_EXCEPTION_ID:
			zero_division();
			break;
		case IOPCODE_EXCEPTION_ID:
			invalid_opcode();
			break;
		case GENERAL_PROTECTION_EXCEPTION_ID:
			general_protection_exc();
			break;
		case PG_FAULT_EXCEPTION_ID:
			page_fault();
			break;
		default:
			hvdClear();
			printColorString("Paso algo...\n", 99, RED);
			while(1);
			break;
	}	
	printCurrentRegisters(dumpedRegisters);
	sleep(3);
	printString("Press any key to continue...\n", 99);
	char key = 0;
	while (key == 0) {
		key = getChar();
		sys_hlt();
	}
}

static void zero_division() {
	hvdClear();												// Handler para manejar exception de div 0
	printColorString("Floating point exception\n", 99, RED);
}

static void invalid_opcode() {								// Handler para manejar exception de codigo de op invalido
	hvdClear();
	printColorString("Invalid opcode\n", 99, RED);
}

static void page_fault() {									// Falta un comentario de lo que hace esta exception
	hvdClear();
	printColorString("Falla de segmentacion\n", 99, RED);
}

static void general_protection_exc() {
	hvdClear();
	printColorString("Wait for it...", 15, RED);
	sleep(1);
	printColorString("SEGMENTATION FAULT", 99, RED);
}

void guruMeditation() {
	hvdClear();
	fillrect(0, 0, 0xFF0000, getVBEWidth(), getVBEHeight());
	setFontSize(6);
	printColorString("GURU MEDITATION", 16, 0xFF0000);
	printChar('\n');
	setFontSize(4);
	printColorString("Fatal error in kernel!", 24, 0xFF0000);
	printChar('\n');
	setFontSize(2);
	printCurrentRegisters(&registerDump);
	while(1);
}


void terminate() {
	hvdClear();
	while(1);
}
