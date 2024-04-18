/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
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

#ifndef _USB_DESCRIPTORS_H_
#define _USB_DESCRIPTORS_H_

#ifdef __RTTHREAD__
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
#endif /*__RTTHREAD__*/

#endif /* _USB_DESCRIPTORS_H_ */
