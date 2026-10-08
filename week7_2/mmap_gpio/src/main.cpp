#include "MMAPGPIO.hpp"
#include <unistd.h>
#include <iostream>

int main(int argv, char* argc[]) {
  MMAPGPIO gpio("/dev/mem");

  gpio.init();

  gpio.setPinAsInput(BTN_LEFT);
  gpio.setPinAsInput(BTN_RIGHT);
  gpio.setPinAsInput(BTN_UP);
  gpio.setPinAsInput(BTN_DOWN);
  gpio.setPinAsInput(BTN_A);
  gpio.setPinAsInput(BTN_B);

  while(true) {
    std::cout << gpio.getPinLevel(BTN_LEFT) << " "
              << gpio.getPinLevel(BTN_RIGHT) << " "
              << gpio.getPinLevel(BTN_UP) << " "
              << gpio.getPinLevel(BTN_DOWN) << " "
              << gpio.getPinLevel(BTN_A) << " "
              << gpio.getPinLevel(BTN_B) << "\n";
    usleep(10000);
  }

  return 0;
}
