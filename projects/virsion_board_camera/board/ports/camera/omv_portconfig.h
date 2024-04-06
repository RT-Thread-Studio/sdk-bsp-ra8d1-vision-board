/*
 * @Descripttion: 
 * @version: 
 * @Author: RCSN
 * @Date: 2022-08-16 17:01:38
 * @LastEditors: fy-tech
 * @LastEditTime: 2022-08-16 17:01:38
 */
/*
 * This file is part of the OpenMV project.
 *
 * Copyright (c) 2013-2021 Ibrahim Abdelkader <iabdalkader@openmv.io>
 * Copyright (c) 2013-2021 Kwabena W. Agyeman <kwagyeman@openmv.io>
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * OpenMV RP2 port abstraction layer.
 */
#ifndef __OMV_PORTCONFIG_H__
#define __OMV_PORTCONFIG_H__

#include "rtthread.h"
#include <rtdevice.h>

// GPIO pull.
#define OMV_GPIO_PULL_UP        PIN_MODE_INPUT_PULLUP
#define OMV_GPIO_PULL_DOWN      PIN_MODE_INPUT_PULLDOWN

// GPIO modes.
#define OMV_GPIO_MODE_INPUT     PIN_MODE_INPUT
#define OMV_GPIO_MODE_OUTPUT    PIN_MODE_OUTPUT

// omv_gpio_t definition
typedef uint32_t omv_gpio_t;
// omv_i2c_t definitions
typedef struct rt_i2c_bus_device *omv_i2c_dev_t;

#endif // __OMV_PORTCONFIG_H__
