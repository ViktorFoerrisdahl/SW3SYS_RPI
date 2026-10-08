#ifndef MMAPGPIO_HPP
#define MMAPGPIO_HPP

#include <cstdint>

// Button pins
#define BTN_LEFT   5
#define BTN_RIGHT  6
#define BTN_UP     12
#define BTN_DOWN   16
#define BTN_A      27
#define BTN_B      22

// Base memory address for peripherals
#define PERIPHERAL_BASE_ADDR 0x1F00000000

// Memory address offsets (from base address)
#define IO_BANK0_ADDR_OFFSET 0xD0000 // IO_BANK0 block
#define RIO0_ADDR_OFFSET 0xE0000  // SYS_RIO0 block
#define PADS_BANK0_ADDR_OFFSET 0xF0000  // PADS_BANK0 block

// Full GPIO address range is 1F00000000 to 0x1F00100000
// Total GPIO memory = 0x1F00100000 - 0x1F00000000
//                   = 0x100000
//                   = 1,048,576 bits
//                   = 128 kB
//
// BLOCK_SIZE = N kB * bytes/kB * bits/byte
#define MEM_BLK_SIZE (128 * 1024 * 8)

class MMAPGPIO {
  public:
    MMAPGPIO(const char* memdev="/dev/mem");
    ~MMAPGPIO();

    int init(void);
    int setPinAsInput(uint8_t pin);
    int getPinLevel(uint8_t pin);

  private:
    const char* memDevice;
    int fd;
    void* map;
    uint32_t* peripheralBaseAddr;
    uint32_t* gpioBaseAddr;
    uint32_t* padBaseAddr;
};

#endif
