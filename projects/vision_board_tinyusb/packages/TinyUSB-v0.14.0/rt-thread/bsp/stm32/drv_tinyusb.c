/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-20     tfx2001      first version
 */

#include <tusb.h>
#include <drv_config.h>

#define to_hex_digit(x) ((x) < 10 ? '0' + (x) : 'A' + (x) - 10)

extern void tud_descriptor_set_serial(char *serial_number, uint8_t length);

static void write_hex_chars(char *dest, uint32_t val)
{
    for (int i = 8; i > 0; i--)
    {
        uint8_t x = val >> 28u;
        *dest++ = to_hex_digit(x);
        val <<= 4u;
    }
}

static void generate_serial_number(void)
{
    char serial_number[32];

    write_hex_chars(serial_number, HAL_GetUIDw0());
    write_hex_chars(serial_number + 8, HAL_GetUIDw1());
    write_hex_chars(serial_number + 16, HAL_GetUIDw2());

    tud_descriptor_set_serial(serial_number, 24);
}

int tusb_board_init(void)
{
    PCD_HandleTypeDef hpcd;

    memset(&hpcd, 0, sizeof(hpcd));
    /* Set LL Driver parameters */
    hpcd.Instance = USBD_INSTANCE;
    hpcd.Init.dev_endpoints = 8;
    hpcd.Init.speed = USBD_PCD_SPEED;
    hpcd.Init.ep0_mps = EP_MPS_64;
#if !defined(SOC_SERIES_STM32F1)
    hpcd.Init.phy_itface = USBD_PCD_PHY_MODULE;
#endif
    /* Initialize LL Driver */
    HAL_PCD_Init(&hpcd);
    /* USB interrupt Init */
    HAL_NVIC_SetPriority(USBD_IRQ_TYPE, 2, 0);
    HAL_NVIC_EnableIRQ(USBD_IRQ_TYPE);

    generate_serial_number();

    return 0;
}

void USBD_IRQ_HANDLER(void)
{
    rt_interrupt_enter();

    tud_int_handler(0);

    rt_interrupt_leave();
}
