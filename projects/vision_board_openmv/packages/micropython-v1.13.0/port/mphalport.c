/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018,2021 Damien P. George
 * Copyright (c) 2021,2022 Renesas Electronics Corporation
 * Copyright (c) 2023 Arduino SA
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
 */

#include <rtthread.h>
#include <time.h>
#include <stdlib.h>
#include "mphalport.h"
#include "mpgetcharport.h"
#include "mpputsnport.h"

#include "py/runtime.h"
#include "py/stream.h"
#include "py/mphal.h"
#include "py/mperrno.h"
#include "py/mphal.h"
#include "py/ringbuf.h"
#include "py/obj.h"
#include "extmod/misc.h"
#include "interrupt_char.h"
#include "tinyusb_debug.h"
#include "tusb.h"
#include "hal_data.h"

typedef enum {
    HAL_OK       = 0x00,
    HAL_ERROR    = 0x01,
    HAL_BUSY     = 0x02,
    HAL_TIMEOUT  = 0x03
} HAL_StatusTypeDef;

const char rtthread_help_text[] =
    "Welcome to MicroPython on RT-Thread!\n"
    "\n"
    "Control commands:\n"
    "  CTRL-A        -- on a blank line, enter raw REPL mode\n"
    "  CTRL-B        -- on a blank line, enter normal REPL mode\n"
    "  CTRL-C        -- interrupt a running program\n"
    "  CTRL-D        -- on a blank line, do a soft reset of the board\n"
    "  CTRL-E        -- on a blank line, enter paste mode\n"
    "\n"
    "For further help on a specific object, type help(obj)\n"
    ;

#if MICROPY_HW_ENABLE_INTERNAL_FLASH_STORAGE
    void flash_cache_commit(void);
#endif

#if MICROPY_HW_ENABLE_UART_REPL || MICROPY_HW_USB_CDC

#ifndef MICROPY_HW_STDIN_BUFFER_LEN
    #define MICROPY_HW_STDIN_BUFFER_LEN 1024
#endif

STATIC uint8_t stdin_ringbuf_array[MICROPY_HW_STDIN_BUFFER_LEN];
ringbuf_t stdin_ringbuf = { stdin_ringbuf_array, sizeof(stdin_ringbuf_array) };

#endif

mp_uint_t mp_hal_ticks_us(void)
{
    return rt_tick_get() * 1000000UL / RT_TICK_PER_SECOND;
}

mp_uint_t mp_hal_ticks_ms(void)
{
    return rt_tick_get_millisecond();
}

mp_uint_t mp_hal_ticks_cpu(void)
{
    return rt_tick_get();
}

void mp_hal_delay_us(mp_uint_t us)
{
    rt_tick_t t0 = rt_tick_get(), t1, dt;
    uint64_t dtick = us * RT_TICK_PER_SECOND / 1000000L;
    while (1)
    {
        t1 = rt_tick_get();
        dt = t1 - t0;
        if (dt >= dtick)
        {
            break;
        }
        mp_handle_pending(true);
    }
}

void mp_hal_delay_ms(mp_uint_t ms)
{
    MICROPY_EVENT_POLL_HOOK;
    rt_thread_mdelay(ms);
}

int mp_port_get_freq(int clkid, int *freq)
{
    int ret = 0;
    uint32_t value;
    value = SystemCoreClock;
    *freq = (int)value;
    return ret;
}

// this table converts from HAL_StatusTypeDef to POSIX errno
const byte mp_hal_status_to_errno_table[4] =
{
    [HAL_OK] = 0,
    [HAL_ERROR] = MP_EIO,
    [HAL_BUSY] = MP_EBUSY,
    [HAL_TIMEOUT] = MP_ETIMEDOUT,
};

#if MICROPY_HW_USB_CDC
uint8_t cdc_itf_pending; // keep track of cdc interfaces which need attention to poll

void poll_cdc_interfaces(void)
{
    // any CDC interfaces left to poll?
    if (cdc_itf_pending && ringbuf_free(&stdin_ringbuf))
    {
        for (uint8_t itf = 0; itf < 8; ++itf)
        {
            if (cdc_itf_pending & (1 << itf))
            {
                tud_cdc_rx_cb(itf);
                if (!cdc_itf_pending)
                {
                    break;
                }
            }
        }
    }
}

void tud_cdc_rx_cb(uint8_t itf)
{
    // consume pending USB data immediately to free usb buffer and keep the endpoint from stalling.
    // in case the ringbuffer is full, mark the CDC interface that need attention later on for polling
    cdc_itf_pending &= ~(1 << itf);
    for (uint32_t bytes_avail = tud_cdc_n_available(itf);
            !tinyusb_debug_enabled() && bytes_avail > 0; --bytes_avail)
    {
        if (ringbuf_free(&stdin_ringbuf))
        {
            int data_char = tud_cdc_read_char();
            if (data_char == mp_interrupt_char)
            {
                mp_keyboard_interrupt();
            }
            else
            {
                ringbuf_put(&stdin_ringbuf, data_char);
            }
        }
        else
        {
            cdc_itf_pending |= (1 << itf);
            return;
        }
    }
}
#endif

uintptr_t mp_hal_stdio_poll(uintptr_t poll_flags)
{
    uintptr_t ret = 0;
#if MICROPY_HW_USB_CDC
    poll_cdc_interfaces();
#endif
#if MICROPY_HW_ENABLE_UART_REPL || MICROPY_HW_USB_CDC
    if ((poll_flags & MP_STREAM_POLL_RD) && ringbuf_peek(&stdin_ringbuf) != -1)
    {
        ret |= MP_STREAM_POLL_RD;
    }
    if (poll_flags & MP_STREAM_POLL_WR)
    {
#if MICROPY_HW_ENABLE_UART_REPL
        ret |= MP_STREAM_POLL_WR;
#else
        if (tud_cdc_connected() && tud_cdc_write_available() > 0)
        {
            ret |= MP_STREAM_POLL_WR;
        }
#endif
    }
#endif
#if MICROPY_PY_OS_DUPTERM
    ret |= mp_uos_dupterm_poll(poll_flags);
#endif
    return ret;
}

// Receive single character
int mp_hal_stdin_rx_chr(void)
{
    for (;;)
    {
#if MICROPY_HW_USB_CDC
        poll_cdc_interfaces();
#endif

        int c = ringbuf_get(&stdin_ringbuf);
        if (c != -1)
        {
            return c;
        }
#if MICROPY_PY_OS_DUPTERM
        int dupterm_c = mp_uos_dupterm_rx_chr();
        if (dupterm_c >= 0)
        {
            return dupterm_c;
        }
#endif
        MICROPY_EVENT_POLL_HOOK
    }
}

// Send string of given length
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len)
{
    tinyusb_debug_tx_strn(str, len);

#if MICROPY_HW_ENABLE_UART_REPL
    mp_uart_write_strn(str, len);
#endif

#if MICROPY_HW_USB_CDC
    if (tud_cdc_connected() && !tinyusb_debug_enabled())
    {
        for (size_t i = 0; i < len;)
        {
            uint32_t n = len - i;
            if (n > CFG_TUD_CDC_EP_BUFSIZE)
            {
                n = CFG_TUD_CDC_EP_BUFSIZE;
            }
            int timeout = 0;
            // Wait with a max of USC_CDC_TIMEOUT ms
            while (n > tud_cdc_write_available() && timeout++ < MICROPY_HW_USB_CDC_TX_TIMEOUT)
            {
                MICROPY_EVENT_POLL_HOOK
            }
            if (timeout >= MICROPY_HW_USB_CDC_TX_TIMEOUT)
            {
                break;
            }
            uint32_t n2 = tud_cdc_write(str + i, n);
            tud_cdc_write_flush();
            i += n2;
        }
    }
#endif

#if MICROPY_PY_OS_DUPTERM
    mp_uos_dupterm_tx_strn(str, len);
#endif
}
/*******************************************************************************/
void fmath_init(void)
{
    srand((unsigned int)time(0));
}

uint32_t rng_randint(uint32_t min, uint32_t max)
{
    uint32_t tmp = max - min;
    tmp = rand() % tmp + min;
    return tmp;
}

#if MICROPY_HW_ENABLE_USBDEV
void usbfs_interrupt_handler(void)
{
    rt_interrupt_enter();

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

#if CFG_TUSB_RHPORT0_MODE & OPT_MODE_HOST
    tuh_int_handler(0);
#endif

#if CFG_TUSB_RHPORT0_MODE & OPT_MODE_DEVICE
    tud_int_handler(0);
#endif

    rt_interrupt_leave();
}

void usbfs_resume_handler(void)
{
    rt_interrupt_enter();

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

#if CFG_TUSB_RHPORT0_MODE & OPT_MODE_HOST
    tuh_int_handler(0);
#endif

#if CFG_TUSB_RHPORT0_MODE & OPT_MODE_DEVICE
    tud_int_handler(0);
#endif
    rt_interrupt_leave();
}

void usbfs_d0fifo_handler(void)
{
    rt_interrupt_enter();

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

#if CFG_TUSB_RHPORT0_MODE & OPT_MODE_HOST
    tuh_int_handler(0);
#endif

#if CFG_TUSB_RHPORT0_MODE & OPT_MODE_DEVICE
    tud_int_handler(0);
#endif
    rt_interrupt_leave();
}

void usbfs_d1fifo_handler(void)
{
    rt_interrupt_enter();

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

#if CFG_TUSB_RHPORT0_MODE & OPT_MODE_HOST
    tuh_int_handler(0);
#endif

#if CFG_TUSB_RHPORT0_MODE & OPT_MODE_DEVICE
    tud_int_handler(0);
#endif
    rt_interrupt_leave();
}

void usbhs_interrupt_handler(void)
{
    rt_interrupt_enter();

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

#if CFG_TUSB_RHPORT1_MODE & OPT_MODE_HOST
    tuh_int_handler(1);
#endif

#if CFG_TUSB_RHPORT1_MODE & OPT_MODE_DEVICE
    tud_int_handler(1);
#endif
    rt_interrupt_leave();
}

void usbhs_d0fifo_handler(void)
{
    rt_interrupt_enter();

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

#if CFG_TUSB_RHPORT1_MODE & OPT_MODE_HOST
    tuh_int_handler(1);
#endif

#if CFG_TUSB_RHPORT1_MODE & OPT_MODE_DEVICE
    tud_int_handler(1);
#endif
    rt_interrupt_leave();
}

void usbhs_d1fifo_handler(void)
{
    rt_interrupt_enter();

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

#if CFG_TUSB_RHPORT1_MODE & OPT_MODE_HOST
    tuh_int_handler(1);
#endif

#if CFG_TUSB_RHPORT1_MODE & OPT_MODE_DEVICE
    tud_int_handler(1);
#endif
    rt_interrupt_leave();
}
#endif
