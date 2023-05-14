#include <funclib.h>

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

int beginsWith(char * container, char * contains) {
	int len = strLength(contains); //length del buffer
	if(len > strLength(container)) {
		return -1;
	}
	int j = 0;
	for(; j < len; j++){
		if(container[j] != contains[j])
			return -1;
	}
	return j;
}

int checkNumWithBase(char val, int base) {				// verifica si es un numero, si lo es, lo retorna
	switch (base) {
		case (2):
			if (val >= '0' && val <= '1') {
				return val - '0';
			}
			return -1;
			break;
		case (10):
			if (val >= '0' && val <= '9') {
				return val - '0';
			}
			return -1;
			break;
		case (16):
			if (val >= '0' && val <= '9') {
				return val - '0';
			} else if (val >= 'A' && val <= 'F') {
				return val - 'A' + 10;
			}
			return -1;
			break;
		default:
			return -1;
	}
	

}

int toNumWithBase(char * buffer, int from, int base) {
	int toReturn = 0;
	for (int i = from; buffer[i] != 0; i++) {
		int currentNum = checkNumWithBase(buffer[i], base);
		if (currentNum != -1) {
			toReturn *= base;
			toReturn += currentNum;
		} else {
			if (buffer[i] == ' ') {
				return -2;
			}
			return -1;
		}
	}
	return toReturn;
}

void sleeps(int seconds) {
    call_to_sleep(seconds * 1000);
}

void sleep(int seconds) {
	sleeps(seconds);
}

void sleepms(int milliseconds) {
    call_to_sleep(milliseconds);
}


void printMemoryAt(int dir) {

	if (dir < 0) {							// habria que poner un maximo?
		printf("Invalid direction\n");
		return;
	}

	int aligneddir = dir & 0xFFFFFFFFFFFFFFF0; 			// Stack alignment

    printf("Displaying memory at 0x%x (Aligned format)\n", dir);
	for (int i = 0; i < 32; i += 4) {
		uint8_t b1 = call_to_memoryAt(aligneddir+i),
			 	b2 = call_to_memoryAt(aligneddir+i+1),
			 	b3 = call_to_memoryAt(aligneddir+i+2),
			 	b4 = call_to_memoryAt(aligneddir+i+3);
		printf("%x | %x %x %x %x | %c %c %c %c\n", aligneddir+i, 
				b1, b2, b3, b4, &b1, &b2, &b3, &b4);
        sleepms(250);
	}
}