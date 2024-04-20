/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-04     tfx2001      first version
 */

#include <stdint.h>
#include <rtthread.h>
#include <tusb.h>

static void cdc_example(void)
{
    uint8_t buffer[32];
    char ch;

    if (tud_cdc_connected())
    {
        rt_memset(buffer, 0, 32);

        tud_cdc_write_str("please enter something: ");
        tud_cdc_write_flush();

        tud_cdc_read_flush();
        for (int i = 0; i < 32; i++)
        {
            while (!tud_cdc_available()) { rt_thread_mdelay(10); }
            // read char
            ch = tud_cdc_read_char();
            *(buffer + i) = ch;
            // echo
            tud_cdc_write_char(ch);
            tud_cdc_write_flush();
            // end with CR
            if (ch == '\r') { break; }
        }
        tud_cdc_write_str("\r\nwhat you enter: ");
        tud_cdc_write((const char *) buffer, 32);
        tud_cdc_write_str("\r\n");
        tud_cdc_write_flush();
    }
    else
    {
        rt_kprintf("please open port and make sure DTR=1\n");
    }
}

MSH_CMD_EXPORT(cdc_example, TinyUSB cdc example)
