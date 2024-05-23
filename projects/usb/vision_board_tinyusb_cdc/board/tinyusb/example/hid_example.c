/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-04     tfx2001      first version
 */

#include <stdarg.h>
#include <stdint.h>
#include <math.h>

#include <rtthread.h>
#include <tusb.h>

#include "../usb_descriptors.h"

#define CIRCLE_RADIUS 300

static const uint8_t conv_table[128][2] =  { HID_ASCII_TO_KEYCODE };
static struct rt_semaphore hid_sem;

void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
    (void) instance;
    (void) report;
    (void) len;

    rt_sem_release(&hid_sem);
}

static void hid_press_key(uint8_t modifier, uint8_t keycode)
{
    uint8_t keycode_array[6] = {0};

    keycode_array[0] = keycode;
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, modifier, keycode_array);
    rt_sem_take(&hid_sem, 100);
    keycode_array[0] = 0;
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode_array);
    rt_sem_take(&hid_sem, 100);
}

static void hid_type_str_fmt(const char *fmt, ...)
{
    uint8_t  keycode;
    uint8_t  modifier;
    uint32_t length;
    va_list  args;
    char     buffer[64];
    char     chr;

    va_start(args, fmt);
    rt_vsnprintf(buffer, 64, fmt, args);

    length = rt_strlen(buffer);
    for (int i = 0; i < length; i++)
    {
        chr = buffer[i];
        if (conv_table[chr][0])
        {
            modifier = KEYBOARD_MODIFIER_LEFTSHIFT;
        }
        else
        {
            modifier = 0;
        }
        keycode = conv_table[chr][1];
        hid_press_key(modifier, keycode);
    }
}

void hid_example(void)
{
    float last_x_pos = CIRCLE_RADIUS, x_pos;
    float last_y_pos = 0, y_pos;

    // Init semaphore
    rt_sem_init(&hid_sem, "hid", 0, RT_IPC_FLAG_PRIO);

    if (tud_connected())
    {
        // Make the mouse move like a circle
        for (int i = 0; i < 360; i++)
        {
            x_pos = cosf((float)i / 57.3F) * CIRCLE_RADIUS;
            y_pos = sinf((float)i / 57.3F) * CIRCLE_RADIUS;

            tud_hid_mouse_report(REPORT_ID_MOUSE, 0,
                                 (int8_t)(x_pos - last_x_pos), (int8_t)(y_pos - last_y_pos), 0, 0);
            rt_sem_take(&hid_sem, 100);

            last_x_pos = x_pos;
            last_y_pos = y_pos;
        }
        rt_thread_mdelay(500);

        // Open notepad and type something...
        hid_press_key(KEYBOARD_MODIFIER_LEFTGUI, HID_KEY_R);
        rt_thread_mdelay(500);
        hid_type_str_fmt("notepad\n\n");
        rt_thread_mdelay(2000);
        hid_type_str_fmt("This is RT-Thread TinyUSB demo.\n");
    }
    else
    {
        rt_kprintf("please connect USB port\n");
    }

    rt_sem_detach(&hid_sem);
}
MSH_CMD_EXPORT(hid_example, TinyUSB hid example)
