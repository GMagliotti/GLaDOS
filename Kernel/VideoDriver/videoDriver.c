#include <videoDriver.h>

typedef int (*prueba)();

typedef struct vbe_mode_info_structure {
  uint16_t
      attributes; // deprecated, only bit 7 should be of interest to you, and it
                  // indicates the mode supports a linear frame buffer.
  uint8_t window_a;     // deprecated
  uint8_t window_b;     // deprecated
  uint16_t granularity; // deprecated; used while calculating bank numbers
  uint16_t window_size;
  uint16_t segment_a;
  uint16_t segment_b;
  uint32_t win_func_ptr; // deprecated; used to switch banks from protected mode
                         // without returning to real mode
  uint16_t pitch;        // number of bytes per horizontal line
  uint16_t width;        // width in pixels
  uint16_t height;       // height in pixels
  uint8_t w_char;        // unused...
  uint8_t y_char;        // ...
  uint8_t planes;
  uint8_t bpp;   // bits per pixel in this mode
  uint8_t banks; // deprecated; total number of banks in this mode
  uint8_t memory_model;
  uint8_t bank_size; // deprecated; size of a bank, almost always 64 KB but may
                     // be 16 KB...
  uint8_t image_pages;
  uint8_t reserved0;

  uint8_t red_mask;
  uint8_t red_position;
  uint8_t green_mask;
  uint8_t green_position;
  uint8_t blue_mask;
  uint8_t blue_position;
  uint8_t reserved_mask;
  uint8_t reserved_position;
  uint8_t direct_color_attributes;

  uint32_t framebuffer; // physical address of the linear frame buffer; write
                        // here to draw to the screen
  uint32_t off_screen_mem_off;
  uint16_t off_screen_mem_size; // size of memory in the framebuffer but not
                                // being displayed on the screen
  uint8_t reserved1[206];
} __attribute__((packed)) VBEModeInfoStructT;

// void vprintf(const uint8_t * fmt, va_list ap);

int str_length(char *str);
int string_equals(char *buf, char *arr);

void time();
void help();

VBEModeInfoStructT *vbe_mode_info_block =
    (VBEModeInfoStructT *)0x0000000000005C00;

// TODO verify if deprecated, we could use it in the future for double
// buffer video memory
// uint32_t videobuffer[1024 * 768] = {0};

/*
        Paints a pixel in the position specified in the color specified.
   The format of the color argument should be 0x00RRGGBB, where each pair of
   letters indicate the opacity of that color (R red, G green, B blue)
*/
void put_pixel(uint32_t x, uint32_t y, uint32_t color) {
  // This "double-cast" is the only thing that avoids gcc
  // from complaining that the address wont fit
  // First cast forces the value to be the size of an address pointer
  // Second cast tells the value that it is an address to chars

  uint8_t *screen = (uint8_t *)((uint64_t)(vbe_mode_info_block->framebuffer));
  uint32_t where =
      y * (vbe_mode_info_block->pitch) + (x * ((vbe_mode_info_block->bpp) / 8));
  screen[where] = color & 0xFF;             // BLUE
  screen[where + 1] = (color >> 8) & 0xFF;  // GREEN
  screen[where + 2] = (color >> 16) & 0xFF; // RED
}

/*
        Paints an area of pixels in the color and position specified.
        The position (or x and y values) indicate the starting vertex of the
   rectangle and it grows RIGHTWARDS in width and DOWNWARDS in height The format
   of the color argument should be 0x00RRGGBB, where each pair of letters
    indicate the opacity of that color (R red, G green, B blue).
*/
void fill_rect(uint16_t x, uint16_t y, uint32_t rgb_value, uint16_t w,
               uint16_t h) {
  // Maybe it's better if we just ignore the warning?
  uint8_t *where =
      (uint8_t *)((uint64_t)((vbe_mode_info_block->framebuffer) +
                             y * (vbe_mode_info_block->pitch) +
                             (x * ((vbe_mode_info_block->bpp) / 8))));
  // uint8_t *where = (vbe_mode_info_block->framebuffer);
  int i, j;

  uint32_t pixel_width = ((vbe_mode_info_block->bpp) / 8),
           pitch = vbe_mode_info_block->pitch;
  // uint8_t *where = (vbe_mode_info_block->framebuffer) + y * pitch + (x *
  // pixel_width);

  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      // put_pixel(vram, 64 + j, 64 + i, (r << 16) + (g << 8) + b);
      where[j * pixel_width] = rgb_value & 0xFF;
      where[j * pixel_width + 1] = (rgb_value >> 8) & 0xFF;
      where[j * pixel_width + 2] = (rgb_value >> 16) & 0xFF;
    }
    where += pitch;
  }
}

void debug_print_grid() {
  uint32_t screen_width = vbe_mode_info_block->width,
           screen_height = vbe_mode_info_block->height;
  uint32_t i, j;
  for (j = 0; j < screen_height; j += screen_height / 32) {
    for (i = 0; i < screen_width; i += screen_width / 32) {
      fill_rect(i, j, 0xFF00FF, 1, screen_height);
    }
    fill_rect(i, j, 0xFF00FF, screen_width, 1);
  }
}

/* Returns the width (in pixels) of the current screen */
uint32_t get_screen_width() { return vbe_mode_info_block->width; }

/* Returns the height (in pixels) of the current screen */
uint32_t get_screen_height() { return vbe_mode_info_block->height; }

/* Clears the screen completely, leaving it completely black */
void hvd_clear() {
  fill_rect(0, 0, 0x0, vbe_mode_info_block->width, vbe_mode_info_block->height);
  set_bash();
  //	memset(screenText, 0, (screenPixelHeight*screenPixelWidth)/64);
  //// screenText vuelve a estar vacio
}

// Clears the screen if the next font to write is beyond the boundaries of the
// screen
uint8_t scroll_if_full_screen(
    int pen_ptr_y) { // TODO BIEN A FUTURO - Implementar bien el scroll
  if (pen_ptr_y >= vbe_mode_info_block->height) {
    hvd_clear();
    return 1;
  }
  return 0;
}

uint64_t uint_to_base(uint64_t value, char *buffer, uint32_t base) {
  char *p = buffer;
  char *p1, *p2;
  uint32_t digits = 0;

  // Calculate characters for each digit
  do {
    uint32_t remainder = value % base;
    *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
    digits++;
  } while (value /= base);

  // Terminate string in buffer.
  *p = 0;

  // Reverse string in buffer.
  p1 = buffer;
  p2 = p - 1;
  while (p1 < p2) {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    p1++;
    p2--;
  }

  return digits;
}
