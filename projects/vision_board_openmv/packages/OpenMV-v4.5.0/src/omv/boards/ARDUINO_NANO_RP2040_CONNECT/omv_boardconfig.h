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

// Architecture info
#define OMV_ARCH_STR            "PICO M0" // 33 chars max
#define OMV_BOARD_TYPE          "PICO"

#ifndef LINKER_SCRIPT
extern unsigned char *OMV_UNIQUE_ID_ADDR;
#endif
#define OMV_UNIQUE_ID_SIZE      2 // 2 words

#define OMV_XCLK_MCO            (0U)
#define OMV_XCLK_TIM            (1U)

// Sensor external clock source.
#define OMV_XCLK_SOURCE         (OMV_XCLK_TIM)

// Sensor external clock timer frequency.
// TODO Not actually used right now, frequency is hardcoded.
#define OMV_XCLK_FREQUENCY      (12500000)

// Enable hardware JPEG
#define OMV_HARDWARE_JPEG       (0)

// Enable sensor drivers
#define OMV_ENABLE_OV2640       (0)
#define OMV_ENABLE_OV5640       (0)
#define OMV_ENABLE_OV7670       (1)
#define OMV_ENABLE_OV7690       (0)
#define OMV_ENABLE_OV7725       (0)
#define OMV_ENABLE_OV9650       (0)
#define OMV_ENABLE_MT9V0XX      (0)
#define OMV_ENABLE_LEPTON       (0)
#define OMV_ENABLE_HM01B0       (0)

// Set which OV767x sensor is used
#define OMV_OV7670_VERSION      (70)

// OV7670 clock divider
#define OMV_OV7670_CLKRC        (0x00)

// FIR Module
#define OMV_ENABLE_FIR_MLX90621 (0)
#define OMV_ENABLE_FIR_MLX90640 (0)
#define OMV_ENABLE_FIR_MLX90641 (0)
#define OMV_ENABLE_FIR_AMG8833  (1)
#define OMV_ENABLE_FIR_LEPTON   (0)

// Enable sensor features
#define OMV_ENABLE_OV5640_AF    (0)

// Enable WiFi debug
#define OMV_ENABLE_WIFIDBG      (0)

// Enable self-tests on first boot
#define OMV_ENABLE_SELFTEST     (0)

// If buffer size is bigger than this threshold, the quality is reduced.
// This is only used for JPEG images sent to the IDE not normal compression.
#define JPEG_QUALITY_THRESH     (160*120)

// Low and high JPEG QS.
#define JPEG_QUALITY_LOW        35
#define JPEG_QUALITY_HIGH       60

// FB Heap Block Size
#define OMV_UMM_BLOCK_SIZE      16

// Core VBAT for selftests
#define OMV_CORE_VBAT           "3.3"

// USB IRQn.
#define OMV_USB_IRQN            (USBCTRL_IRQ_IRQn)

// Jump to bootloader function.
#ifndef LINKER_SCRIPT
void pico_reset_to_bootloader(void);
#endif
#define MICROPY_RESET_TO_BOOTLOADER pico_reset_to_bootloader

// Linker script constants (see the linker script template port/x.ld.S).
#define OMV_FB_MEMORY           RAM    // Framebuffer, fb_alloc
#define OMV_MAIN_MEMORY         RAM    // data, bss and heap memory
#define OMV_STACK_MEMORY        RAM    // stack memory

#define OMV_FB_SIZE             (100K) // FB memory
#define OMV_FB_ALLOC_SIZE       (16K)  // minimum fb alloc size
#define OMV_STACK_SIZE          (16K)
#define OMV_HEAP_SIZE           (64 * 1024) // MicroPython's heap
#define OMV_JPEG_BUF_SIZE       (20 * 1024) // IDE JPEG buffer (header + data).

// GP LED
#define LED_PIN                 (6)

// FIR I2C
#define FIR_I2C_ID              (0)
#define FIR_I2C_SCL_PIN         (13)
#define FIR_I2C_SDA_PIN         (12)
#define FIR_I2C_SPEED           (CAMBUS_SPEED_FULL)

// ISC I2C
#define ISC_I2C_ID              (0)
#define ISC_I2C_SCL_PIN         (13)
#define ISC_I2C_SDA_PIN         (12)
#define ISC_I2C_SPEED           (CAMBUS_SPEED_STANDARD)

// I2C0
#define I2C0_ID                 (0)
#define I2C0_SCL_PIN            (13)
#define I2C0_SDA_PIN            (12)
#define I2C0_SPEED              (CAMBUS_SPEED_FULL)

// I2C1
#define I2C1_ID                 (1)
#define I2C1_SCL_PIN            (27)
#define I2C1_SDA_PIN            (26)
#define I2C1_SPEED              (CAMBUS_SPEED_FULL)

// LCD config.
#define LCD_SPI                 (spi0)
#define LCD_CS_PIN              (5)
#define LCD_MOSI_PIN            (7)
#define LCD_SCLK_PIN            (6)
#define LCD_RST_PIN             (4)
#define LCD_RS_PIN              (0)

// WiFi/NINA config.
#define WIFI_SPI                (spi1)
#define WIFI_CS_PIN             (9)
#define WIFI_MOSI_PIN           (11)
#define WIFI_MISO_PIN           (8)
#define WIFI_SCLK_PIN           (14)
#define WIFI_RST_PIN            (3)
#define WIFI_GPIO0_PIN          (2)
#define WIFI_ACK_PIN            (10)

// AUDIO config.
#define PDM_PIO                 (pio1)
#define PDM_SM                  (0)
#define PDM_DMA                 (1)
#define PDM_DMA_IRQ             (DMA_IRQ_1)
#define PDM_DMA_CHANNEL         (0)

#define PDM_CLK_PIN             (23)
#define PDM_DIN_PIN             (22)

// DCMI config.
#define DCMI_PIO                (pio0)
#define DCMI_SM                 (0)
#define DCMI_DMA                (0)
#define DCMI_DMA_IRQ            (DMA_IRQ_0)
#define DCMI_DMA_CHANNEL        (0)

#define DCMI_PWDN_PIN           (0)
#define DCMI_RESET_PIN          (1)

#define DCMI_D0_PIN             (15)
#define DCMI_D1_PIN             (16)
#define DCMI_D2_PIN             (17)
#define DCMI_D3_PIN             (18)
#define DCMI_D4_PIN             (19)
#define DCMI_D5_PIN             (20)
#define DCMI_D6_PIN             (21)
#define DCMI_D7_PIN             (25) // MSB is read separately.

#define DCMI_XCLK_PIN           (28)

#define DCMI_PXCLK_PIN          (29)
#define DCMI_HSYNC_PIN          (27)
#define DCMI_VSYNC_PIN          (26)

#if defined(DCMI_RESET_PIN)
#define DCMI_RESET_LOW()        gpio_put(DCMI_RESET_PIN, 0)
#define DCMI_RESET_HIGH()       gpio_put(DCMI_RESET_PIN, 1)
#else
#define DCMI_RESET_LOW()
#define DCMI_RESET_HIGH()
#endif

#if defined(DCMI_PWDN_PIN)
#define DCMI_PWDN_LOW()        gpio_put(DCMI_PWDN_PIN, 0)
#define DCMI_PWDN_HIGH()       gpio_put(DCMI_PWDN_PIN, 1)
#else
#define DCMI_PWDN_LOW()
#define DCMI_PWDN_HIGH()
#endif

#endif //__OMV_BOARDCONFIG_H__
