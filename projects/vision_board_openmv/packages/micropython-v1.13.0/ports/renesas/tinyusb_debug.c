/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 * Copyright (c) 2019 Glenn Ruben Bakke
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */
#include "omv_boardconfig.h"
#if (OMV_ENABLE_TUSBDBG == 1)
#include "py/runtime.h"
#include "py/stream.h"
#include "py/mphal.h"
#include "py/ringbuf.h"
#include "extmod/misc.h"

#include "tusb.h"
#include "usbdbg.h"
#include "tinyusb_debug.h"

#include "drv_log.h"
#define DRV_DEBUG
#define LOG_TAG             "usb_cdc"

#define DBG_MAX_PACKET      (OMV_TUSBDBG_PACKET)
#define IDE_BAUDRATE_SLOW   (921600)
#define IDE_BAUDRATE_FAST   (12000000)

extern void __fatal_error();

typedef struct __attribute__((packed))
{
    uint8_t cmd;
    uint8_t request;
    uint32_t xfer_length;
}
usbdbg_cmd_t;

static uint8_t debug_ringbuf_array[512];
static volatile bool tinyusb_debug_mode = false;
ringbuf_t debug_ringbuf = { debug_ringbuf_array, sizeof(debug_ringbuf_array) };

/* if there is not enable heap, we should use static thread and stack. */
static rt_uint8_t tusb_stack[PKG_TINYUSB_STACK_SIZE];
static struct rt_thread tusb_thread;

static int init_tinyusb(void);

uint32_t usb_cdc_buf_len()
{
    return ringbuf_avail((ringbuf_t *) &debug_ringbuf);
}

uint32_t usb_cdc_get_buf(uint8_t *buf, uint32_t len)
{
    for (int i = 0; i < len; i++)
    {
        buf[i] = ringbuf_get((ringbuf_t *) &debug_ringbuf);
    }
    return len;
}

bool tinyusb_debug_enabled(void)
{
    return tinyusb_debug_mode;
}

void tinyusb_debug_tx_strn(const char *str, mp_uint_t len)
{
    // TODO can be faster.
    if (tinyusb_debug_enabled() && tud_cdc_connected())
    {
        for (int i = 0; i < len; i++)
        {
            rt_base_t level;
            level = rt_hw_interrupt_disable();
            ringbuf_put((ringbuf_t *) &debug_ringbuf, str[i]);
            rt_hw_interrupt_enable(level);
        }
    }
}

static void tinyusb_debug_task(void)
{    
    if (!tinyusb_debug_enabled() || !tud_cdc_connected() || tud_cdc_available() < 6) {
        return;
    }
    
    uint8_t dbg_buf[DBG_MAX_PACKET];
    uint32_t count = tud_cdc_read(dbg_buf, 6);
    if (count < 6 || dbg_buf[0] != 0x30)
    {
        // Maybe we should try to recover from this state
        // but for now, call __fatal_error which doesn't
        // return.
        __fatal_error();
        return;
    }
    usbdbg_cmd_t *cmd = (usbdbg_cmd_t *) dbg_buf;
    uint8_t request = cmd->request;
    uint32_t xfer_length = cmd->xfer_length;
    usbdbg_control(NULL, request, xfer_length);

    while (xfer_length)
    {
        // && tud_cdc_connected())
        if (tud_task_event_ready())
        {
            tud_task();
        }
        if (request & 0x80)
        {
            // Device-to-host data phase
            int bytes = MIN(xfer_length, DBG_MAX_PACKET);
            if (bytes <= tud_cdc_write_available())
            {
                xfer_length -= bytes;
                usbdbg_data_in(dbg_buf, bytes);
                tud_cdc_write(dbg_buf, bytes);
            }
            tud_cdc_write_flush();
        }
        else
        {
            // Host-to-device data phase
            int bytes = MIN(xfer_length, DBG_MAX_PACKET);
            uint32_t count = tud_cdc_read(dbg_buf, bytes);
            xfer_length -= count;
            usbdbg_data_out(dbg_buf, count);
        }
    }
}

__attribute__((__used__))
void tud_cdc_line_coding_cb(uint8_t itf, cdc_line_coding_t const *p_line_coding)
{
    debug_ringbuf.iget = 0;
    debug_ringbuf.iput = 0;

    if (p_line_coding->bit_rate == IDE_BAUDRATE_SLOW ||
            p_line_coding->bit_rate == IDE_BAUDRATE_FAST)
    {
        tinyusb_debug_mode = true;
    }
    else
    {
        tinyusb_debug_mode = false;
    }
}

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
        if (tinyusb_debug_enabled())
        {
            tinyusb_debug_task();
        }
    }
}

int usb_cdc_init(void)
{
    static bool initialized = false;
    if (!initialized)
    {
        initialized = true;
        init_tinyusb();
    }
    return 0;
}

static int init_tinyusb(void)
{
    rt_thread_t tid;
    tusb_init();

    rt_err_t result;

    tid = &tusb_thread;
    result = rt_thread_init(tid, "usb", tusb_thread_entry, RT_NULL,
                            tusb_stack, sizeof(tusb_stack), PKG_TINYUSB_THREAD_PRIORITY, 10);
    if (result != RT_EOK)
    {
        LOG_E("Fail to create tinyusb thread");
        return -1;
    }

    rt_thread_startup(tid);

    return 0;
}
#endif // OMV_ENABLE_TUSBDBG
