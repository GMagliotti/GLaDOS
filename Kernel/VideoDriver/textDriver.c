#include <textDriver.h>

static int penPtrX = 0, penPtrY = 0;

uint8_t (*fontType)[][8] = &font8x8_basic;

uint16_t fontSize = 2; //default font size
uint16_t fontWidth = DEFAULT_FONT_WIDTH;
uint16_t fontHeight = DEFAULT_FONT_HEIGHT;
uint32_t fontColor = DEFAULT_FONT_COLOR;
static uint8_t buffer[64] = { '0' };

void setptrx(int num){penPtrX = num;}
void setptry(int num){penPtrY = num;}

/* Placeholder if multiple fonts are added*/
void setFontType(int type) {
	switch (type)
	{
		default:
			fontWidth = DEFAULT_FONT_WIDTH;
			fontHeight = DEFAULT_FONT_HEIGHT;
			fontType = &font8x8_basic;
			break;
	}
}

void setFontSize(int size){
	//validacion de parametros se hace en shell
	fontSize = size;
	//si no esta entre esos valores no lo modifico
}

void setBash(){
	penPtrX = 0;
	penPtrY = 0;
}

void printChar(uint8_t keyCode){
    uint8_t letter = keyCode; // getCharacterFromKeyboardHex(keyCode);
	uint8_t goUpALine = 0; //1 si quiero seguir borrando en la linea anterior porque es la misma linea de comando que ocupa mas de una linea
	if(letter == '\n'){ //es un enter
		penPtrY += fontSize*fontHeight;
		penPtrX = 0;
		if(scrollIfFullScreen(penPtrY))
            setBash();
		return;
	}
	else if(letter == '\b'){
		penPtrX-=fontSize*fontWidth;
		if(penPtrX < 0){
			//deberia estar en linea de arriba
			penPtrX = ((getScreenWidth()/(fontSize*fontWidth))-1)*(fontSize * fontWidth) - 1; 
			penPtrY -= fontSize*fontHeight;
		}
		letter = ' ';
	}

	int bitmapOffset = letter;

	if (penPtrX + fontSize*fontWidth > getScreenWidth()) {
		penPtrY += fontSize*fontHeight;
		penPtrX = 0;
		if(scrollIfFullScreen(penPtrY))
            setBash();
	}

	for (int y = 0; y < fontHeight; y++) {
		uint8_t bitmapHex = (*fontType)[bitmapOffset][y]; 
		for (int x = 0; x < fontWidth; x++) {
			if ( (bitmapHex >> x) & 0x01 ) fillrect(penPtrX+x*fontSize, penPtrY+y*fontSize, fontColor, fontSize, fontSize);
			else fillrect(penPtrX+x*fontSize, penPtrY+y*fontSize, 0x0, fontSize, fontSize);
		} 
	}

	if(keyCode!='\b') penPtrX += fontSize*fontWidth;
}

void printString(char * string, uint64_t strLength){
    for(int i = 0; i < strLength && string[i] != '\0'; i++) printChar(string[i]);
}

void printColorString(char * string, uint64_t strLength, uint32_t color){
	fontColor = color;
	printString(string, strLength);
	fontColor = DEFAULT_FONT_COLOR;
}

void printNumber(int value, int base) {
	char buffer[64] = {0};
	printString(buffer, uintToBase(value, buffer, base));
}

//funcion auxiliar que me retorna la resta de dos strings (sus chars)
int stringEquals(char* buf, char* arr){
	int len = strLength(buf); //length del buffer
	if(len != strLength(arr))
		return 0;
	for(int j = 0; j < len; j++){
		if(buf[j] != arr[j])
			return 0;
	}
	return 1;
}

int strLength(char* str){
	int i = 0;
	while(str[i] != 0){
		i++;
	}
	return i;
}

/* -------------------------------- GUARDADO DE KEY INTERRUPTS --------------------------------*/
static char keyBuffer[256] = {0};
static int keyBufferPos = 0;
static int retrievedPos = 0; //va a trackear hasta que posicion del buffer el userland ya pidió char
void saveKey(uint8_t c){
	if(keyBufferPos+1 == 256){
		clearBuffer();
	}
	keyBuffer[keyBufferPos++] = c;
}
void clearBuffer(){
	memset(buffer, 0, 256); //clear buffer
	keyBufferPos = 0;
	retrievedPos = 0;
}
char getChar(){
	if(retrievedPos < keyBufferPos)
		return keyBuffer[retrievedPos++]; 
	return 0;
}
int getBufferPos(){
	return keyBufferPos;
}
char bufferAt(int n){
	if(n > keyBufferPos)
		return 0;
	return keyBuffer[n];
}