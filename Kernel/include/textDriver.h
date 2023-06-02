#ifndef _TEXT_DRIVER_H
#define _TEXT_DRIVER_H
#include <stdint.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <stdarg.h>

#define DEFAULT_FONT_HEIGHT 8
#define DEFAULT_FONT_WIDTH 8
#define DEFAULT_FONT_COLOR 0xFFFFFF
#define RED 0xFF0000

void setFontType(int type);
void setFontSize(int size);
void setBash();

extern uint8_t getKey();
void saveKey(uint8_t c);

void setptrx(int num);
void setptry(int num);

void printChar(uint8_t keyCode);
void printString(char * string, uint64_t strLength);
void printColorString(char * string, uint64_t strLength, uint32_t color);
void printNumber(int value, int base);

int stringEquals(char* buf, char* arr);
int strLength(char* str);
void strCpy(char* dest, char* src);

void checkBuffer();
void saveChar(uint8_t c);
int getBufferPos();
char bufferAt(int n);

void checkAndScroll();

char* strCat(char* destination, const char* source);
char* int_to_string(int num, char* str, int base);

#endif