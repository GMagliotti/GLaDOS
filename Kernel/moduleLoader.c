#include <moduleLoader.h>

static void loadModule(uint8_t ** module, void * targetModuleAddress);
static uint32_t readUint32(uint8_t ** address);

void loadModules(void * payloadStart, void ** targetModuleAddress)
{
	int i;
	uint8_t * currentModule = (uint8_t*)payloadStart;
	uint32_t moduleCount = readUint32(&currentModule);

	for (i = 0; i < moduleCount; i++)
		loadModule(&currentModule, targetModuleAddress[i]);
}

static void loadModule(uint8_t ** module, void * targetModuleAddress)
{
	uint32_t moduleSize = readUint32(module);

	hvdPrint("  Will copy module at 0x", BLACK_BG | WHITE_LT);
	hvdPrintHex((uint64_t)*module, BLACK_BG | WHITE_LT);
	hvdPrint(" to 0x", BLACK_BG | WHITE_LT);
	hvdPrintHex((uint64_t)targetModuleAddress, BLACK_BG | WHITE_LT);
	hvdPrint(" (", BLACK_BG | WHITE_LT);
	hvdPrintDec(moduleSize, BLACK_BG | WHITE_LT);
	hvdPrint(" bytes)", BLACK_BG | WHITE_LT);

	memcpy(targetModuleAddress, *module, moduleSize);
	*module += moduleSize;

	hvdPrint(" [Done]", BLACK_BG | WHITE_LT);
	hvdNewline();
}

static uint32_t readUint32(uint8_t ** address)
{
	uint32_t result = *(uint32_t*)(*address);
	*address += sizeof(uint32_t);
	return result;
}
