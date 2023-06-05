// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <registers.h>

void print_current_registers(registerStructT *dumped_registers) {
  print_string("RIP=0x", 10);
  print_number(dumped_registers->rip, 16);
  print_char('\n');
  print_string("RAX=0x", 10);
  print_number(dumped_registers->rax, 16);
  print_char('\n');
  print_string("RBX=0x", 10);
  print_number(dumped_registers->rbx, 16);
  print_char('\n');
  print_string("RCX=0x", 10);
  print_number(dumped_registers->rcx, 16);
  print_char('\n');
  print_string("RDX=0x", 10);
  print_number(dumped_registers->rdx, 16);
  print_char('\n');
  print_string("RSP=0x", 10);
  print_number(dumped_registers->rsp, 16);
  print_char('\n');
  print_string("RBP=0x", 10);
  print_number(dumped_registers->rbp, 16);
  print_char('\n');
  print_string("RSI=0x", 10);
  print_number(dumped_registers->rsi, 16);
  print_char('\n');
  print_string("RDI=0x", 10);
  print_number(dumped_registers->rdi, 16);
  print_char('\n');
  print_string("R8 =0x", 10);
  print_number(dumped_registers->r8, 16);
  print_char('\n');
  print_string("R9 =0x", 10);
  print_number(dumped_registers->r9, 16);
  print_char('\n');
  print_string("R10=0x", 10);
  print_number(dumped_registers->r10, 16);
  print_char('\n');
  print_string("R11=0x", 10);
  print_number(dumped_registers->r11, 16);
  print_char('\n');
  print_string("R12=0x", 10);
  print_number(dumped_registers->r12, 16);
  print_char('\n');
  print_string("R13=0x", 10);
  print_number(dumped_registers->r13, 16);
  print_char('\n');
  print_string("R14=0x", 10);
  print_number(dumped_registers->r14, 16);
  print_char('\n');
  print_string("R15=0x", 10);
  print_number(dumped_registers->r15, 16);
  print_char('\n');
  print_string("RFLAGS=0x", 10);
  print_number(dumped_registers->rflags, 16);
  print_char('\n');
  print_string("SS=0x", 10);
  print_number(dumped_registers->ss, 16);
  print_char('\n');
  print_string("CS=0x", 10);
  print_number(dumped_registers->cs, 16);
  print_char('\n');
}