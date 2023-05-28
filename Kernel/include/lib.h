#ifndef LIB_H
#define LIB_H

#include <stdint.h>

#include <time.h>
#include "interrupts.h"

#define RTCSECONDS      0x00
#define RTCSECONDSALARM 0x01
#define RTCMINUTES      0x02
#define RTCMINUTESALARM 0x03
#define RTCHOURS        0x04
#define RTCHOURSALARM   0x05
#define RTCDAYOFWEEK    0x06
#define RTCDAYOFMONTH   0x07
#define RTCMONTH        0x08
#define RTCYEAR         0x09
#define RTCCENTURY      0x32
#define RTCREGISTERA    0x0A
#define RTCREGISTERB    0x0B
#define RTCREGISTERC    0x0C
#define RTCREGISTERD    0x0D



void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

uint8_t accessRTC(uint8_t reg);

void sleep(uint32_t seconds);
void sleepms(int mseconds);

int32_t strlen(char * string);

#endif