/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-20     tfx2001      first version
 */

#include <rtthread.h>
#define  DBG_TAG  "TinyUSB"
#define  DBG_LVL  DBG_INFO
#include <rtdbg.h>
#include <tusb.h>

extern int tusb_board_init(void);

#ifndef RT_USING_HEAP
/* if there is not enable heap, we should use static thread and stack. */
static rt_uint8_t tusb_stack[PKG_TINYUSB_STACK_SIZE];
static struct rt_thread tusb_thread;
#endif /* RT_USING_HEAP */

static void tusb_thread_entry(void *parameter)
{
    (void) parameter;
    while (1)
    {
        tud_task();
    }
}

static int init_tinyusb(void)
{
    rt_thread_t tid;

    tusb_board_init();
    tusb_init();

#ifdef RT_USING_HEAP
    tid = rt_thread_create("tusb", tusb_thread_entry, RT_NULL,
                           PKG_TINYUSB_STACK_SIZE,
                           PKG_TINYUSB_THREAD_PRIORITY, 10);
    if (tid == RT_NULL)
#else
    rt_err_t result;

    tid = &tusb_thread;
    result = rt_thread_init(tid, "tusb", tusb_thread_entry, RT_NULL,
                            tusb_stack, sizeof(tusb_stack), 4, 10);
    if (result != RT_EOK)
#endif /* RT_USING_HEAP */
    {
        LOG_E("Fail to create TinyUSB thread");
        return -1;
    }

    rt_thread_startup(tid);

    return 0;
}
INIT_DEVICE_EXPORT(init_tinyusb);
