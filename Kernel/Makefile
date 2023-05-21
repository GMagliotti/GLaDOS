include Makefile.inc

KERNEL=kernel.bin
# SOURCES=$(wildcard *.c) $(wildcard VideoDriver/*.c) $(wildcard MemoryManager/*.c) 
SOURCES_ASM=$(wildcard asm/*.asm)

ifdef enable_buddy
	SOURCES := $(wildcard *.c) $(wildcard VideoDriver/*.c) $(wildcard MemoryManager/Buddy.c) 
	MEMORY_MANAGER_FLAGS := -DIS_BUDDY
else
	SOURCES := $(wildcard *.c) $(wildcard VideoDriver/*.c) $(wildcard MemoryManager/MemoryManager.c) $(wildcard MemoryManager/Bitmap.c) 
endif

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm
LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin MemoryManager/*.o

.PHONY: all clean
