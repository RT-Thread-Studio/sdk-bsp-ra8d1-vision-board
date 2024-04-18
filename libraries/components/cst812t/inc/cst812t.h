/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-01     Yuanjie      Add cst812t Touch Device
 */

#ifndef __CST812_H__
#define __CST812_H__

#include <rtthread.h>
#include <rtdevice.h>

#ifdef PKG_USING_CST812T

#ifndef TOUCH_IRQ_PIN
#define TOUCH_IRQ_PIN (rt_pin_get("p010"))
#endif
#ifndef TOUCH_DEVICE_NAME
#define TOUCH_DEVICE_NAME "sci3i"
#endif
#ifndef TOUCH_RST_PIN
#define TOUCH_RST_PIN (rt_pin_get("p000"))
#endif

#define TOUCH_SLAVE_ADDRESS (0x15)
// resolution pre-defined by firmware in touch IC.
#define TOUCH_X_RESOLUTION  480
#define TOUCH_Y_RESOLUTION  360

/*register address*/
#define CST8XX_REG_GESTUREID    0x01
#define CST8XX_REG_FINGERNUM    0x02
#define CST8XX_REG_XPOSH        0x03
#define CST8XX_REG_XPOSL        0x04
#define CST8XX_REG_YPOSH        0x05
#define CST8XX_REG_YPOSL        0x06
#define CST8XX_REG_SLIDE_ID     0x20
#define CST8XX_REG_SLIDE_POS    0x21
#define CST8XX_REG_FWVERSION    0xA9
#define CST8XX_REG_VERSION      0xFF

/* 0x01 CST8XX_REG_GESTUREID */
enum cst_gesture
{
    SWAP_NONE = 0x00,
    SWAP_UP = 0x01,
    SWAP_DOWN = 0x02,
    SWAP_LEFT = 0x03,
    SWAP_RIGHT = 0x04,
    SINGLE_TAP = 0x05,
    DOUBLE_TAP = 0x0B,
};

#define TOUCH_EVENT_UP      (0x01)
#define TOUCH_EVENT_DOWN    (0x02)
#define TOUCH_EVENT_MOVE    (0x03)
#define TOUCH_EVENT_NONE    (0x80)

/* 0x02 CST8XX_REG_FINGERNUM */
#define CST8XX_REG_FINGERNUM_NONE 0x00
#define CST8XX_REG_FINGERNUM_ONE 0x01

void cst8xx_reset(uint16_t ms);
rt_size_t cst816_i2c_write(uint8_t deive_addr, uint16_t reg, uint8_t *p_data, uint16_t len);
rt_size_t cst816_i2c_read(uint8_t deive_addr, const uint16_t reg, uint8_t *p_data, uint16_t len);

rt_err_t cst8xx_probe(void);
int rt_hw_cst8xx_init(const char *name, struct rt_touch_config *cfg);

#endif

#endif /* PKG_USING_CST812T */
