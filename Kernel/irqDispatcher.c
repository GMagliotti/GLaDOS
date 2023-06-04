#include "Scheduler/include/process.h"
#include "Scheduler/include/scheduler.h"
#include "Semaphore/include/semaphore.h"
#include "include/syscalls.h"
#include "include/textDriver.h"
#include "include/textModeDriver.h"
#include <irqDispatcher.h>

extern uint8_t get_key();

extern int r_w_sem_id;

static uint64_t int_20();
static uint64_t int_21();
static uint64_t int_80();

static uint64_t (*functions[256])(uint64_t, uint64_t, uint64_t, uint64_t,
                                  uint64_t);

void initialize_irq_functions_array() {
  functions[0x20] = int_20;
  functions[0x21] = int_21;
  functions[0x80] = int_80;
}

uint64_t irq_dispatcher(uint64_t irq, uint64_t rsi, uint64_t rdx, uint64_t rcx,
                        uint64_t r8, uint64_t r9) {
  return (*functions[irq + 0x20])(rsi, rdx, rcx, r8, r9);
}

uint64_t int_20() {
  timer_handler();
  return 0;
}

uint64_t int_21() {
  static bool shift_pressed = false, tab_pressed = false, ctrl_pressed = false;
  uint8_t key_scan_code = get_key();
  if (key_scan_code == 0x0F)
    tab_pressed = true;
  else if (key_scan_code == 0x8F)
    tab_pressed = false;
  else if (key_scan_code == 0x2A || key_scan_code == 0x36)
    shift_pressed = true;
  else if (key_scan_code == 0xAA || key_scan_code == 0xB6)
    shift_pressed = false;
  else if (key_scan_code == 0x1D)
    ctrl_pressed = true;
  else if (key_scan_code == 0x9D)
    ctrl_pressed = false;
  else {
    int c = get_character_from_keyboard_hex(key_scan_code);
    if (c == 0)
      return 0;
    if (ctrl_pressed && c == 'C') {
      print_char('^');
      print_char(c);
      clear_buffer();
      save_key('\n');
      kill_process(get_foreground_process());
    } else if (ctrl_pressed && c == 'Z') {
      print_char('^');
      print_char(c);
      clear_buffer();
      save_key('\n');
      block_process(get_foreground_process());
      print_number(get_foreground_process(), 10);
    } else if (ctrl_pressed && c == 'D') {
      save_key('\0');
      sys_write("\0", 1, NULL);
    } else if (shift_pressed && c == '7') {
      save_key('&');
    } else if (shift_pressed && c == '\\') {
      save_key('|');
    } else if (shift_pressed && c == '1') { // debugging
      ps();
    } else if (shift_pressed && c == '2') { // debugging
      print_scheduler_robin();
      return 0;
    } else {
      // si estoy en bash imprimo el caracter y ademas lo guardo en buffer (para
      // su posterior validacion de comando)
      save_key(c);
    }
    sem_post(r_w_sem_id);
  }
  if (tab_pressed && shift_pressed)
    return 1;
  else
    return 0;
}

uint64_t int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx,
                uint64_t r8, uint64_t r9) {
  switch (rdi) { // las funciones que estan comentadas no se usan en Userland
  case 0:
    sys_exit();
    break;
  case 1:
    sys_write((char *)rsi, rdx, rcx);
    break;
  case 2:
    break;
  case 3:
    sys_put_pixel((uint32_t)rsi, (uint32_t)rdx, (uint32_t)rcx);
    break;
  case 4:
    return sys_read(rsi, (char *)rdx, rcx);
    break;
  case 5:
    sys_sleep((uint32_t)rsi);
    break;
  case 6:
    sys_set_ptrx((int)rsi);
    break;
  case 7:
    sys_set_ptry((int)rsi);
    break;
  case 8:
    return sys_get_screen_height();
  case 9:
    return sys_get_screen_width();
  case 10:
    break;
  case 11:
    sys_clear_buffer();
    break;
  case 12:
    sys_beep((int)rsi, (int)rdx);
    break;
  case 13:
    sys_fill_rectangle((uint16_t)rsi, (uint16_t)rdx, (uint32_t)rcx,
                       (uint16_t)r8, (uint16_t)r9);
    break;
  case 14:
    sys_accessRTC((void *)rsi);
    break;
  case 15:
    return sys_getticks();
  case 16:
    sys_hlt();
    break;
  case 17:
    return sys_memoryAt((int)rsi);
  case 18:
    sys_setSize((int)rsi);
    break;
  case 19:
    sys_printRegisters();
    break;
  case 20:
    return (uint64_t)sys_malloc((uint32_t)rsi);
    break;
  case 21:
    sys_free((void *)rsi);
    break;
  case 22:
    return sys_getpid();
    break;
  case 23:
    sys_ps();
    break;
  case 24:
    sys_set_print_mode();
    break;
  case 25:
    return sys_kill((int)rsi);
    break;
  case 26:
    return sys_nice((int)rsi, (int)rdx);
    break;
  case 27:
    return sys_block((int)rsi);
    break;
  case 28:
    return sys_create_process((int)rsi, (char **)rdx, (void *)rcx, (int *)r8);
    break;
  case 29:
    return sys_waitpid((int)rsi);
    break;
  case 30:
    return sys_create_sem((int)rsi, (char *)rdx);
    break;
  case 31:
    sys_destroy_sem((int)rsi);
    break;
  case 32:
    return sys_sem_open((char *)rsi);
    break;
  case 33:
    return sys_sem_close((char *)rsi);
    break;
  case 34:
    return sys_sem_wait((int)rsi);
    break;
  case 35:
    return sys_sem_post((int)rsi);
    break;

  case 36:
    return sys_pipe_open((char *)rsi);
    break;
  case 37:
    return sys_pipe_close((int)rsi);
    break;
  case 38:
    return sys_unblock((int)rsi);
    break;
  case 39:
    sys_print_mem();
    break;
  case 40:
    sys_yield();
    break;
  case 41:
    sys_clear_screen();
    break;
  default:
    return 0;
    break;
  }
  return 0;
}
