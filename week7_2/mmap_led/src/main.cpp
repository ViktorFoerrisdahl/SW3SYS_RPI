#include "MMAPLED.hpp"
#include <unistd.h>
#include <iostream>

int main(int argv, char* argc[]) {
  MMAPLED led("/dev/mem");

  led.init();

  led.setPinAsOutput(LED0_PIN);
  led.setPinAsOutput(LED1_PIN);
  led.setPinAsOutput(LED2_PIN);

  while(true) {
    led.setPinLevel(LED0_PIN,1);
    led.setPinLevel(LED1_PIN,1);
    led.setPinLevel(LED2_PIN,1);
    usleep(250000);
    led.setPinLevel(LED0_PIN,0);
    led.setPinLevel(LED1_PIN,0);
    led.setPinLevel(LED2_PIN,0);
    usleep(250000);
  }

  return 0;
}
