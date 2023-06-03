#ifndef MODULELOADER_H
#define MODULELOADER_H

#include <lib.h>
#include <stdint.h>
#include <textModeDriver.h>
#include <videoDriver.h>

void load_modules(void *payload_start, void **module_target_addres);

#endif