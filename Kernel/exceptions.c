// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <registers.h>
#include <textDriver.h>
#include <videoDriver.h>

#define ZERO_EXCEPTION_ID 0
#define IOPCODE_EXCEPTION_ID 6
#define GENERAL_PROTECTION_EXCEPTION_ID 13
#define PG_FAULT_EXCEPTION_ID 14

extern void loader();
extern void sys_hlt();

static void zero_division();
static void invalid_opcode();
static void general_protection_exc();
static void page_fault();
void guruMeditation();
void terminate();

void exceptionDispatcher(int exception, registerStructT *dumped_registers) {
  switch (exception) {
  case ZERO_EXCEPTION_ID:
    zero_division();
    break;
  case IOPCODE_EXCEPTION_ID:
    invalid_opcode();
    break;
  case GENERAL_PROTECTION_EXCEPTION_ID:
    general_protection_exc();
    break;
  case PG_FAULT_EXCEPTION_ID:
    page_fault();
    break;
  default:
    hvd_clear();
    print_color_string("Paso algo...\n", 99, RED);
    while (1)
      ;
    break;
  }
  print_current_registers(dumped_registers);
  sleep(3);
  print_string("Press any key to continue...\n", 99);
  char key = 0;
  while (key == 0) {
    key = get_char();
    sys_hlt();
  }
}

static void zero_division() {
  hvd_clear(); // _handler para manejar exception de div 0
  print_color_string("Floating point exception\n", 99, RED);
}

static void
invalid_opcode() { // _handler para manejar exception de codigo de op invalido
  hvd_clear();
  print_color_string("Invalid opcode\n", 99, RED);
}

static void page_fault() { // Falta un comentario de lo que hace esta exception
  hvd_clear();
  print_color_string("Falla de segmentacion\n", 99, RED);
}

static void general_protection_exc() {
  hvd_clear();
  print_color_string("Wait for it...", 15, RED);
  sleep(1);
  print_color_string("SEGMENTATION FAULT", 99, RED);
}

void guruMeditation() {
  hvd_clear();
  fill_rect(0, 0, 0xFF0000, get_screen_width(), get_screen_height());
  set_font_size(6);
  print_color_string("GURU MEDITATION", 16, 0xFF0000);
  print_char('\n');
  set_font_size(4);
  print_color_string("Fatal error in kernel!", 24, 0xFF0000);
  print_char('\n');
  set_font_size(2);
  print_current_registers(&registerDump);
  haltcpu();
}

// void terminate() {
//   hvd_clear();
//   while (1)
//     ;
// }
