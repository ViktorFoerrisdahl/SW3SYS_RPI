#ifndef MMAPLED_HPP
#define MMAPLED_HPP

#include <cstdint>

// LED pins
#define LED0_PIN 26
#define LED1_PIN 20
#define LED2_PIN 21

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

class MMAPLED {
  public:
    MMAPLED(const char* memdev="/dev/mem");
    ~MMAPLED();

    int init(void);
    int setPinAsOutput(uint8_t pin);
    int setPinLevel(uint8_t pin, uint8_t level);

  private:
    const char* memDevice;
    int fd;
    void* map;
    uint32_t* peripheralBaseAddr;
    uint32_t* gpioBaseAddr;
    uint32_t* padBaseAddr;
};

#endif
