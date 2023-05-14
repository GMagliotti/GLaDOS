#include <videoDriver.h>
#define NULL 0

typedef int (*prueba)();

typedef struct vbe_mode_info_structure {
    uint16_t attributes;        // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
    uint8_t window_a;            // deprecated
    uint8_t window_b;            // deprecated
    uint16_t granularity;        // deprecated; used while calculating bank numbers
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;        // deprecated; used to switch banks from protected mode without returning to real mode
    uint16_t pitch;            // number of bytes per horizontal line
    uint16_t width;            // width in pixels
    uint16_t height;            // height in pixels
    uint8_t w_char;            // unused...
    uint8_t y_char;            // ...
    uint8_t planes;
    uint8_t bpp;            // bits per pixel in this mode
    uint8_t banks;            // deprecated; total number of banks in this mode
    uint8_t memory_model;
    uint8_t bank_size;        // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
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

    uint32_t framebuffer;        // physical address of the linear frame buffer; write here to draw to the screen
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size;    // size of memory in the framebuffer but not being displayed on the screen
    uint8_t reserved1[206];
} __attribute__ ((packed)) VBEModeInfoStructT;


int bashFlag = 1; //flag que nos dice si se esta corriendo la terminal, o el tron
void setBashFlag(int num){bashFlag = num;}
int getBashFlag(){return bashFlag;}

// void vprintf(const uint8_t * fmt, va_list ap);

int strLength(char* str);
int stringEquals(char* buf, char* arr);

void time();
void help();

VBEModeInfoStructT * VBEModeInfoBlock = (VBEModeInfoStructT *) 0x0000000000005C00;
uint16_t getVBEHeight(){return VBEModeInfoBlock->height;}
uint16_t getVBEWidth(){return VBEModeInfoBlock->width;}

uint32_t videobuffer[1024 * 768] = {0};


/* 
	Paints a pixel in the position specified in the color specified. 
   The format of the color argument should be 0x00RRGGBB, where each pair of letters
   indicate the opacity of that color (R red, G green, B blue)
*/
void putpixel(uint32_t x, uint32_t y, uint32_t color) {
	// This "double-cast" is the only thing that avoids gcc 
	// from complaining that the address wont fit
	// First cast forces the value to be the size of an address pointer
	// Second cast tells the value that it is an address to chars
	
	uint8_t * screen = (uint8_t *) ((uint64_t) (VBEModeInfoBlock->framebuffer));
    uint32_t where = y * (VBEModeInfoBlock->pitch) + (x * ((VBEModeInfoBlock->bpp)/8));
    screen[where]     = color & 0xFF;          // BLUE
    screen[where + 1] = (color >> 8) & 0xFF;   // GREEN
    screen[where + 2] = (color >> 16) & 0xFF;  // RED
}

/*
	Paints an area of pixels in the color and position specified.
	The position (or x and y values) indicate the starting vertex of the rectangle
	and it grows RIGHTWARDS in width and DOWNWARDS in height
	The format of the color argument should be 0x00RRGGBB, where each pair of letters
    indicate the opacity of that color (R red, G green, B blue).
*/
void fillrect(uint16_t x, uint16_t y, uint32_t rgbValue, uint16_t w, uint16_t h) {
	// Maybe it's better if we just ignore the warning?
	uint8_t *where = (uint8_t *) ((uint64_t) ((VBEModeInfoBlock->framebuffer) + y * (VBEModeInfoBlock->pitch) + (x * ((VBEModeInfoBlock->bpp)/8))));
    //uint8_t *where = (VBEModeInfoBlock->framebuffer);
    int i, j;

	uint32_t pixelwidth = ((VBEModeInfoBlock->bpp)/8), pitch = VBEModeInfoBlock->pitch;
	//uint8_t *where = (VBEModeInfoBlock->framebuffer) + y * pitch + (x * pixelwidth);
 
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            //putpixel(vram, 64 + j, 64 + i, (r << 16) + (g << 8) + b);
            where[j*pixelwidth] = rgbValue & 0xFF; 
            where[j*pixelwidth + 1] = (rgbValue >> 8) & 0xFF;
            where[j*pixelwidth + 2] = (rgbValue >> 16) & 0xFF;
        }
        where+=pitch;
    }
}

void debugPrintGrid() {
	uint32_t screenwidth = VBEModeInfoBlock->width, screenheight = VBEModeInfoBlock->height;
	uint32_t i, j;
	for ( j = 0; j < screenheight; j += screenheight/32 ) {	
		for ( i = 0; i < screenwidth; i += screenwidth/32 ) {
			fillrect(i, j, 0xFF00FF, 1, screenheight);
		}
		fillrect(i, j, 0xFF00FF, screenwidth, 1);
	}
}

/* Returns the width (in pixels) of the current screen */
uint32_t getScreenWidth(){
	return VBEModeInfoBlock->width;
}

/* Returns the height (in pixels) of the current screen */
uint32_t getScreenHeight(){
	return VBEModeInfoBlock->height;
}

/* Clears the screen completely, leaving it completely black */
void hvdClear() {
	fillrect(0, 0, 0x0, VBEModeInfoBlock->width, VBEModeInfoBlock->height);
	setBash();
//	memset(screenText, 0, (screenPixelHeight*screenPixelWidth)/64);		// screenText vuelve a estar vacio
	}

// Clears the screen if the next font to write is beyond the boundaries of the screen
uint8_t scrollIfFullScreen(int penPTRY) {									//TODO BIEN A FUTURO - Implementar bien el scroll
	if (penPTRY >= VBEModeInfoBlock->height) {
		hvdClear();
		return 1;
	}
	return 0;
}


uint64_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

