#include "MMAPGPIO.hpp"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h> // map() and munmap() defined here
#include <cstdlib>
#include <cstdint>
#include <iostream>

MMAPGPIO::MMAPGPIO(const char* memdev) {
  memDevice = memdev;
}

MMAPGPIO::~MMAPGPIO(){
  // Release pointer to memory
  // block accessed by map()
  munmap(map,MEM_BLK_SIZE);
  close(fd);
}

int MMAPGPIO::init(void) {
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

int MMAPGPIO::setPinAsInput(uint8_t pin) {
  uint32_t* pinCntlRegAddr = gpioBaseAddr
                             + pin*2
                             + 1;
  *pinCntlRegAddr = 0x00;

  uint32_t* padRegAddr = padBaseAddr
                         + 1
                         + pin;
  *padRegAddr = 0xC0;

  return 0;
}

int MMAPGPIO::getPinLevel(uint8_t pin) {
  uint32_t* pinAddr = gpioBaseAddr + pin*2;
  int pinStatus = (*pinAddr >> 27) & 1;

  return pinStatus;
}
