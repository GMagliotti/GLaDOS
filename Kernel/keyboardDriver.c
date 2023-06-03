#include <keyboardDriver.h>

static const uint8_t charHexMap[256] = {
    0x1B, 0,       '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',  '=',
    '\b', '	', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[',  ']',
    '\n', 0,       'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', 0,
    0,    '\\',    'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0,    '*',
    0,    ' ',     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
    0,    0,       0,   0,   0,   0,   0,   0,   0,   0,

};

uint8_t get_character_from_keyboard_hex(uint8_t kb_hex) {
  return charHexMap[kb_hex];
}