#ifndef OLEDGPIO_H
#define OLEDGPIO_H

#include <linux/gpio.h>

// GPIO numbers for DC and RESET
#define DC_PIN 24
#define RESET_PIN 25
#define GPIO_CHIP "/dev/gpiochip0"

extern int gpio_fd;
// Request structure to get handle to GPIO line
extern struct gpiohandle_request req;
// Structure to store values to write to GPIO lines
extern struct gpiohandle_data data;
extern int retval;

int initGPIO(void);
int writeGPIO(int pin, int value);
void closeGPIO(void);

#endif
