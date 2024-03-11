/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-01     Yuanjie      Add cst812t Touch Device
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "cst812t.h"

#define DBG_LEVEL DBG_INFO // DBG_LOG //
#define LOG_TAG "cst812t"
#include <rtdbg.h>

#ifdef PKG_USING_CST812T

static struct rt_i2c_bus_device *ft_bus = NULL;

void cst8xx_reset(uint16_t ms)
{
    rt_pin_write(TOUCH_RST_PIN, PIN_LOW);
    rt_thread_mdelay(ms);
    rt_pin_write(TOUCH_RST_PIN, PIN_HIGH);
    // rt_thread_mdelay(10);
}

static void cst8xx_gpio_init()
{
    rt_pin_mode(TOUCH_RST_PIN, PIN_MODE_OUTPUT);
    //    rt_pin_mode(TOUCH_IRQ_PIN, PIN_MODE_INPUT_PULLUP);
}

rt_err_t cst816_i2c_read_reg8(uint8_t reg, uint8_t *p_data, uint8_t len)
{
    rt_int8_t res = 0;
    struct rt_i2c_msg msgs[2];


    msgs[0].addr = TOUCH_SLAVE_ADDRESS; /* Slave address */
    msgs[0].flags = RT_I2C_WR;          /* Read flag */
    msgs[0].buf = &reg;                 /* Read data pointer */
    msgs[0].len = 1;                    /* Number of bytes read */

    msgs[1].addr = TOUCH_SLAVE_ADDRESS; /* Slave address */
    msgs[1].flags = RT_I2C_RD;          /* Read flag */
    msgs[1].buf = p_data;               /* Read data pointer */
    msgs[1].len = len;                  /* Number of bytes read */

    if (rt_i2c_transfer(ft_bus, msgs, 2) == 1)
    {
        res = RT_EOK;
    }
    else
    {
        res = -1;
    }
    return res;
}

int cst816_i2c_write_reg16(uint8_t device_addr, uint16_t reg, uint8_t *p_data, uint16_t len)
{
    rt_int8_t res = 0;
    struct rt_i2c_msg msgs;

    uint8_t *send_buffer = rt_malloc(len + sizeof(uint16_t));
    RT_ASSERT(send_buffer);
    send_buffer[0] = (reg >> 8);
    send_buffer[1] = (reg & 0xFF);

    rt_memcpy(send_buffer + sizeof(uint16_t), p_data, len);

    msgs.addr = device_addr;           /* slave address */
    msgs.flags = RT_I2C_WR;            /* write flag */
    msgs.buf = (uint8_t *)send_buffer; /* Send data pointer */
    msgs.len = len + sizeof(uint16_t);

    res = rt_i2c_transfer(ft_bus, &msgs, 1);
    rt_free(send_buffer);
    send_buffer = RT_NULL;

    return res;
}

int cst816_i2c_read_reg16(uint8_t device_addr, uint16_t reg, uint8_t *p_data, uint16_t len)
{
    rt_int8_t res = 0;
    rt_uint8_t buf[2];
    struct rt_i2c_msg msgs[2];

    buf[0] = reg >> 8;          // cmd
    buf[1] = reg;               // cmd
    msgs[0].addr = device_addr; /* Slave address */
    msgs[0].flags = RT_I2C_WR;  /* Read flag */
    msgs[0].buf = buf;          /* Read data pointer */
    msgs[0].len = 2;            /* Number of bytes read */

    msgs[1].addr = device_addr; /* Slave address */
    msgs[1].flags = RT_I2C_RD;  /* Read flag */
    msgs[1].buf = p_data;       /* Read data pointer */
    msgs[1].len = len;          /* Number of bytes read */

    if (rt_i2c_transfer(ft_bus, msgs, 2) == 1)
    {
        res = 0;
    }
    else
    {
        res = -1;
    }
    return res;
}

rt_size_t cst816_i2c_write(uint8_t device_addr, uint16_t reg, uint8_t *p_data, uint16_t len)
{
    rt_size_t res;
    res = cst816_i2c_write_reg16(device_addr, reg, p_data, len);

    return res;
}

rt_size_t cst816_i2c_read(uint8_t device_addr, const uint16_t reg, uint8_t *p_data, uint16_t len)
{
    rt_size_t res;
    res = cst816_i2c_read_reg16(device_addr, reg, p_data, len);

    return res;
}

static void cst816_irq_enable(rt_bool_t enable)
{
    rt_pin_irq_enable(TOUCH_IRQ_PIN, enable ? 1 : 0);
}

void cst816_irq_handler(void *arg)
{
    rt_err_t ret = RT_ERROR;

    rt_interrupt_enter();

    LOG_D("cst816 touch_irq_handler\n");

    // rt_pin_irq_enable(TOUCH_IRQ_PIN, 0);

    RT_ASSERT(RT_EOK == ret);

    rt_interrupt_leave();
}

static rt_size_t cst8xx_read_point(struct rt_touch_device *touch, void *buf, rt_size_t touch_num)
{
    static struct rt_touch_data *read_data;
    uint8_t rbuf[5] = {0};

    read_data = (struct rt_touch_data *)buf;
    // rt_pin_irq_enable(TOUCH_IRQ_PIN, 1);
    cst816_i2c_read_reg8(CST8XX_REG_FINGERNUM, rbuf, 5); // 0x02-0x06

    if (CST8XX_REG_FINGERNUM_ONE == (rbuf[0] & 0x0F))
    {
        read_data->event = TOUCH_EVENT_DOWN;
    }
    else
    {
        read_data->event = TOUCH_EVENT_UP;
    }
    read_data->x_coordinate = (((uint16_t)(rbuf[1] & 0x0F)) << 8) | rbuf[2];
    read_data->y_coordinate = (((uint16_t)(rbuf[3] & 0X0F)) << 8) | rbuf[4];
    read_data->track_id = 1;
    read_data->width = 4;
    read_data->timestamp = rt_touch_get_ts();

    LOG_D("event=%d, X=%d, Y=%d", read_data->event, read_data->x_coordinate, read_data->y_coordinate);

    return 1;
}

static rt_err_t cst8xx_control(struct rt_touch_device *touch, int cmd, void *arg)
{
    struct rt_touch_info *info;
    info = (struct rt_touch_info *)arg;

    switch (cmd)
    {
    case RT_TOUCH_CTRL_GET_ID:
    {
        break;
    }

    case RT_TOUCH_CTRL_GET_INFO:
    {
        info->type = RT_TOUCH_TYPE_CAPACITANCE;
        info->vendor = RT_TOUCH_VENDOR_UNKNOWN;
        info->point_num = 1;
        info->range_x = 480;
        info->range_y = 360;
        break;
    }

    case RT_TOUCH_CTRL_SET_MODE:
    {
        break;
    }
    default:
        break;
    }
    return RT_EOK;
}

static rt_err_t init(void)
{
    LOG_D("cst812t init");
    cst8xx_gpio_init();
    cst8xx_reset(20);
    rt_thread_mdelay(10);

    uint8_t id = 0;
    cst816_i2c_read_reg8(CST8XX_REG_FWVERSION, &id, 1);
    rt_kprintf("cts812t fw=0x%x\n", id);

    //    {
    //        struct rt_i2c_configuration configuration =
    //        {
    //            .mode = 0,
    //            .addr = 0,
    //            .timeout = 5000,
    //            .max_hz  = 400000,
    //        };

    //        i2c_bus_configure(ft_bus, &configuration);
    //    }
    LOG_I("cst812t probe OK");

    // No need for FSP configure
    // rt_pin_mode(TOUCH_IRQ_PIN, PIN_MODE_INPUT);
    rt_pin_attach_irq(TOUCH_IRQ_PIN, PIN_IRQ_MODE_FALLING, cst816_irq_handler, (void *)(rt_uint32_t)TOUCH_IRQ_PIN);
    rt_pin_irq_enable(TOUCH_IRQ_PIN, PIN_IRQ_ENABLE); // Must enable before read I2C

    LOG_D("cst812t init OK");

    return RT_EOK;
}

static rt_err_t deinit(void)
{
    LOG_D("cst816 deinit");

    rt_pin_irq_enable(TOUCH_IRQ_PIN, PIN_IRQ_DISABLE);
    rt_pin_detach_irq(TOUCH_IRQ_PIN);

    return RT_EOK;
}

rt_err_t cst8xx_probe(void)
{
    rt_err_t err;

    ft_bus = (struct rt_i2c_bus_device *)rt_device_find(TOUCH_DEVICE_NAME);
    if (RT_Device_Class_I2CBUS != ft_bus->parent.type)
    {
        ft_bus = NULL;
    }
    if (ft_bus)
    {
        rt_device_open((rt_device_t)ft_bus, RT_DEVICE_FLAG_RDWR);
    }
    else
    {
        LOG_I("bus not find\n");
        return RT_ERROR;
    }
    // {
    //     struct rt_i2c_configuration configuration =
    //     {
    //         .mode = 0,
    //         .addr = 0,
    //         .timeout = 500,
    //         .max_hz  = 100000,
    //     };
    //     i2c_bus_configure(ft_bus, &configuration);
    // }
    LOG_I("cst812t probe OK");
    return RT_EOK;
}

static struct rt_touch_ops cst8xx_touch_ops =
{
        .touch_readpoint = cst8xx_read_point,
        .touch_control = cst8xx_control,
};

int rt_hw_cst8xx_init(const char *name, struct rt_touch_config *cfg)
{
    struct rt_touch_device *touch_device = RT_NULL;
    struct rt_i2c_bus_device *i2c_bus = RT_NULL;

    touch_device = (struct rt_touch_device *)rt_malloc(sizeof(struct rt_touch_device));
    if (touch_device == RT_NULL)
    {
        LOG_E("touch device malloc fail");
        return -RT_ERROR;
    }
    rt_memset((void *)touch_device, 0, sizeof(struct rt_touch_device));

    /* hw init*/
    // rt_pin_mode(*(rt_uint8_t *)cfg->user_data, PIN_MODE_OUTPUT);
    // rt_pin_mode(cfg->irq_pin.pin, PIN_MODE_OUTPUT);
    // rt_pin_write(cfg->irq_pin.pin, PIN_LOW);
    // rt_pin_write(*(rt_uint8_t *)cfg->user_data, PIN_LOW);
    // rt_thread_mdelay(10);

    i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(cfg->dev_name);

    if (rt_device_open((rt_device_t)i2c_bus, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        LOG_E("open %s device failed", cfg->dev_name);
        return -RT_ERROR;
    }

    //@TODO: soft reset

    /* register touch device */
    touch_device->info.type = RT_TOUCH_TYPE_CAPACITANCE;
    touch_device->info.vendor = RT_TOUCH_VENDOR_UNKNOWN; // RT_TOUCH_VENDOR_CST
    rt_memcpy(&touch_device->config, cfg, sizeof(struct rt_touch_config));

    touch_device->ops = &cst8xx_touch_ops;
    rt_hw_touch_register(touch_device, name, RT_DEVICE_FLAG_INT_RX, RT_NULL);

    LOG_I("touch device cst8xx init success");

    return RT_EOK;
}

#endif /* PKG_USING_CST812T */
