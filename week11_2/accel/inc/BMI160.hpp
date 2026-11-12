#ifndef BMI160_HPP
#define BMI160_HPP

#include <cstdint>
#include <linux/spi/spidev.h>

#define BMI160_CMD_REG     0x7E
#define BMI160_CHIP_ID_REG 0x00
#define BMI160_ACCEL_REG   0x12
#define BMI160_GYRO_REG    0x0C
#define BMI160_STATUS_REG  0x1B
#define BMI160_READ_BIT    0x80

#define BMI160_ACCEL_SENS 16384
#define BMI160_GYRO_SENS  16.4

#define SPI_SPEED 1000000 // SPI clock frequency
#define SPI_BITS_PER_WORD 8 // Number of bits in one SPI dataframe

#define MAXBUFSIZE 32 // Max. TX/RX buffer size in bytes

// SPI device file descriptor
#define SPI_DEV "/dev/spidev0.0"

class BMI160 {
  private:
    uint8_t fd;
    const char* device;
    struct spi_ioc_transfer tx[1];

  public:
    uint8_t buffer[MAXBUFSIZE];

    BMI160(const char* spiDevice = SPI_DEV);
    ~BMI160(void);
    int openSPI(void);
    int closeSPI(void);
    int readReg(uint8_t reg, uint8_t nbytes);
    int writeReg(uint8_t reg, uint8_t data);
    int startAccel(void);
    int startGyro(void);
    int isDataAvailable(void);
    int readAccel(void);
    int readGyro(void);
};

#endif
