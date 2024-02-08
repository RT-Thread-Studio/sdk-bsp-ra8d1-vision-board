/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-23     Mr.Tiger     first version
 * 2022-12-7      Vandoul      ADD sci spi
 */

#ifndef __DRV_SCI_SPI_H__
#define __DRV_SCI_SPI_H__

#include <rtthread.h>
#include <rtdevice.h>
#include "hal_data.h"
#include "board.h"
#include <rthw.h>
#include <drv_common.h>
#include <drv_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(R_SCI_SPI_H) || defined(R_SCI_B_SPI_H)
struct ra_sci_spi_handle
{
    const char bus_name[RT_NAME_MAX];
    const spi_cfg_t               *spi_cfg_t;
#ifdef R_SCI_B_SPI_H
    const sci_b_spi_instance_ctrl_t *spi_ctrl_t;
#else
    const sci_spi_instance_ctrl_t *spi_ctrl_t;
#endif
};

struct ra_sci_spi
{
    rt_uint32_t                  cs_pin;
    struct ra_sci_spi_handle    *ra_spi_handle_t;
    struct rt_spi_configuration *rt_spi_cfg_t;
    struct rt_spi_bus            bus;
};
#endif

rt_err_t rt_hw_sci_spi_device_attach(const char *bus_name, const char *device_name, rt_base_t cs_pin);

#ifdef __cplusplus
}
#endif

/* stm32 spi dirver class */

#endif /*__DRV_SPI_H__ */
