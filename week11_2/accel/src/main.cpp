#include "BMI160.hpp"
#include <unistd.h>
#include <iomanip>
#include <iostream>

int main(int argc, char* argv[]) {
  BMI160 imu(SPI_DEV);

  imu.openSPI();

  imu.startAccel();
  imu.startGyro();

  while ( imu.isDataAvailable() ) {
    imu.readAccel();
    double ax = ((double)((int8_t)imu.buffer[1] << 8 | (int8_t)imu.buffer[0]) / BMI160_ACCEL_SENS) * 9.81;
    double ay = ((double)((int8_t)imu.buffer[3] << 8 | (int8_t)imu.buffer[2]) / BMI160_ACCEL_SENS) * 9.81;
    double az = ((double)((int8_t)imu.buffer[5] << 8 | (int8_t)imu.buffer[4]) / BMI160_ACCEL_SENS) * 9.81;

    imu.readGyro();
    double gx = ((double)((int8_t)imu.buffer[1] << 8 | (int8_t)imu.buffer[0]) / BMI160_GYRO_SENS);
    double gy = ((double)((int8_t)imu.buffer[3] << 8 | (int8_t)imu.buffer[2]) / BMI160_GYRO_SENS);
    double gz = ((double)((int8_t)imu.buffer[5] << 8 | (int8_t)imu.buffer[4]) / BMI160_GYRO_SENS);

    std::cout << "ax: " << std::fixed << std::setprecision(2) << ax << "\t";
    std::cout << "ay: " << std::fixed << std::setprecision(2) << ay << "\t";
    std::cout << "az: " << std::fixed << std::setprecision(2) << az << "\t";
    std::cout << "gx: " << std::fixed << std::setprecision(2) << gx << "\t";
    std::cout << "gy: " << std::fixed << std::setprecision(2) << gy << "\t";
    std::cout << "gz: " << std::fixed << std::setprecision(2) << gz << "\n";

    usleep(100000);
  }

  imu.closeSPI();

  return 0;
}
