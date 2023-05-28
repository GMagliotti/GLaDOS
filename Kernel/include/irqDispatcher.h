#ifndef _IRQDISPATCHER_H
#define _IRQDISPATCHER_H

#include "syscalls.h"
#include <lib.h>
#include <videoDriver.h>
#include <stdarg.h>
#include <time.h>
#include <stdint.h>
#include <syscalls.h>
#include <time.h>
#include <keyboardDriver.h>
#include <textDriver.h>

void initializeIrqFunctionsArray();
void initializeSyscallsArray();

#endif