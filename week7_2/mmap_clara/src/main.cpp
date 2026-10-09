#include "MMAPGPIO_shutdown.hpp"
#include "MMAPLED_shutdown.hpp"
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

  MMAPLED led("/dev/mem");

  led.init();

  led.setPinAsOutput(LED0_PIN);
  led.setPinAsOutput(LED1_PIN);
  led.setPinAsOutput(LED2_PIN);

  while (true)
  {

    // String test
    oled.drawString(0, 0, "Hej Clara ;))");
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
      led.setPinLevel(LED0_PIN, 1);
      led.setPinLevel(LED1_PIN, 1);
      led.setPinLevel(LED2_PIN, 1);
      usleep(250000);
      led.setPinLevel(LED0_PIN, 0);
      led.setPinLevel(LED1_PIN, 0);
      led.setPinLevel(LED2_PIN, 0);

      sleep(1);
      oled.clear();
      oled.drawString(0, 10, "I will die in 4!");
      led.setPinLevel(LED0_PIN, 1);
      led.setPinLevel(LED1_PIN, 1);
      led.setPinLevel(LED2_PIN, 1);
      usleep(250000);
      led.setPinLevel(LED0_PIN, 0);
      led.setPinLevel(LED1_PIN, 0);
      led.setPinLevel(LED2_PIN, 0);
      sleep(1);
      oled.clear();
      oled.drawString(0, 10, "I will die in 3!");
      led.setPinLevel(LED0_PIN, 1);
      led.setPinLevel(LED1_PIN, 1);
      led.setPinLevel(LED2_PIN, 1);
      usleep(250000);
      led.setPinLevel(LED0_PIN, 0);
      led.setPinLevel(LED1_PIN, 0);
      led.setPinLevel(LED2_PIN, 0);
      sleep(1);
      oled.clear();
      oled.drawString(0, 10, "I will die in 2!");
      led.setPinLevel(LED0_PIN, 1);
      led.setPinLevel(LED1_PIN, 1);
      led.setPinLevel(LED2_PIN, 1);
      usleep(250000);
      led.setPinLevel(LED0_PIN, 0);
      led.setPinLevel(LED1_PIN, 0);
      led.setPinLevel(LED2_PIN, 0);
      sleep(1);
      oled.clear();
      oled.drawString(0, 10, "I will die in 1!");
      led.setPinLevel(LED0_PIN, 1);
      led.setPinLevel(LED1_PIN, 1);
      led.setPinLevel(LED2_PIN, 1);
      usleep(250000);
      led.setPinLevel(LED0_PIN, 0);
      led.setPinLevel(LED1_PIN, 0);
      led.setPinLevel(LED2_PIN, 0);
      sleep(1);
      oled.clear();
      oled.drawString(0, 10, "I am now DEAD :(");
      led.setPinLevel(LED0_PIN, 1);
      led.setPinLevel(LED1_PIN, 1);
      led.setPinLevel(LED2_PIN, 1);
      usleep(1000);
      led.setPinLevel(LED0_PIN, 0);
      led.setPinLevel(LED1_PIN, 0);
      led.setPinLevel(LED2_PIN, 0);
      usleep(1000);
      led.setPinLevel(LED0_PIN, 1);
      led.setPinLevel(LED1_PIN, 1);
      led.setPinLevel(LED2_PIN, 1);
      usleep(1000);
      led.setPinLevel(LED0_PIN, 0);
      led.setPinLevel(LED1_PIN, 0);
      led.setPinLevel(LED2_PIN, 0);
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
