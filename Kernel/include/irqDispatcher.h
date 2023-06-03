#ifndef _IRQDISPATCHER_H
#define _IRQDISPATCHER_H

#include "syscalls.h"
#include <keyboardDriver.h>
#include <lib.h>
#include <stdarg.h>
#include <stdint.h>
#include <syscalls.h>
#include <textDriver.h>
#include <time.h>
#include <videoDriver.h>

void initialize_irq_functions_array();

#endif