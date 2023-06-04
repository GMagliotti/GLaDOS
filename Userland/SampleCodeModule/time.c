#include <time.h>

#define GMT_TO_ARG -3

int my_timezone_hour(uint8_t hour,
                     int myRelativeTime) { // convierte a la zona horaria
  int hourdec = ((hour >> 4) % 16) * 10 +
                hour % 16; // el RTC devuelve un decimal en formato hexa, que no
                           // interactua intuitivamente con + y -
  int to_return = hourdec + 24 + myRelativeTime;
  while (to_return >= 24) {
    to_return -= 24;
  }
  return to_return;
}

void print_current_time() { // imprime la hora en Argentina
  timeStructT myStruct = {0, 0, 0};
  call_to_accessRTC(&myStruct);
  printf("Current time: %d:%x:%x\n",
         my_timezone_hour(myStruct.hours, GMT_TO_ARG), myStruct.minutes,
         myStruct.seconds);
}

void time() { print_current_time(); }