#include "BMI160.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
  BMI160 imu(SPI_DEV);

  imu.openSPI();

  if (imu.readReg(BMI160_CHIP_ID_REG, 1) < 0) {
    std::cerr << "readReg(): MAXBUFSIZE exceeded \n";
    imu.closeSPI();
    return -1;
  }

  std::cout << "Chip ID: 0x" << std::hex << (int) imu.buffer[0] << "\n\n";

  imu.closeSPI();

  return 0;
}
