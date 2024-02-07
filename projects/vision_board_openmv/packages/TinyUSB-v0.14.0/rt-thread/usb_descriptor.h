/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-01-15     tfx2001      first version
 */

#ifndef _USB_DESCRIPTORS_H_
#define _USB_DESCRIPTORS_H_

#include <rtconfig.h>

enum
{
#if CFG_TUD_CDC
    ITF_NUM_CDC = 0,
    ITF_NUM_CDC_DATA,
#endif
#if CFG_TUD_MSC
    ITF_NUM_MSC,
#endif
#if CFG_TUD_HID
    ITF_NUM_HID,
#endif
    ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN    (TUD_CONFIG_DESC_LEN + \
                             TUD_MSC_DESC_LEN * CFG_TUD_MSC + \
                             TUD_CDC_DESC_LEN * CFG_TUD_CDC + \
                             TUD_HID_DESC_LEN * CFG_TUD_HID)

#define EPNUM_CDC_NOTIF     (0x80 | PKG_TINYUSB_DEVICE_CDC_EPNUM_NOTIF)
#define EPNUM_CDC_OUT       PKG_TINYUSB_DEVICE_CDC_EPNUM
#define EPNUM_CDC_IN        (0x80 | PKG_TINYUSB_DEVICE_CDC_EPNUM)

#define EPNUM_MSC_OUT       PKG_TINYUSB_DEVICE_MSC_EPNUM
#define EPNUM_MSC_IN        (0x80 | PKG_TINYUSB_DEVICE_MSC_EPNUM)
  
#define EPNUM_HID           (0x80 | PKG_TINYUSB_DEVICE_HID_EPNUM)

enum
{
    REPORT_ID_BEGIN = 0,
#ifdef PKG_TINYUSB_DEVICE_HID_KEYBOARD
    REPORT_ID_KEYBOARD,
#endif
#ifdef PKG_TINYUSB_DEVICE_HID_MOUSE
    REPORT_ID_MOUSE,
#endif
#ifdef PKG_TINYUSB_DEVICE_HID_CONSUMER
    REPORT_ID_CONSUMER_CONTROL,
#endif
#ifdef PKG_TINYUSB_DEVICE_HID_GAMEPAD
    REPORT_ID_GAMEPAD,
#endif
    REPORT_ID_COUNT
};

#endif /* _USB_DESCRIPTORS_H_ */
