#include <rtconfig.h>

// MCU config

// module config

// peripheral config

// board config

// UART

// I2C

// SPI

// PWM

// DAC

// Switch

// LEDs
#define MICROPY_HW_LED1             (0x0A07)    // R
#define MICROPY_HW_LED2             (0x0106)    // G
#define MICROPY_HW_LED3             (0x0102)    // B
#define MICROPY_HW_LED_ON(pin)      mp_hal_pin_high(pin)
#define MICROPY_HW_LED_OFF(pin)     mp_hal_pin_low(pin)
#define MICROPY_HW_LED_TOGGLE(pin)  mp_hal_pin_toggle(pin)
