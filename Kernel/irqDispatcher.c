#include <irqDispatcher.h>

extern uint8_t getKey();

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
	static bool shiftPressed = false, tabPressed = false;
	uint8_t keyScanCode = getKey();
	if (keyScanCode == 0x0F) tabPressed = true;
	else if (keyScanCode == 0x8F) tabPressed = false;
	else if (keyScanCode == 0x2A) shiftPressed = true;
	else if (keyScanCode == 0xAA) shiftPressed = false;
	else {
		int c = getCharacterFromKeyboardHex(keyScanCode);
		if (c != 0) {
		//si estoy en bash imprimo el caracter y ademas lo guardo en buffer (para su posterior validacion de comando)
		saveKey(c);
		}
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
		case 2: //putchar que usara el printf
			sys_putchar((int)rsi); 
			break;
		case 3:
			sys_putpixel((uint32_t)rsi, (uint32_t)rdx, (uint32_t)rcx);
			break;
		case 4:
			sys_read(rsi, (char *)rdx, rcx);
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
			return sys_getchar();
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
		default:
			return 0;
			break;
 	}
	return 0;
}

