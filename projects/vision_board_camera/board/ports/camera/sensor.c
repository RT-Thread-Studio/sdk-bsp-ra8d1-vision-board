/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2024-04-06     Rbb666        first version
 */
#include <string.h>
#include <stdint.h>

#include "sensor.h"
#include "hal_data.h"

#define DRV_DEBUG
#define LOG_TAG             "sensor"
#include <drv_log.h>

#define CEU_EVENT_FRAME_END_TIMEOUT 80

sensor_t sensor = {0};
capture_instance_t *gp_ceu_instance = NULL;

static struct rt_completion ceu_completion;

static int ceu_device_open(void);

int sensor_init(void)
{
    int init_ret = 0;

    /* Reset the sesnor state */
    memset(&sensor, 0, sizeof(sensor_t));

    /* Set default snapshot function */
    sensor.snapshot = sensor_snapshot;

    /* Configure the sensor external clock (XCLK) */
    if (sensor_set_xclk_frequency(OMV_XCLK_FREQUENCY) != 0)
    {
        /* Failed to initialize the sensor clock */
        return SENSOR_ERROR_TIM_INIT_FAILED;
    }

    /* Detect and initialize the image sensor */
    if ((init_ret = sensor_probe_init(ISC_I2C_ID, ISC_I2C_SPEED)) != 0)
    {
        LOG_E("not found any sensors !!");
        /* Sensor probe/init failed */
        return init_ret;
    }

    /* Set default color palette */
    sensor.pwdn_pol = ACTIVE_HIGH;

    /* Disable VSYNC IRQ and callback */
    sensor_set_vsync_callback(NULL);

    /* All good! */
    sensor.detected = true;

    rt_completion_init(&ceu_completion);

    return 0;
}

int sensor_set_xclk_frequency(uint32_t frequency)
{
    static uint8_t is_enable = 0;
    if (is_enable)
        return 0;

    R_GPT_Open(&g_timer7_ctrl, &g_timer7_cfg);
    R_GPT_Enable(&g_timer7_ctrl);
    R_GPT_Start(&g_timer7_ctrl);
    LOG_D("Enable 24MHz pwm for camera");

    is_enable = 1;

    return 0;
}

int sensor_set_vsync_callback(vsync_cb_t vsync_cb)
{
    return 0;
}

int sensor_dcmi_config(uint32_t pixformat)
{
    return 0;
}

int sensor_abort(void)
{
    return 0;
}

static int ceu_device_open(void)
{
    capture_status_t ceu_state;
    R_CEU_StatusGet(gp_ceu_instance->p_ctrl, (capture_status_t *)&ceu_state);
    if (ceu_state.state == CAPTURE_STATE_IDLE)
    {
        fsp_err_t err = R_CEU_Open(gp_ceu_instance->p_ctrl, gp_ceu_instance->p_cfg);
        if (err != FSP_SUCCESS)
        {
            LOG_E("R_CEU_Open API FAILED:%d", err);
            return -1;
        }
    }

    LOG_I("Open CEU device");
    return 0;
}

int sensor_set_framesize(framesize_t framesize)
{
    if (sensor.framesize == framesize)
    {
        return 0;
    }

    switch (framesize)
    {
    case FRAMESIZE_QVGA://320x240
        gp_ceu_instance = (capture_instance_t *)&g_ceu_qvga;
        break;
    default:
        break;
    }
    LOG_I("set framesize:%dX%d", gp_ceu_instance->p_cfg->x_capture_pixels, gp_ceu_instance->p_cfg->y_capture_pixels);

    ceu_device_open();

    /* Disable any ongoing frame capture */
    sensor_abort();

    /* Call the sensor specific function */
    if (sensor.set_framesize == NULL)
    {
        return SENSOR_ERROR_CTL_UNSUPPORTED;
    }

    if (sensor.set_framesize(&sensor, framesize) != 0)
    {
        return SENSOR_ERROR_CTL_FAILED;
    }

    /* wait for the camera to settle */
    rt_thread_mdelay(100);

    /* Set framebuffer size */
    sensor.framesize = framesize;

    return 0;
}

void g_ceu_callback(capture_callback_args_t *p_args)
{
    rt_interrupt_enter();

    if (CEU_EVENT_HD == p_args->event)
    {
        /* Capture has started. Process V-Sync event. */
    }
    if (CEU_EVENT_FRAME_END == p_args->event)
    {
        if (sensor.frame_callback)
        {
            sensor.frame_callback();
        }
        rt_completion_done(&ceu_completion);
    }

    rt_interrupt_leave();
}

int write_raw_yuyv_to_gray(sensor_t *sensor, uint8_t *out, const uint8_t *in, size_t len)
{
    // YUV to Grayscale
    if (sensor->hw_flags.gs_bpp == 2)
    {
        size_t end = len / 8;
        for (size_t i = 0; i < end; ++i)
        {
            out[0] = in[0];
            out[1] = in[2];
            out[2] = in[4];
            out[3] = in[6];
            out += 4;
            in += 8;
        }
        return len / 2;
    }

    // just memcpy
    memcpy(out, in, len);
    return len;
}

/* This is the default snapshot function, which can be replaced in sensor_init functions. */
int sensor_snapshot(sensor_t *sensor, uint8_t *image, uint32_t flags)
{
    if (sensor->pixformat == PIXFORMAT_INVALID)
    {
        return SENSOR_ERROR_INVALID_PIXFORMAT;
    }

    if (sensor->framesize == FRAMESIZE_INVALID)
    {
        return SENSOR_ERROR_INVALID_FRAMESIZE;
    }

    /* Start capture image and store it in the buffer specified by image buffer pointer */
    fsp_err_t err = R_CEU_CaptureStart(gp_ceu_instance->p_ctrl, image);
    if (err != FSP_SUCCESS)
    {
        LOG_E("R_CEU_CaptureStart API FAILED:%d", err);
        return -1;
    }

    rt_completion_wait(&ceu_completion, CEU_EVENT_FRAME_END_TIMEOUT);

    return 0;
}
