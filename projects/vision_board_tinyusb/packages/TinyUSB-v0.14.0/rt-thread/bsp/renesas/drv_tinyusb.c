/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-07     Lishuo       first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <tusb.h>
#include "hal_data.h"

#define USB_CH_SEL_PIN    BSP_IO_PORT_10_PIN_13

int tusb_board_init(void)
{
    rt_pin_write(USB_CH_SEL_PIN, PIN_HIGH);
    return 0;
}

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
