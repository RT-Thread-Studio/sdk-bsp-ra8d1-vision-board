/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2023-12-17     Rbb666        first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "hal_data.h"

#include "nes.h"

#define DRV_DEBUG
#define LOG_TAG             "main"
#include <drv_log.h>

#define LED_PIN    BSP_IO_PORT_01_PIN_02 /* Onboard LED pins */

void hal_entry(void)
{
    rt_kprintf("===================================\n");
    rt_kprintf("This is mipi-2.0inch nes demo\n");
    rt_kprintf("===================================\n");

    rt_device_t device;
    device = rt_device_find("sd");
    while (device == NULL)
    {
        device = rt_device_find("sd");
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(200);
        rt_pin_write(LED_PIN, PIN_LOW);
        rt_thread_mdelay(200);
    }

    nes_t *nes = nes_load_file("nes/SuperMary.nes");
    if (!nes)
    {
        LOG_E("nes load error");
    }
    nes_run(nes);
    nes_unload_file(nes);
}
