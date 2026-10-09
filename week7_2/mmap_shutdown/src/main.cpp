#include "MMAPGPIO_shutdown.hpp"
#include "SSD1306.hpp"
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(int argv, char *argc[])
{

  // Create instance of SSD1306 class
  SSD1306 oled(I2C_DEV, SSD1306_ADDR);

  // Open I2C device
  if (oled.openI2C() < 0)
    exit(EXIT_FAILURE);

  // Initialize OLED screen
  oled.init();
  usleep(100000);

  // Clear OLED screen
  oled.clear();
  usleep(100000);

  MMAPGPIO gpio("/dev/mem");

  gpio.init();

  gpio.setPinAsInput(BTN_UP);

  while (true)
  {

    // String test
    oled.drawString(0, 10, "Press 'BTN-UP'");
    oled.drawString(0, 20, "for safe shutdown");

    if (gpio.getPinLevel(BTN_UP) == 0)
    {
      oled.clear();
      oled.drawString(0, 0, "Button pressed!");
      oled.drawString(0, 15, "Self-destruct");
      oled.drawString(0, 25, "initiated! RUN!");
      sleep(5);
      oled.clear();
      oled.drawString(0, 10, "I will die in 5!");
      sleep(1);
      oled.clear();
      oled.drawString(0, 10, "I will die in 4!");
      sleep(1);
      oled.clear();
      oled.drawString(0, 10, "I will die in 3!");
      sleep(1);
      oled.clear();
      oled.drawString(0, 10, "I will die in 2!");
      sleep(1);
      oled.clear();
      oled.drawString(0, 10, "I will die in 1!");
      sleep(1);
      oled.clear();
      oled.drawString(0, 10, "I am now DEAD :(");
      sleep(2);
      oled.clear();
      if (gpio.startSafeShutdown() == -1)
      {
        std::cout << "Safe shutdown failed :( \n";
      }
    }
    usleep(10000);
  }

  return 0;
}
