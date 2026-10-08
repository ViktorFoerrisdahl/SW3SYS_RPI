#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "oledGPIO.hpp"

int gpio_fd = -1;
// Request structure to get handle to GPIO line
struct gpiohandle_request req;
// Structure to store values to write to GPIO lines
struct gpiohandle_data data;
int retval = -1;

// Initialize GPIO access
int initGPIO(void) {
  // Open gpiochip0 as read-only device
  gpio_fd = open(GPIO_CHIP, O_RDWR);

  if (gpio_fd < 0) {
    perror("open");
    return EXIT_FAILURE;
  }

  // Populate structure to be safe
  memset(&req, 0, sizeof(req));

  // Specify GPIO lines we want to access
  req.lineoffsets[0] = DC_PIN;
  req.lineoffsets[1] = RESET_PIN;
  // Access all lines as inputs
  req.flags = GPIOHANDLE_REQUEST_OUTPUT;
  // Number of lines we want to access
  req.lines = 2;
  // Set a name to be shown when gpioinfo is called
  strcpy(req.consumer_label, "OLED SPI GPIO write example");

  // Use ioctl to get GPIO line handle from kernel
  retval = ioctl(gpio_fd,
                 GPIO_GET_LINEHANDLE_IOCTL,
                 &req);
  if (retval < 0) {
    perror("ioctl");
    close(gpio_fd);
    return EXIT_FAILURE;
  }

  return 1;
}

// Write to DC and RESET pins
int writeGPIO(int pin, int value) {
  // Save pin states
  retval = ioctl(req.fd,
                 GPIOHANDLE_GET_LINE_VALUES_IOCTL,
                 &data);
  if (retval < 0) {
    perror("ioctl");
    close(req.fd);
    close(gpio_fd);
    return EXIT_FAILURE;
  }

  // Write pin states
  if (pin == DC_PIN)
    data.values[0] = value;
  if (pin == RESET_PIN)
    data.values[1] = value;

  retval = ioctl(req.fd,
                 GPIOHANDLE_SET_LINE_VALUES_IOCTL,
                 &data);
  if (retval < 0) {
    perror("ioctl");
    close(req.fd);
    close(gpio_fd);
    return EXIT_FAILURE;
  }

  return 1;
}

// Close GPIO access
void closeGPIO(void) {
  close(req.fd);
  close(gpio_fd);
}
