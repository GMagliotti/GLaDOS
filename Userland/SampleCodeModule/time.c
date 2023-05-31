#include <time.h>

#define GMT_TO_ARG - 3

int myTimezoneHour(uint8_t hour, int myRelativeTime) {			// convierte a la zona horaria
	int hourdec = ((hour >> 4) % 16) * 10 + hour % 16;			// el RTC devuelve un decimal en formato hexa, que no interactua intuitivamente con + y -
	int toReturn = hourdec + 24 + myRelativeTime;
		while (toReturn >= 24) {
			toReturn -= 24;
		}
		return toReturn;		
	}

void printCurrentTime() {										// imprime la hora en Argentina
	timeStructT myStruct = {0,0,0};
	call_to_accessRTC(&myStruct);
	printf("Current time: %d:%x:%x\n", myTimezoneHour(myStruct.hours, GMT_TO_ARG), myStruct.minutes, myStruct.seconds);
}

void time() {
	printCurrentTime();
}