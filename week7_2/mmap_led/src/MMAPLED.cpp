#include "MMAPLED.hpp"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h> // map() and munmap() defined here
#include <cstdlib>
#include <cstdint>
#include <iostream>

MMAPLED::MMAPLED(const char* memdev) {
  memDevice = memdev;
}

MMAPLED::~MMAPLED(){
  // Release pointer to memory
  // block accessed by map()
  munmap(map,MEM_BLK_SIZE);
  close(fd);
}

int MMAPLED::init(void) {
  // Open /dev/mem
  fd = open(memDevice, O_RDWR | O_SYNC);
  if (fd < 0) { perror("open()"); return -1; }

  // Get a pointer to memory block
  map = mmap(nullptr,
             MEM_BLK_SIZE,
             PROT_READ | PROT_WRITE,
             MAP_SHARED,
             fd,
             PERIPHERAL_BASE_ADDR);

  if (map == MAP_FAILED) {
    perror("mmap failed"); close(fd); return -1; }

  // Pointer to base address for peripheral devices
  peripheralBaseAddr = (uint32_t*) map;
  // Pointer to address for IO_BANK0 block
  gpioBaseAddr = peripheralBaseAddr
                 + IO_BANK0_ADDR_OFFSET / 4;
  // Pointer to address for PADS_BANK0 block
  padBaseAddr = peripheralBaseAddr
                + PADS_BANK0_ADDR_OFFSET / 4;

  return 0;
}

int MMAPLED::setPinAsOutput(uint8_t pin) {
  uint32_t* pinCntlRegAddr = gpioBaseAddr
                             + pin*2
                             + 1;
  *pinCntlRegAddr = (0x03<<14) + (0x04<<5) + 0x1F;

  uint32_t* padRegAddr = padBaseAddr
                         + 1
                         + pin;
  *padRegAddr = 0x00;

  return 0;
}

int MMAPLED::setPinLevel(uint8_t pin, uint8_t level) {
  uint32_t* pinAddr = gpioBaseAddr
                      + pin*2
                      + 1;

  uint32_t status = *pinAddr;

  if (level == 1)
    status = status | (uint32_t)(3<<12);
  else if (level == 0)
    status = status & (uint32_t)(~(3<<12)) | (uint32_t)(2<<12);

  *pinAddr = status;

  return 0;
}
