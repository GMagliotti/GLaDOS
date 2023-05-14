#ifndef MODULELOADER_H
#define MODULELOADER_H

#include <textModeDriver.h>
#include <videoDriver.h>
#include <stdint.h>
#include <lib.h>

void loadModules(void * payloadStart, void ** moduleTargetAddress);

#endif