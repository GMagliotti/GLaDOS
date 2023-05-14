#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include <keyboardDriver.h>
#include <lib.h>
#include <stdarg.h>
#include <naiveConsole.h>
#include <interrupts.h>
#include <textDriver.h>
#include <time.h>
#include <syscalls.h>

extern uint8_t getKey();

void setTextBufferActive(bool active);

void debugPrintGrid();
uint8_t scrollIfFullScreen(int penPTRY);

void hvdClear();
char getChar();
void clearBuffer();
void saveKey(uint8_t c);

void setBash();

void putChar(uint8_t key); 
void printString(char * string, uint64_t strLength);

uint16_t getVBEHeight();
uint16_t getVBEWidth();

void putpixel(uint32_t x, uint32_t y, uint32_t color);
void fillrect(uint16_t x, uint16_t y, uint32_t rgbValue, uint16_t w, uint16_t h);

uint32_t getScreenWidth();
uint32_t getScreenHeight();

#endif