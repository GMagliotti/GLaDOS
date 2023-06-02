#include "Scheduler/include/process.h"
#include "Scheduler/include/scheduler.h"
#include "Semaphore/include/semaphore.h"
#include "include/syscalls.h"
#include "include/textDriver.h"
#include "include/textModeDriver.h"
#include <irqDispatcher.h>

extern uint8_t getKey();

extern int r_w_sem_id;

static uint64_t int_20();
static uint64_t int_21();
static uint64_t int_80();

static uint64_t (*functions[256])(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

void initializeIrqFunctionsArray() {
	functions[0x20] = int_20;
	functions[0x21] = int_21;
	functions[0x80] = int_80;
}

uint64_t irqDispatcher(uint64_t irq, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	return (*functions[irq+0x20])(rsi, rdx, rcx, r8, r9);
}

uint64_t int_20() {
	timer_handler();
	return 0;
}

uint64_t int_21() {
	static bool shiftPressed = false, tabPressed = false, ctrlPressed = false;
	uint8_t keyScanCode = getKey();
	if (keyScanCode == 0x0F) tabPressed = true;
	else if (keyScanCode == 0x8F) tabPressed = false;
	else if (keyScanCode == 0x2A || keyScanCode == 0x36) shiftPressed = true;
	else if (keyScanCode == 0xAA || keyScanCode == 0xB6) shiftPressed = false;
	else if (keyScanCode == 0x1D) ctrlPressed = true;
	else if (keyScanCode == 0x9D) ctrlPressed = false;
	else {
		int c = getCharacterFromKeyboardHex(keyScanCode);
		if (c == 0) return 0;
		if (ctrlPressed && c == 'C') {
			printChar('^');
			printChar(c);
			clearBuffer();
			printChar('\n');
			kill_process(get_foreground_process());
		} else if (ctrlPressed && c == 'Z') {
			printChar('^');
			printChar(c);
			clearBuffer();
			printChar('\n');
			block_process(get_foreground_process());
		} else if (ctrlPressed && c == 'D') {
			saveKey('\0');
		} else if (shiftPressed && c == '7') {
			saveKey('&');
		} else if (shiftPressed && c == '\\') {
			saveKey('|');
		} else if (shiftPressed && c == '1') { // debugging
			ps();
			return 0;
		} else {
			//si estoy en bash imprimo el caracter y ademas lo guardo en buffer (para su posterior validacion de comando)
			saveKey(c);
		}
		sem_post(r_w_sem_id);
	}
	if (tabPressed && shiftPressed) return 1;
	else return 0;
}

uint64_t int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {	
	switch (rdi) {							// las funciones que estan comentadas no se usan en Userland
		case 0:
			sys_exit();
			break;
		case 1:
			sys_write((char *)rsi, rdx, rcx);
			break;
		case 2:
			break;
		case 3:
			sys_putpixel((uint32_t)rsi, (uint32_t)rdx, (uint32_t)rcx);
			break;
		case 4:
			return sys_read(rsi, (char *)rdx, rcx);
			break;
		case 5:
			sys_sleep((uint32_t)rsi);
			break;
		case 6:
			sys_setptrx((int) rsi);
			break;
		case 7:
			sys_setptry((int) rsi);
			break;
		case 8:
			return sys_getvbeheight();
		case 9:
			return sys_getvbewidth();
		case 10:
			break;
		case 11:
			sys_clearbuffer();
			break;
		case 12:
			sys_beep((int)rsi, (int)rdx);
			break;
		case 13:
			sys_fillrectangle((uint16_t)rsi, (uint16_t)rdx, (uint32_t)rcx, (uint16_t)r8, (uint16_t)r9);
			break;
		case 14:
			sys_accessRTC((void *) rsi);
			break;
		case 15:
			return sys_getticks();
		case 16:
			sys_hlt();
			break;
		case 17:
			return sys_memoryAt((int) rsi);
		case 18:
			sys_setSize((int) rsi);
			break;
		case 19:
			sys_printRegisters();
			break;
		case 20:
			return (uint64_t)sys_malloc((uint32_t) rsi);
			break;
		case 21:
			sys_free((void *) rsi);
			break;
		case 22:
			return sys_getpid();
			break;
		case 23:
			sys_ps();
			break;
		case 24:
			sys_set_print_mode();
			break;
		case 25:
			return sys_kill((int) rsi);
			break;
		case 26:
			return sys_nice((int) rsi, (int) rdx);
			break;
		case 27:
			return sys_block((int) rsi);
			break;
		case 28:
			return sys_create_process((int) rsi, (char**) rdx, (void *) rcx, (int *) r8);
			break;
		case 29:
			return sys_waitpid((int) rsi);
			break;
		default:
			return 0;
			break;
 	}
	return 0;
}

