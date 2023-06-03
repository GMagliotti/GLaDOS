#include <moduleLoader.h>

static void loadModule(uint8_t **module, void *targetModuleAddress);
static uint32_t readUint32(uint8_t **address);

void load_modules(void *payload_start, void **targetModuleAddress) {
  int i;
  uint8_t *currentModule = (uint8_t *)payload_start;
  uint32_t moduleCount = readUint32(&currentModule);

  for (i = 0; i < moduleCount; i++)
    loadModule(&currentModule, targetModuleAddress[i]);
}

static void loadModule(uint8_t **module, void *targetModuleAddress) {
  uint32_t moduleSize = readUint32(module);

  hvd_print("  Will copy module at 0x", BLACK_BG | WHITE_LT);
  hvd_print_hex((uint64_t)*module, BLACK_BG | WHITE_LT);
  hvd_print(" to 0x", BLACK_BG | WHITE_LT);
  hvd_print_hex((uint64_t)targetModuleAddress, BLACK_BG | WHITE_LT);
  hvd_print(" (", BLACK_BG | WHITE_LT);
  hvd_print_dec(moduleSize, BLACK_BG | WHITE_LT);
  hvd_print(" bytes)", BLACK_BG | WHITE_LT);

  memcpy(targetModuleAddress, *module, moduleSize);
  *module += moduleSize;

  hvd_print(" [Done]", BLACK_BG | WHITE_LT);
  hvd_newline();
}

static uint32_t readUint32(uint8_t **address) {
  uint32_t result = *(uint32_t *)(*address);
  *address += sizeof(uint32_t);
  return result;
}
