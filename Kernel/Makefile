include Makefile.inc

KERNEL=kernel.bin
# SOURCES=$(wildcard *.c) $(wildcard VideoDriver/*.c) $(wildcard MemoryManager/*.c) 
SOURCES_ASM=$(wildcard asm/*.asm) $(wildcard Semaphore/*.asm)

ifdef enable_buddy
	SOURCES := $(wildcard *.c) $(wildcard VideoDriver/*.c) $(wildcard MemoryManager/buddy.c) $(wildcard Scheduler/*.c) $(wildcard Semaphore/*.c)
	MEMORY_MANAGER_FLAGS := -DIS_BUDDY
else
	SOURCES := $(wildcard *.c) $(wildcard VideoDriver/*.c) $(wildcard MemoryManager/memoryManager.c) $(wildcard MemoryManager/bitmap.c) $(wildcard Scheduler/*.c) $(wildcard Semaphore/*.c)
endif

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm
LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin ./*/*.o

.PHONY: all clean
