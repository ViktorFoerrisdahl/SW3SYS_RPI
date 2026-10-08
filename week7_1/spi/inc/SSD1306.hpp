#ifndef SSD1306_H
#define SSD1306_H

#include <cstdint>
#include <string>
#include <sys/types.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

//  SSD1306 Command Set

// Fundamental Commands
#define SSD1306_SET_CONTRAST_CONTROL 0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME 0xA4
#define SSD1306_DISPLAY_ALL_ON 0xA5
#define SSD1306_NORMAL_DISPLAY 0xA6
#define SSD1306_INVERT_DISPLAY 0xA7
#define SSD1306_DISPLAY_OFF 0xAE
#define SSD1306_DISPLAY_ON 0xAF
#define SSD1306_NOP 0xE3

// Scrolling Commands
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3

// Addressing Setting Commands
#define SSD1306_SET_LOWER_COLUMN 0x00
#define SSD1306_SET_HIGHER_COLUMN 0x10
#define SSD1306_MEMORY_ADDR_MODE 0x20
#define SSD1306_SET_COLUMN_ADDR 0x21
#define SSD1306_SET_PAGE_ADDR 0x22

// Hardware Configuration Commands
#define SSD1306_SET_START_LINE 0x40
#define SSD1306_SET_SEGMENT_REMAP 0xA0
#define SSD1306_SET_MULTIPLEX_RATIO 0xA8
#define SSD1306_COM_SCAN_DIR_INC 0xC0
#define SSD1306_COM_SCAN_DIR_DEC 0xC8
#define SSD1306_SET_DISPLAY_OFFSET 0xD3
#define SSD1306_SET_COM_PINS 0xDA
#define SSD1306_CHARGE_PUMP 0x8D

// Timing & Driving Scheme Setting Commands
#define SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO 0xD5
#define SSD1306_SET_PRECHARGE_PERIOD 0xD9
#define SSD1306_SET_VCOM_DESELECT 0xDB

// SPI related
#define SSD1306_COMMAND 0x00
#define SSD1306_DATA 0xC0
#define SSD1306_DATA_CONTINUE 0x40

// SPI device file descriptor
#define SPI_DEV "/dev/spidev0.0"
// SPI speed in Hz
#define SPI_SPEED 8000000

class SSD1306 {
  public:
    SSD1306(const char* spiDevice = SPI_DEV);
    ~SSD1306();

    int openSPI();
    void closeSPI();
    int init();
    void fill();
    void clear();
    void reset();
    void setCursor(uint8_t row, uint8_t col);
    int drawPixel(int x, int y, uint8_t* currPixel);
    int clearPixel(int x, int y, uint8_t* currPixel);
    int drawChar(int x, int y, char c);
    int drawString(int x, int y, const std::string& str);

  private:
    uint8_t fd;
    const char* device;

    int sendCommand(uint8_t cmd);
    int sendData(uint8_t data);
    int sendNData(uint8_t *data, size_t n);
};

#endif
