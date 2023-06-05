// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <idtLoader.h>
#include <interrupts.h>

#pragma pack(push) /* Push de la alineación actual */
#pragma pack(1)    /* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de interrupcion */
typedef struct {
  uint16_t offset_l;
  uint16_t selector;
  uint8_t cero;
  uint8_t access;
  uint16_t offset_m;
  uint32_t offset_h;
  uint32_t other_cero;
} DESCR_INT;

#pragma pack(pop) /* Reestablece la alineación actual */

DESCR_INT *idt = (DESCR_INT *)0; // IDT de 255 entradas

static void setup_IDT_entry(int index, uint64_t offset);

void load_idt() {

  _cli();
  setup_IDT_entry(0x20, (uint64_t)&_irq00_handler);
  setup_IDT_entry(0x21, (uint64_t)&_irq01_handler);
  setup_IDT_entry(0x80, (uint64_t)&_irq60_handler);
  setup_IDT_entry(0x00, (uint64_t)&_exception0_handler);
  setup_IDT_entry(0x06, (uint64_t)&_exception6_handler);
  setup_IDT_entry(0x0D, (uint64_t)&_exception13_handler);

  initialize_irq_functions_array();

  // Interrupciones timer tick y teclado unicamente
  pic_master_mask(0xFC);
  pic_slave_mask(0xFF);

  _sti();
}

static void setup_IDT_entry(int index, uint64_t offset) {
  idt[index].offset_l = (offset & 0xFFFF);
  idt[index].selector = 0x08;
  idt[index].offset_m = ((offset >> 16) & 0xFFFF);
  idt[index].offset_h = ((offset >> 32) & 0xFFFFFFFF);
  idt[index].access = ACS_INT;
  idt[index].cero = 0;
  idt[index].other_cero = (uint64_t)0;
}
