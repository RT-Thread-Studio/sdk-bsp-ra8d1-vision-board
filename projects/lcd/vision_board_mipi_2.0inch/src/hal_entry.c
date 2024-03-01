/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2023-12-17     Rbb666        first version
 */

#include <rtthread.h>
#include <rtdevice.h>

void hal_entry(void)
{
    rt_kprintf("This is rgb-2.0inch lcd demo.\n\n");
    rt_kprintf("======================================================\n");
    rt_kprintf("You can enter the lcd_test command to test the lcd.\n");
    rt_kprintf("======================================================\n");
}
