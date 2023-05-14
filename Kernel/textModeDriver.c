#include <textModeDriver.h>

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static uint8_t buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25;

void hvdPrint(char * string, char attribute)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		hvdPutChar(string[i], attribute);
}

void hvdPutChar(char character, char attribute) {
    *currentVideo = character;
    *(currentVideo+1) = attribute;
    currentVideo += 2;
}

void hvdNewline()
{
	do
	{
		hvdPutChar(' ', BLACK_BG | BLACK_LT);
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void hvdPrintDec(uint64_t value, uint8_t attribute)
{
	hvdPrintBase(value, 10, attribute);
}

void hvdPrintHex(uint64_t value, uint8_t attribute)
{
	hvdPrintBase(value, 16, attribute);
}

void hvdPrintBin(uint64_t value, uint8_t attribute)
{
	hvdPrintBase(value, 2, attribute);
}

void hvdPrintBase(uint64_t value, uint32_t base, uint8_t attribute)
{
    uintToBase(value, buffer, base);
    hvdPrint(buffer, attribute);
}

void hvdEraseChar() {
	while ( (uint64_t)currentVideo % 2 != 0 ); // We cycle until we are on a character position!
	currentVideo -= 2;
	*(currentVideo) = 0x0;
	*(currentVideo+1) = 0x0;
}

void hvdEraseLine() {
	while ( (uint64_t)(currentVideo - video) % (width * 2) != 0 ) hvdEraseChar();
}

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

void textClear() {
	for ( int i = 0; i < 80*25*2; i++ ) {
		currentVideo = video;
		hvdPutChar(' ', WHITE_LT);
	}
}