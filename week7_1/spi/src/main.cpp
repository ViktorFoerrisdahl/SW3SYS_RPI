#include <cstring>
#include <cstdlib>
#include <fstream>
#include <fcntl.h>
#include <iostream>
#include <linux/spi/spidev.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include "oledGPIO.hpp"
#include "SSD1306.hpp"


int main(int argv, char* argc[]) {
  // Initialize extra GPIO lines
  initGPIO();

  // Create instance of SSD1306 class
  SSD1306 oled(SPI_DEV);

  // Open SPI device
  if (oled.openSPI() < 0)
    exit(EXIT_FAILURE);

  // Reset OLED screen
  oled.reset();

  // Initialize OLED screen
  oled.init();
  usleep(100000);

  // Clear OLED screen
  oled.clear();
  usleep(100000);

  // Rain animation
  uint8_t pixelState = 0x00;
  std::srand(time(0));
  for (int i=0; i<15; ++i) {
    int x = rand() % (OLED_WIDTH-1);
    for (int y=0; y<OLED_HEIGHT; ++y) {
      oled.drawPixel(x,y,&pixelState);
      usleep(3000);
      oled.clearPixel(x,y,&pixelState);
      usleep(3000);
    }
  }
  usleep(2000000);

  oled.clear();
  usleep(100000);

  // ASCII character set test
  char c = '!';
  uint8_t i = 0;
  for (uint8_t y=0; y<OLED_HEIGHT; y+=8) {
    for (uint8_t x=0; x<OLED_WIDTH-6; x+=6) {
      oled.drawChar(x,y,c+i);
      ++i;
    }
  }
  usleep(2000000);

  oled.clear();
  usleep(100000);

  // String test
  oled.drawString((OLED_WIDTH/2)-(6*3),3*8,"Hello!");
  usleep(2000000);

  oled.clear();
  usleep(100000);

  // Close I2C device
  oled.closeSPI();
  closeGPIO();

  // Exit application
  return EXIT_SUCCESS;
}
