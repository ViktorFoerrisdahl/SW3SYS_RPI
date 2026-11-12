#include "BMI160.hpp"
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>

// Constructor
BMI160::BMI160(const char* spiDevice) {
  device = spiDevice;

  memset(&tx,0,sizeof(tx));
  memset(&buffer,0,sizeof(buffer));
}

// Destructor
BMI160::~BMI160() {
  closeSPI();
}

// Initialize SPI bus
int BMI160::openSPI(void) {
  // Open SPI device
  fd = open(device, O_RDWR);
  if (fd < 0) { perror("open()"); return -1; }

  // Define SPI protocol settings
  uint8_t mode = SPI_MODE_3;
  uint8_t bits = SPI_BITS_PER_WORD;
  uint32_t speed = SPI_SPEED;

  // Write settings to kernel SPI interface
  ioctl(fd, SPI_IOC_RD_MODE, &mode);
  ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
  ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
  ioctl(fd, SPI_IOC_WR_MODE, &mode);
  ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
  ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

  return 0;
}

// Close SPI bus
int BMI160::closeSPI(void) {
  close(fd);
  return 0;
}

// Read from BMI160 register
int BMI160::readReg(uint8_t reg, uint8_t nbytes) {
  if (nbytes > MAXBUFSIZE) { return -1; }

  uint8_t buf[nbytes+1] = {0}; // Internal buffer with all array elements initialized to 0
  buf[0] = reg | (1<<7); // Address of BMI160 register to read from, with READ bit set to 1

  // Initialize all SPI data transfer settings
  tx[0].tx_buf = (__u64)buf;
  tx[0].rx_buf = (__u64)buf;
  tx[0].len = (__u32)nbytes + 1;
  tx[0].cs_change = 0;
  tx[0].delay_usecs = 0;
  tx[0].speed_hz = SPI_SPEED;
  tx[0].bits_per_word = SPI_BITS_PER_WORD;

  // Start SPI data transfer
  if (ioctl(fd, SPI_IOC_MESSAGE(1), &tx[0]) < 0) { perror("ioctl()"); close(fd); return -1; }

  // Copy all data to external buffer
  for (int i = 0; i < nbytes; ++i) { buffer[i] = buf[i+1]; }
  return 0;
}

// Write to BMI160 register
int BMI160::writeReg(uint8_t reg, uint8_t data) {
  uint8_t buf[2] = {0}; // Internal buffer with all array elements initialized to 0
  buf[0] = reg & ~(1<<7); // Address of BMI160 register to write to, with READ bit set to 0
  buf[1] = data; // Data byte to write to BMI160 register

  // Initialize all SPI data transfer settings
  tx[0].tx_buf = (__u64)buf;
  tx[0].rx_buf = (__u64)buf;
  tx[0].len = (__u32)sizeof(buf);
  tx[0].cs_change = 0;
  tx[0].delay_usecs = 0;
  tx[0].speed_hz = SPI_SPEED;
  tx[0].bits_per_word = SPI_BITS_PER_WORD;

  // Start SPI data transfer
  if (ioctl(fd, SPI_IOC_MESSAGE(1), &tx[0]) < 0) { perror("ioctl()"); close(fd); return -1; }
  return 0;
}

// Turn on accel
int BMI160::startAccel(void) {
  memset(&tx,0,sizeof(tx));
  memset(&buffer,0,sizeof(buffer));

  if (writeReg(BMI160_CMD_REG, 0x11) < 0) {
    std::cerr << "writeReg() failed\n";
    close(fd);
    return -1;
  }

  // Sleep to allow gyro to start up
  usleep(10000);

  return 0;
}

// Turn on gyro
int BMI160::startGyro(void) {
  memset(&tx,0,sizeof(tx));
  memset(&buffer,0,sizeof(buffer));

  if (writeReg(BMI160_CMD_REG, 0x15) < 0) {
    std::cerr << "writeReg() failed\n";
    close(fd);
    return -1;
  }
  // Sleep to allow gyro to start up
  usleep(100000);

  return 0;
}

// Get data status
int BMI160::isDataAvailable(void) {
  memset(&tx,0,sizeof(tx));
  memset(&buffer,0,sizeof(buffer));

  if (readReg(BMI160_STATUS_REG, 1) < 0) {
    std::cerr << "readReg() failed\n";
    close(fd);
    return -1;
  }
  return (int) ((buffer[0] & 0xC0) == 0xC0);
}

// Read accelerometer data
int BMI160::readAccel(void) {
  memset(&tx,0,sizeof(tx));
  memset(&buffer,0,sizeof(buffer));

  if (readReg(BMI160_ACCEL_REG, 6) < 0) {
    std::cerr << "readReg() failed\n";
    close(fd);
    return -1;
  }

  return 0;
}

// Read gyro data
int BMI160::readGyro(void) {
  memset(&tx,0,sizeof(tx));
  memset(&buffer,0,sizeof(buffer));

  if (readReg(BMI160_GYRO_REG, 6) < 0) {
    std::cerr << "readReg() failed\n";
    close(fd);
    return -1;
  }

  return 0;
}
