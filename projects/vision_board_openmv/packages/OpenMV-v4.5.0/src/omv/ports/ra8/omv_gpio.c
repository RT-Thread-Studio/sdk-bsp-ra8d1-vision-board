/*
 * This file is part of the OpenMV project.
 *
 * Copyright (c) 2023 Ibrahim Abdelkader <iabdalkader@openmv.io>
 * Copyright (c) 2023 Kwabena W. Agyeman <kwagyeman@openmv.io>
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * GPIO port for rp2.
 */
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <rtdevice.h>
#include "omv_boardconfig.h"
#include "omv_gpio.h"

void omv_gpio_config(omv_gpio_t pin, uint32_t mode, uint32_t pull, uint32_t speed, uint32_t af) {
    switch(mode) {
    case OMV_GPIO_MODE_INPUT: {
         mode = PIN_MODE_INPUT;
        break;
    }
    case OMV_GPIO_PULL_UP : {
        mode = PIN_MODE_INPUT_PULLUP;
        break;
    }
    case OMV_GPIO_PULL_DOWN : {
        mode = PIN_MODE_INPUT_PULLDOWN;
        break;
    }
    case OMV_GPIO_MODE_OUTPUT :
        mode = PIN_MODE_OUTPUT;
        break;
    }
    rt_pin_mode(pin, mode);
}

void omv_gpio_deinit(omv_gpio_t pin) {
    
}

bool omv_gpio_read(omv_gpio_t pin) {
    return rt_pin_read(pin);
}

void omv_gpio_write(omv_gpio_t pin, bool value) {
    rt_pin_write(pin, value);
}
