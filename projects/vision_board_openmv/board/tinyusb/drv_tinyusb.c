/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-07     Lishuo       first version
 */
#include <rtdevice.h>
#include <tusb.h>
#include "hal_data.h"

#define USB_CH_SEL_PIN    BSP_IO_PORT_10_PIN_13

int tusb_board_init(void)
{
#ifdef PKG_TINYUSB_FULL_SPEED
    rt_pin_write(USB_CH_SEL_PIN, PIN_LOW);
#else
    rt_pin_write(USB_CH_SEL_PIN, PIN_HIGH);
#endif
    return 0;
}
