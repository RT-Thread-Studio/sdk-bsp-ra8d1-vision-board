/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2024-04-06     Rbb666        first version
 */
#include <rtthread.h>

#include <drv_lcd.h>
#include <tusb.h>
#include "sensor.h"

#define CAM_WIDTH   320
#define CAM_HEIGHT  240

extern sensor_t sensor;
uint8_t g_image_yuv422_sdram_buffer[CAM_WIDTH * CAM_HEIGHT * 16 / 8] BSP_PLACE_IN_SECTION(".sdram") BSP_ALIGN_VARIABLE(8);

void hal_entry(void)
{
    rt_kprintf("This is a tinyusb uvc demo.\n\n");
    rt_kprintf("===========================================================================\n");
    rt_kprintf("The example will become a camera device under the Windows system, you can open the camera to view.\n");
    rt_kprintf("===========================================================================\n");

    sensor_init();
    sensor_reset();
    sensor_set_hmirror(true);
    sensor_set_vflip(true);
    sensor_set_pixformat(PIXFORMAT_YUV422);
    sensor_set_framesize(FRAMESIZE_QVGA);

    while (1)
    {
        sensor_snapshot(&sensor, g_image_yuv422_sdram_buffer, 0);
        if (tud_video_n_streaming(0, 0))
        {
            tud_video_n_frame_xfer(0, 0, (void *)g_image_yuv422_sdram_buffer, CAM_WIDTH * CAM_HEIGHT * 16 / 8);
        }
        rt_thread_mdelay(10);
    }
}
