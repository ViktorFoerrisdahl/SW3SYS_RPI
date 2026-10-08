#include "MMAPGPIO_shutdown.hpp"
#include <unistd.h>
#include <iostream>

int main(int argv, char *argc[])
{
  MMAPGPIO gpio("/dev/mem");

  gpio.init();

  gpio.setPinAsInput(BTN_UP);

  while (true)
  {
    if (gpio.getPinLevel(BTN_UP) == 0)
    {
      if (gpio.startSafeShutdown() == -1)
      {
        std::cout << "Safe shutdown failed :( \n";
      }
    }

    usleep(10000);
  }

  return 0;
}
