/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-09     Rbb666       The first version
 */
#include <lvgl.h>
#include <rtdevice.h>

#include "cst812t.h"

#define DBG_TAG "lv_port_indev"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include "hal_data.h"

static rt_device_t touch_dev = RT_NULL;
static struct rt_touch_data read_data;

static rt_err_t touch_probe()
{
    touch_dev = rt_device_find("cst8xx");
    if (touch_dev == RT_NULL)
    {
        LOG_E("can't find device cst8xx");
        return -RT_ERROR;
    }
    if (rt_device_open(touch_dev, RT_DEVICE_FLAG_INT_RX) != RT_EOK)
    {
        LOG_E("open device failed!");
        return -RT_ERROR;
    }
    return RT_EOK;
}

static void touchpad_read(lv_indev_t *indev_drv, lv_indev_data_t *data)
{
    if (rt_device_read(touch_dev, 0, &read_data, 1) == 1)
    {
        data->point.x  = read_data.x_coordinate;
        data->point.y  = read_data.y_coordinate;
        
        switch (read_data.event)
        {
        case TOUCH_EVENT_UP:
            data->state = LV_INDEV_STATE_REL;
            break;
        case TOUCH_EVENT_DOWN:
            data->state = LV_INDEV_STATE_PR;
            break;
        case TOUCH_EVENT_MOVE:
            data->state = LV_INDEV_STATE_PR;
            break;
        default:
            break;
        }
    }
}

#define RST_PIN   "p000"
#define INT_PIN   "p010"
#define TOUCH_DEVICE_NAME "sci3i"

int rt_hw_cst812t_register(void)
{
    struct rt_touch_config cfg;
    rt_base_t int_pin = rt_pin_get(INT_PIN);
    rt_base_t rst_pin = rt_pin_get(RST_PIN);

    cfg.dev_name = TOUCH_DEVICE_NAME;
    cfg.irq_pin.pin = int_pin;
    cfg.irq_pin.mode = PIN_MODE_INPUT_PULLUP;
    cfg.user_data = &rst_pin;

    rt_hw_cst8xx_init("cst8xx", &cfg);
    cst8xx_probe();
    cst8xx_reset(20);

    return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_cst812t_register);

void lv_port_indev_init(void)
{
    lv_indev_t *indev_touchpad;

    if (touch_probe() != RT_EOK)
    {
        rt_kprintf("probe cst812t failed.\n");
        return;
    }
    /*Register a touchpad input device*/
    indev_touchpad = lv_indev_create();
    lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touchpad, touchpad_read);

    LV_IMAGE_DECLARE(mouse_cursor_icon);                /* Declare the image file. */
    lv_obj_t *cursor_obj;
    cursor_obj = lv_image_create(lv_screen_active());   /* Create an image object for the cursor */
    lv_image_set_src(cursor_obj, &mouse_cursor_icon);   /* Set the image source*/
    lv_indev_set_cursor(indev_touchpad, cursor_obj);    /* Connect the image  object to the driver*/
}
