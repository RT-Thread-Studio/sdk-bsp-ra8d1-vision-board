#include <stdio.h>
#include <stdbool.h>
#include "omv_i2c.h"
#include "cam_boardconfig.h"
#include "board.h"

int omv_i2c_init(omv_i2c_t *bus, uint32_t bus_id, uint32_t speed)
{
    bus->id = bus_id;
    bus->speed = speed;
    bus->inst = (struct rt_i2c_bus_device *)rt_device_find(OMV_CAM_BUS_NAME);
    bus->scl_pin = (omv_gpio_t)
    {
        OMV_CAM_BUS_SCL_PIN
    };
    bus->sda_pin = (omv_gpio_t)
    {
        OMV_CAM_BUS_SDA_PIN
    };
    bus->initialized = true;

    return 0;
}

int omv_i2c_deinit(omv_i2c_t *bus)
{
    bus->inst = NULL;
    bus->scl_pin = (omv_gpio_t)
    {
        0
    };
    bus->sda_pin = (omv_gpio_t)
    {
        0
    };
    bus->initialized = false;

    return 0;
}

static int i2c_probe(omv_i2c_t *bus, char addr)
{
    unsigned char cmd[1];
    cmd[0] = 0;

    struct rt_i2c_msg msgs;
    msgs.addr = addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf = cmd;
    msgs.len = 0;

    return rt_i2c_transfer(bus->inst, &msgs, 1);
}

int omv_i2c_scan(omv_i2c_t *i2c, uint8_t *list, uint8_t size)
{
    int idx = 0;
    for (uint8_t addr = 0x15; addr <= 0x77; addr++)
    {
        if (i2c_probe(i2c, addr) == 1)
        {
            rt_kprintf("i2c probe camera id:0x%02X\n", addr << 1);

            if (list == NULL || size == 0)
            {
                return (addr << 1);
            }
            else if (idx < size)
            {
                list[idx++] = (addr << 1);
            }
            else
            {
                break;
            }
        }
    }
    return idx;
}

int omv_i2c_enable(omv_i2c_t *bus, bool enable)
{
    return RT_EOK;
}

int omv_i2c_gencall(omv_i2c_t *bus, uint8_t cmd)
{
    int bytes = 0;
    bytes += rt_i2c_master_send(bus->inst, 0x00, 0, &cmd, 1);
    return (bytes == 1) ? 0 : -1;
}

int omv_i2c_readb(omv_i2c_t *bus, uint8_t slv_addr, uint8_t reg_addr, uint8_t *reg_data)
{
    int bytes = 0;
    slv_addr = slv_addr >> 1;

    bytes += rt_i2c_master_send(bus->inst, slv_addr, RT_I2C_WR, &reg_addr, 1);
    bytes += rt_i2c_master_recv(bus->inst, slv_addr, RT_I2C_RD, reg_data, 1);

    return (bytes == 2) ? 0 : -1;
}

int omv_i2c_writeb(omv_i2c_t *bus, uint8_t slv_addr, uint8_t reg_addr, uint8_t reg_data)
{
    int bytes = 0;
    slv_addr = slv_addr >> 1;

    uint8_t buf[] = {reg_addr, reg_data};
    bytes = rt_i2c_master_send(bus->inst, slv_addr, RT_I2C_WR, buf, 2);

    return (bytes == 2) ? 0 : -1;
}

int omv_i2c_readb2(omv_i2c_t *bus, uint8_t slv_addr, uint16_t reg_addr, uint8_t *reg_data)
{
    int bytes = 0;
    slv_addr = slv_addr >> 1;

    uint8_t buf[] = {(reg_addr >> 8), reg_addr};
    bytes += rt_i2c_master_send(bus->inst, slv_addr, RT_I2C_WR, buf, 2);
    bytes += rt_i2c_master_recv(bus->inst, slv_addr, RT_I2C_RD, reg_data, 1);

    return (bytes == 3) ? 0 : -1;
}

int omv_i2c_writeb2(omv_i2c_t *bus, uint8_t slv_addr, uint16_t reg_addr, uint8_t reg_data)
{
    int bytes = 0;
    slv_addr = slv_addr >> 1;

    uint8_t buf[] = {(reg_addr >> 8), reg_addr, reg_data};
    bytes = rt_i2c_master_send(bus->inst, slv_addr, RT_I2C_WR, buf, 3);

    return (bytes == 3) ? 0 : -1;
}

int omv_i2c_readw(omv_i2c_t *bus, uint8_t slv_addr, uint8_t reg_addr, uint16_t *reg_data)
{
    int bytes = 0;
    slv_addr = slv_addr >> 1;

    bytes += rt_i2c_master_send(bus->inst, slv_addr, RT_I2C_WR, &reg_addr, 1);
    bytes += rt_i2c_master_recv(bus->inst, slv_addr, RT_I2C_RD, (uint8_t *)reg_data, 2);
    *reg_data = (*reg_data << 8) | (*reg_data >> 8);

    return (bytes == 3) ? 0 : -1;
}

int omv_i2c_writew(omv_i2c_t *bus, uint8_t slv_addr, uint8_t reg_addr, uint16_t reg_data)
{
    int bytes = 0;
    slv_addr = slv_addr >> 1;

    uint8_t buf[] = {reg_addr, (reg_data >> 8), reg_data};
    bytes = rt_i2c_master_send(bus->inst, slv_addr, RT_I2C_WR, buf, 3);

    return (bytes == 3) ? 0 : -1;
}

int omv_i2c_read_bytes(omv_i2c_t *bus, uint8_t slv_addr, uint8_t *buf, int len, uint32_t flags)
{
    int bytes = 0;
    slv_addr = slv_addr >> 1;
    uint16_t nostop = RT_I2C_RD;

    if (flags & OMV_I2C_XFER_NO_STOP)
    {
        nostop |= RT_I2C_NO_STOP;
    }

    bytes = rt_i2c_master_recv(bus->inst, slv_addr, nostop, buf, len);

    return (bytes == len) ? 0 : -1;
}

// OV5640 | MT9M114
int omv_i2c_write_bytes(omv_i2c_t *bus, uint8_t slv_addr, uint8_t *buf, int len, uint32_t flags)
{
    int bytes = 0;
    slv_addr = slv_addr >> 1;
    uint16_t nostop = RT_I2C_WR;

    if (flags & OMV_I2C_XFER_NO_STOP)
    {
        nostop |= RT_I2C_NO_STOP;
    }

    bytes = rt_i2c_master_send(bus->inst, slv_addr, nostop, buf, len);

    return (bytes == len) ? 0 : -1;
}

int cambus_pulse_scl(omv_i2c_t *bus)
{
    return 0;
}
