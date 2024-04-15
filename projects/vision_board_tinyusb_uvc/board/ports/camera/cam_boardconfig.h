/*
 * This file is part of the OpenMV project.
 *
 * Copyright (c) 2013-2021 Ibrahim Abdelkader <iabdalkader@openmv.io>
 * Copyright (c) 2013-2021 Kwabena W. Agyeman <kwagyeman@openmv.io>
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * Board configuration and pin definitions.
 */
#ifndef __CAM_BOARDCONFIG_H__
#define __CAM_BOARDCONFIG_H__

#include "rtconfig.h"
#include "hal_data.h"

// Sensor external clock timer frequency.
// TODO Not actually used right now, frequency is hardcoded.
#define OMV_XCLK_FREQUENCY      (24000000)

// Sensor IIC Bus
#define OMV_CAM_BUS_NAME        SENSOR_BUS_NAME
#define OMV_CAM_BUS_SCL_PIN     BSP_I2C1_SCL_PIN
#define OMV_CAM_BUS_SDA_PIN     BSP_I2C1_SDA_PIN

// Sensor PLL register value.
#define OMV_OV7725_PLL_CONFIG   (0x01)  // bypass

// Sensor Banding Filter Value
#define OMV_OV7725_BANDING      (0x7F)

// Enable sensor features
#define OMV_ENABLE_OV5640_AF    (1)
#define OMV_OV5640_XCLK_FREQ    (24000000)
#define OMV_OV5640_PLL_CTRL2    (0x64)
#define OMV_OV5640_PLL_CTRL3    (0x13)
#define OMV_OV5640_REV_Y_CHECK  (0)
#define OMV_OV5640_REV_Y_FREQ   (25000000)
#define OMV_OV5640_REV_Y_CTRL2  (0x54)
#define OMV_OV5640_REV_Y_CTRL3  (0x13)

// Enable hardware JPEG
#define OMV_HARDWARE_JPEG       (0)

// Enable sensor drivers
#define OMV_ENABLE_OV2640       (SENSOR_OV2640)
#define OMV_ENABLE_OV5640       (SENSOR_OV5640)
#define OMV_ENABLE_OV7670       (SENSOR_OV7670)
#define OMV_ENABLE_OV7690       (SENSOR_OV7690)
#define OMV_ENABLE_OV7725       (SENSOR_OV7725)
#define OMV_ENABLE_OV9650       (SENSOR_OV9650)
#define OMV_ENABLE_MT9V0XX      (SENSOR_MT9V034)
#define OMV_ENABLE_GC0328       (SENSOR_GC0328)

// Set which OV767x sensor is used
#define OMV_OV7670_VERSION      (70)

// OV7670 clock divider
#define OMV_OV7670_CLKRC        (0x00)

// Main image sensor I2C bus
#define ISC_I2C_ID              (1)
#define ISC_I2C_SPEED           (OMV_I2C_SPEED_STANDARD)

// DCMI config.
#define DCMI_POWER_PIN           (DCMI_POWER_PIN_NUM)
#define DCMI_RESET_PIN           (DCMI_RESET_PIN_NUM)

#endif //__OMV_BOARDCONFIG_H__
