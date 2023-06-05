// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./Scheduler/include/scheduler.h"
#include "include/lib.h"
#include "include/pipe.h"
#include "memoryManager.h"
#include "syscalls.h"
#include <idtLoader.h>
#include <interrupts.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <videoDriver.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t end_of_kernel_binary;
extern uint8_t end_of_kernel;

memoryManagerADT the_memory_manager = NULL;
rr_queue_ptr the_scheduler = NULL;
static const uint64_t page_size = 0x1000;

static void *const sample_code_module_address = (void *)0x400000;
static void *const sample_data_module_address = (void *)0x500000;

void idle(int, char **);

typedef int (*EntryPoint)();

void clearBSS(void *bss_address, uint64_t bss_size) {
  memset(bss_address, 0, bss_size);
  // memset(&bss, 0, &endOfBinary - &bss);
}

void *get_stack_base() {
  return (void *)((uint64_t)&end_of_kernel +
                  page_size * 8      // The size of the stack itself, 32KiB
                  - sizeof(uint64_t) // Begin at the top of the stack
  );
}

void *initializeKernelBinary() {
  char buffer[10];

  nc_print("[x64BareBones]");
  nc_newline();

  nc_print("CPU Vendor:");
  nc_print(cpuVendor(buffer));
  nc_newline();

  nc_print("[Loading modules]");
  nc_newline();
  void *module_addresses[] = {sample_code_module_address,
                              sample_data_module_address};

  load_modules(&end_of_kernel_binary, module_addresses);
  nc_print("[Done]");
  nc_newline();
  nc_newline();

  nc_print("[Initializing kernel's binary]");
  nc_newline();

  memset(&bss, 0, &end_of_kernel - &bss);
  // The &end_of_kernel variable above is, for some reason, the address 0x409000
  // Instead of the expected 0x109000
  // This completely breaks the OS since all memory from BSS (0x108000) all the
  // way past the userland (0x409000) gets completely wiped back to 0 The
  // solution was to hardcode the .bss size, we do however need to find why the
  // address is so off the mark, it is the same file since the cloning after
  // all. In addition, the clearBSS clearly overwrites the code (hence, the
  // invalid opcode thrown at 0x400000, the start of a function which works) but
  // this makes more questions pop up since the code at 0x400000 is in section
  // .text which should be WRITE PROTECTED

  // clearBSS(&bss, 0x1000);

  nc_print("  text: 0x");
  nc_print_hex((uint64_t)&text);
  nc_newline();
  nc_print("  rodata: 0x");
  nc_print_hex((uint64_t)&rodata);
  nc_newline();
  nc_print("  data: 0x");
  nc_print_hex((uint64_t)&data);
  nc_newline();
  nc_print("  bss: 0x");
  nc_print_hex((uint64_t)&bss);
  nc_newline();

  nc_print("[Done]");
  nc_newline();
  nc_newline();
  return get_stack_base();
}

extern void test_invalid_exc();

int main() {
  hvd_clear();
  init_sem();
  init_pipes();
  the_memory_manager = create_memory_manager((void *)0x50000, (void *)0x800000);
  the_scheduler = create_scheduler(idle, sample_code_module_address);
  initialize_sys_blocking_sem();
  load_idt();

  while (1) {
    print_string("Mistakes were made...", 30);
    _hlt();
  }
  return 0;
}

void idle(int argc, char **argv) {
  while (1) {
    // print_color_string("This is truly a runescape crocodile moment",
    // 0xFFFFFFFFFFFFFFFF, 0x0000FF); print_char('\n');
    _hlt();
  }
}
