// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <textDriver.h>

static int pen_ptr_x = 0, pen_ptr_y = 0;

uint8_t (*fontType)[][8] = &font8x8_basic;

uint16_t font_size = 2; // default font size
uint16_t font_width = DEFAULT_FONT_WIDTH;
uint16_t font_height = DEFAULT_FONT_HEIGHT;
uint32_t font_color = DEFAULT_FONT_COLOR;

void set_ptrx(int num) { pen_ptr_x = num; }
void set_ptry(int num) { pen_ptr_y = num; }

void set_font_size(int size) {
  // parameter validation done in shell
  font_size = size;
}

void set_bash() {
  pen_ptr_x = 0;
  pen_ptr_y = 0;
}

void print_char(uint8_t key_code) {
  uint8_t letter = key_code; // get_character_from_keyboard_hex(key_code);
  if (letter == '\0')
    return;
  if (letter == '\n') { 
    pen_ptr_y += font_size * font_height;
    pen_ptr_x = 0;
    if (scroll_if_full_screen(pen_ptr_y))
      set_bash();
    return;
  } else if (letter == '\b') {
    pen_ptr_x -= font_size * font_width;
    if (pen_ptr_x < 0) {
      pen_ptr_x = ((get_screen_width() / (font_size * font_width)) - 1) *
                      (font_size * font_width) -
                  1;
      pen_ptr_y -= font_size * font_height;
    }
    letter = ' ';
  }

  int bitmap_offset = letter;

  if (pen_ptr_x + font_size * font_width > get_screen_width()) {
    pen_ptr_y += font_size * font_height;
    pen_ptr_x = 0;
    if (scroll_if_full_screen(pen_ptr_y))
      set_bash();
  }

  for (int y = 0; y < font_height; y++) {
    uint8_t bitmap_hex = (*fontType)[bitmap_offset][y];
    for (int x = 0; x < font_width; x++) {
      if ((bitmap_hex >> x) & 0x01)
        fill_rect(pen_ptr_x + x * font_size, pen_ptr_y + y * font_size,
                  font_color, font_size, font_size);
      else
        fill_rect(pen_ptr_x + x * font_size, pen_ptr_y + y * font_size, 0x0,
                  font_size, font_size);
    }
  }

  if (key_code != '\b')
    pen_ptr_x += font_size * font_width;
}

void print_string(char *string, uint64_t str_length) {
  for (int i = 0; i < str_length && string[i] != '\0'; i++)
    print_char(string[i]);
}

void print_color_string(char *string, uint64_t str_length, uint32_t color) {
  font_color = color;
  print_string(string, str_length);
  font_color = DEFAULT_FONT_COLOR;
}

void print_number(int value, int base) {
  char buffer_aux[64] = {0};
  print_string(buffer_aux, uint_to_base(value, buffer_aux, base));
}

// returns 1 if strings are equal, 0 if not
int string_equals(char *buf, char *arr) {
  int len = str_length(buf); // length del buffer
  if (len != str_length(arr))
    return 0;
  for (int j = 0; j < len; j++) {
    if (buf[j] != arr[j])
      return 0;
  }
  return 1;
}

int str_length(char *str) {
  int i = 0;
  while (str[i] != 0) {
    i++;
  }
  return i;
}

void str_cpy(char *dest, char *src) {
  int i = 0;
  while (src[i] != '\0') {
    dest[i] = src[i];
    i++;
  }
  dest[i] = '\0'; // Don't forget to add the null terminator at the end
}

/* -------------------------------- SAVING OF KEY INTERRUPTS
 * --------------------------------*/
static char key_buffer[256] = {0};
static int key_buffer_pos = 0;
static int retrieved_pos =
    0; // will track till what buffer position userland already requested char
void save_key(uint8_t c) {
  if (key_buffer_pos + 1 == 256) {
    clear_buffer();
  }
  key_buffer[key_buffer_pos++] = c;
}
void clear_buffer() {
  memset(key_buffer, 0, 256); // clear buffer
  key_buffer_pos = 0;
  retrieved_pos = 0;
}
char get_char() {
  if (retrieved_pos < key_buffer_pos)
    return key_buffer[retrieved_pos++];
  return 0;
}

void reverse(char str[], int length) {
  int start = 0;
  int end = length - 1;
  while (start < end) {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    end--;
    start++;
  }
}

char *int_to_string(int num, char *str, int base) {
  int i = 0;

  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return str;
  }

  // Process individual digits
  while (num != 0) {
    int rem = num % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num = num / base;
  }

  str[i] = '\0';

  reverse(str, i);
  return str;
}