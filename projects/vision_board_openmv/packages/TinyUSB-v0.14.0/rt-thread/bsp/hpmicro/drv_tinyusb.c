/*
 * Copyright (c) 2021 hpmicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <tusb.h>
#include "board.h"

extern void tud_descriptor_set_serial(char *serial_number, uint8_t length);

TU_ATTR_WEAK void generate_serial_number(void)
{
    char serial_number[32] = {"00001"};

    tud_descriptor_set_serial(serial_number, sizeof(serial_number));
}

TU_ATTR_WEAK int tusb_board_init(void)
{
    generate_serial_number();

    return 0;
}

TU_ATTR_WEAK void isr_usb0(void)
{
    rt_interrupt_enter();
    dcd_int_handler(0);
    rt_interrupt_leave();
}
SDK_DECLARE_EXT_ISR_M(IRQn_USB0, isr_usb0)

TU_ATTR_WEAK void isr_usb1(void)
{
    rt_interrupt_enter();
    dcd_int_handler(1);
    rt_interrupt_leave();
}
SDK_DECLARE_EXT_ISR_M(IRQn_USB1, isr_usb1)
