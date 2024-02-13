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
#include "doom.h"

#define LED_PIN    BSP_IO_PORT_01_PIN_02 /* Onboard LED pins */

static int Doom_run_start(void);

void hal_entry(void)
{
    rt_kprintf("Doom&RT-Thread!\n");
    /* Start-up doom */
    Doom_run_start();
}

static void doom_thread_entry(void *param)
{
    extern void D_DoomMain(void);
    D_DoomMain();
}

static int Doom_run_start(void)
{
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

    rt_thread_t tid;
    tid = rt_thread_create("doom", doom_thread_entry, RT_NULL, 4096, 22, 20);

    RT_ASSERT(tid != RT_NULL);
    rt_thread_startup(tid);

    return 0;
}
