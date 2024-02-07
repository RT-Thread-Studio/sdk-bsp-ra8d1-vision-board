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
#ifndef __OMV_BOARDCONFIG_H__
#define __OMV_BOARDCONFIG_H__

#include "rtconfig.h"
#include "hal_data.h"

// Architecture info
#define OMV_ARCH_STR            "RTT&RA8" // OMV4 H7 PRO 32768 SDRAM
#define OMV_BOARD_TYPE          "M4"

#define OMV_UNIQUE_ID_ADDR      0x03008190
#define OMV_UNIQUE_ID_SIZE      3 // 3 words
#define OMV_UNIQUE_ID_OFFSET    4 // Bytes offset for multi-word UIDs.

#define OMV_XCLK_MCO            (0U)
#define OMV_XCLK_TIM            (1U)

// Sensor external clock source.
#define OMV_XCLK_SOURCE         (OMV_XCLK_TIM)

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

// FIR Module

// Enable WiFi debug
#define OMV_ENABLE_WIFIDBG      (0)
#define OMV_ENABLE_TUSBDBG      (1)
#define OMV_TUSBDBG_PACKET      (512)

// Enable self-tests on first boot
#define OMV_ENABLE_SELFTEST     (0)

// If buffer size is bigger than this threshold, the quality is reduced.
// This is only used for JPEG images sent to the IDE not normal compression.
#define JPEG_QUALITY_THRESH     (320 * 240 * 2)

// Low and high JPEG QS.
#define JPEG_QUALITY_LOW        50
#define JPEG_QUALITY_HIGH       90

// FB Heap Block Size
#define OMV_UMM_BLOCK_SIZE      16

// Core VBAT for selftests
#define OMV_CORE_VBAT           "3.3"

// Buffer size
#define OMV_FB_OVERLAY_MEMORY   SDRAM
#define OMV_JPEG_BUF_SIZE       (OPENMV_JPEG_BUF_SIZE)  // IDE JPEG buffer (header + data).
#define OMV_HEAP_SIZE           (OPENMV_HEAP_SIZE)      // MicroPython's heap

// Main image sensor I2C bus
#define ISC_I2C_ID              (1)
#define ISC_I2C_SPEED           (OMV_I2C_SPEED_STANDARD)

// LCD config.

// WiFi/NINA config.

// AUDIO config.

// DCMI config.
#define DCMI_POWER_PIN           (DCMI_POWER_PIN_NUM)
#define DCMI_RESET_PIN           (DCMI_RESET_PIN_NUM)

#endif //__OMV_BOARDCONFIG_H__
