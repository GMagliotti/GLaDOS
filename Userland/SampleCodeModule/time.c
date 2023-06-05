// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <time.h>

#define GMT_TO_ARG -3

int my_timezone_hour(uint8_t hour,
                     int myRelativeTime) { // converts to time zone
  int hourdec = ((hour >> 4) % 16) * 10 +
                hour % 16; // RTC returns a decimal in hex, doesnt
                           // work well with + and -
  int to_return = hourdec + 24 + myRelativeTime;
  while (to_return >= 24) {
    to_return -= 24;
  }
  return to_return;
}

void print_current_time() { // prints time in Argentina
  timeStructT myStruct = {0, 0, 0};
  call_to_accessRTC(&myStruct);
  printf("Current time: %d:%x:%x\n",
         my_timezone_hour(myStruct.hours, GMT_TO_ARG), myStruct.minutes,
         myStruct.seconds);
}

void time() { print_current_time(); }