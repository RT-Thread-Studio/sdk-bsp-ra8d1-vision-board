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
#include <ra8/lcd_config.h>

#include "hal_data.h"
#include "gt9147.h"

#define RST_PIN   "p000"
#define INT_PIN   "p010"

static rt_device_t touch_dev = RT_NULL;
static struct rt_touch_data *read_data = RT_NULL;
static rt_thread_t gt9147_thread = RT_NULL;
static rt_sem_t gt9147_sem = RT_NULL;

rt_err_t gt911_register(void);

void hal_entry(void)
{
    rt_kprintf("This is rgb-4.3inch lcd demo.\n\n");
    rt_kprintf("======================================================\n");
    rt_kprintf("You can enter the lcd_test command to test the lcd.\n\n");
    rt_kprintf("You can enter the gt911_register command to test the touch.\n\n");
    rt_kprintf("======================================================\n");
}

static void gt911_entry(void *parameter)
{
    while (1)
    {
        rt_sem_take(gt9147_sem, RT_WAITING_FOREVER);

        if (rt_device_read(touch_dev, 0, read_data, 1))
        {
            rt_kprintf("%d %d %d %d %d\n", read_data->track_id,
                       read_data->x_coordinate,
                       read_data->y_coordinate,
                       read_data->timestamp,
                       read_data->width);
        }
        rt_device_control(touch_dev, RT_TOUCH_CTRL_ENABLE_INT, RT_NULL);
    }
}

static rt_err_t rx_callback(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(gt9147_sem);
    rt_device_control(dev, RT_TOUCH_CTRL_DISABLE_INT, RT_NULL);
    return 0;
}

rt_err_t gt911_probe(rt_uint16_t x, rt_uint16_t y)
{
    void *id;

    touch_dev = rt_device_find("gt9147");
    if (touch_dev == RT_NULL)
    {
        rt_kprintf("can't find device gt911\n");
        return -1;
    }

    if (rt_device_open(touch_dev, RT_DEVICE_FLAG_INT_RX) != RT_EOK)
    {
        rt_kprintf("open device failed!");
        return -1;
    }

    id = rt_malloc(sizeof(rt_uint8_t) * 8);
    rt_device_control(touch_dev, RT_TOUCH_CTRL_GET_ID, id);
    rt_uint8_t *read_id = (rt_uint8_t *)id;
    rt_kprintf("id = GT%d%d%d \n", read_id[0] - '0', read_id[1] - '0', read_id[2] - '0');

    rt_device_control(touch_dev, RT_TOUCH_CTRL_SET_X_RANGE, &x);  /* if possible you can set your x y coordinate*/
    rt_device_control(touch_dev, RT_TOUCH_CTRL_SET_Y_RANGE, &y);
    rt_device_control(touch_dev, RT_TOUCH_CTRL_GET_INFO, id);
    rt_kprintf("range_x = %d \n", (*(struct rt_touch_info *)id).range_x);
    rt_kprintf("range_y = %d \n", (*(struct rt_touch_info *)id).range_y);
    rt_kprintf("point_num = %d \n", (*(struct rt_touch_info *)id).point_num);
    rt_free(id);

    rt_device_set_rx_indicate(touch_dev, rx_callback);

    read_data = (struct rt_touch_data *)rt_calloc(1, sizeof(struct rt_touch_data));
    if (!read_data)
    {
        return -RT_ENOMEM;
    }

    gt9147_sem = rt_sem_create("dsem", 0, RT_IPC_FLAG_FIFO);

    if (gt9147_sem == RT_NULL)
    {
        rt_kprintf("create dynamic semaphore failed.\n");
        return -1;
    }

    gt9147_thread = rt_thread_create("gt9147",
                                    gt911_entry,
                                    RT_NULL,
                                    1024,
                                    20,
                                    5);

    if (gt9147_thread != RT_NULL)
        rt_thread_startup(gt9147_thread);

    return RT_EOK;
}

rt_err_t gt911_register(void)
{
    struct rt_touch_config cfg;
    rt_base_t int_pin = rt_pin_get(INT_PIN);
    rt_base_t rst_pin = rt_pin_get(RST_PIN);

    cfg.dev_name = "sci3i";
    cfg.irq_pin.pin = int_pin;
    cfg.irq_pin.mode = PIN_MODE_INPUT_PULLDOWN;
    cfg.user_data = &rst_pin;

    rt_hw_gt9147_init("gt9147", &cfg);

    gt911_probe(LCD_WIDTH, LCD_HEIGHT);

    return RT_EOK;
}
MSH_CMD_EXPORT(gt911_register, touch device test)
