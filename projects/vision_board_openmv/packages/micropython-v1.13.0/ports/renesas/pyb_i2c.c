/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
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
 */

#include <stdio.h>
#include <string.h>
#include <rtdevice.h>

#include "py/mperrno.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "bufhelper.h"
#include "pyb_i2c.h"

#ifdef MICROPYTHON_USING_MACHINE_I2C

#define PYB_I2C_MASTER (0)
#define PYB_I2C_SLAVE  (1)

// use this for OwnAddress1 to configure I2C in master mode
#define PYB_I2C_MASTER_ADDRESS (0xfe)

/// \moduleref pyb
/// \class I2C - a two-wire serial protocol
///
/// I2C is a two-wire protocol for communicating between devices.  At the physical
/// level it consists of 2 wires: SCL and SDA, the clock and data lines respectively.
///
/// I2C objects are created attached to a specific bus.  They can be initialised
/// when created, or initialised later on:
///
///     from pyb import I2C
///
///     i2c = I2C(1)                         # create on bus 1
///     i2c = I2C(1, I2C.CONTROLLER)         # create and init as a controller
///     i2c.init(I2C.CONTROLLER, baudrate=20000) # init as a controller
///     i2c.init(I2C.PERIPHERAL, addr=0x42)  # init as a peripheral with given address
///     i2c.deinit()                         # turn off the I2C unit
///
/// Printing the i2c object gives you information about its configuration.
///
/// Basic methods for peripheral are send and recv:
///
///     i2c.send('abc')      # send 3 bytes
///     i2c.send(0x42)       # send a single byte, given by the number
///     data = i2c.recv(3)   # receive 3 bytes
///
/// To receive inplace, first create a bytearray:
///
///     data = bytearray(3)  # create a buffer
///     i2c.recv(data)       # receive 3 bytes, writing them into data
///
/// You can specify a timeout (in ms):
///
///     i2c.send(b'123', timeout=2000)   # timout after 2 seconds
///
/// A controller must specify the recipient's address:
///
///     i2c.init(I2C.CONTROLLER)
///     i2c.send('123', 0x42)        # send 3 bytes to peripheral with address 0x42
///     i2c.send(b'456', addr=0x42)  # keyword for address
///
/// Master also has other methods:
///
///     i2c.is_ready(0x42)           # check if peripheral 0x42 is ready
///     i2c.scan()                   # scan for peripherals on the bus, returning
///                                  #   a list of valid addresses
///     i2c.mem_read(3, 0x42, 2)     # read 3 bytes from memory of peripheral 0x42,
///                                  #   starting at address 2 in the peripheral
///     i2c.mem_write('abc', 0x42, 2, timeout=1000)
typedef struct _pyb_i2c_obj_t
{
    mp_obj_base_t base;
    struct rt_i2c_bus_device *i2c_bus;
} pyb_i2c_obj_t;

/******************************************************************************/
/* MicroPython bindings                                                       */

STATIC void pyb_i2c_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind)
{
    pyb_i2c_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "I2C(%s, timeout=%u)",
              self->i2c_bus->parent.parent.name,
              self->i2c_bus->timeout);
    return;
}

STATIC int write_mem(mp_obj_t self_in, uint16_t addr, uint32_t memaddr, uint8_t addrsize, const uint8_t *buf, size_t len)
{
    pyb_i2c_obj_t *self = (pyb_i2c_obj_t *)MP_OBJ_TO_PTR(self_in);

    // Create buffer with memory address
    size_t memaddr_len = 0;
    uint8_t memaddr_buf[4];
    for (int16_t i = addrsize - 8; i >= 0; i -= 8)
    {
        memaddr_buf[memaddr_len++] = memaddr >> i;
    }

    struct rt_i2c_msg msg[2];

    msg[0].buf = memaddr_buf;
    msg[0].len = (addrsize + 7) / 8;
    msg[0].flags = RT_I2C_WR;
    msg[0].addr = addr;

    msg[1].buf = (rt_uint8_t *)buf;
    msg[1].len = len;
    msg[1].flags = RT_I2C_WR;
    msg[1].addr = addr;

    if (rt_i2c_transfer(self->i2c_bus, msg, 2) != 2)
        return -MP_EIO;

    return len;
}

STATIC int read_mem(mp_obj_t self_in, uint16_t addr, uint32_t memaddr, uint8_t addrsize, uint8_t *buf, size_t len)
{
    pyb_i2c_obj_t *self = (pyb_i2c_obj_t *)MP_OBJ_TO_PTR(self_in);
    uint8_t memaddr_buf[4];
    size_t memaddr_len = 0;
    for (int16_t i = addrsize - 8; i >= 0; i -= 8)
    {
        memaddr_buf[memaddr_len++] = memaddr >> i;
    }

    struct rt_i2c_msg msg[2];

    msg[0].buf = memaddr_buf;
    msg[0].len = (addrsize + 7) / 8;
    msg[0].flags = RT_I2C_WR;
    msg[0].addr = addr;

    msg[1].buf = buf;
    msg[1].len = len;
    msg[1].flags = RT_I2C_RD;
    msg[1].addr = addr;

    if (rt_i2c_transfer(self->i2c_bus, msg, 2) != 2)
        return -MP_EIO;

    return len;
}

/// \classmethod \constructor(bus, ...)
///
/// Construct an I2C object on the given bus.  `bus` can be 1 or 2.
/// With no additional parameters, the I2C object is created but not
/// initialised (it has the settings from the last initialisation of
/// the bus, if any).  If extra arguments are given, the bus is initialised.
/// See `init` for parameters of initialisation.
///
/// The physical pins of the I2C buses are:
///
///   - `I2C(1)` is on the X position: `(SCL, SDA) = (X9, X10) = (PB6, PB7)`
///   - `I2C(2)` is on the Y position: `(SCL, SDA) = (Y9, Y10) = (PB10, PB11)`
STATIC mp_obj_t pyb_i2c_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args)
{
    // check arguments
    mp_arg_check_num(n_args, n_kw, 1, MP_OBJ_FUN_ARGS_MAX, true);

    char iic_device[RT_NAME_MAX];

    snprintf(iic_device, sizeof(iic_device), "i2c%d", mp_obj_get_int(args[0]));
    struct rt_i2c_bus_device *i2c_bus = rt_i2c_bus_device_find(iic_device);

    if (i2c_bus == RT_NULL)
    {
        mp_printf(&mp_plat_print, "can't find %s device\r\n", iic_device);
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "I2C(%s) doesn't exist", iic_device));
    }

    // create new hard I2C object
    pyb_i2c_obj_t *self = m_new_obj(pyb_i2c_obj_t);
    self->base.type = &pyb_i2c_type;
    self->i2c_bus = i2c_bus;
    return (mp_obj_t) self;
}

STATIC mp_obj_t pyb_i2c_init(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(pyb_i2c_init_obj, 1, pyb_i2c_init);

/// \method deinit()
/// Turn off the I2C bus.
STATIC mp_obj_t pyb_i2c_deinit(mp_obj_t self_in)
{
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(pyb_i2c_deinit_obj, pyb_i2c_deinit);

STATIC int mp_machine_i2c_writeto(mp_obj_base_t *self_in, uint16_t addr, const uint8_t *src, size_t len, bool stop)
{
    uint8_t buf[1] = {0};
    pyb_i2c_obj_t *self = MP_OBJ_TO_PTR(self_in);
    if (len == 0)
    {
        len = 1;
        if (src == NULL)
        {
            src = buf;
        }
        return rt_i2c_master_send(self->i2c_bus, addr, 0, src, len);
    }
    else if (src == NULL)
    {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "buf must not NULL"));
    }
    return rt_i2c_master_send(self->i2c_bus, addr, 0, src, len);
}

/// \method scan()
/// Scan all I2C addresses from 0x08 to 0x77 and return a list of those that respond.
/// Only valid when in controller mode.
STATIC mp_obj_t pyb_i2c_scan(mp_obj_t self_in)
{
    mp_obj_base_t *self = MP_OBJ_TO_PTR(self_in);
    mp_obj_t list = mp_obj_new_list(0, NULL);
    // 7-bit addresses 0b0000xxx and 0b1111xxx are reserved
    mp_printf(&mp_plat_print, "i2c scan addr:[");
    for (int addr = 0x08; addr < 0x78; ++addr)
    {
        int ret = mp_machine_i2c_writeto(self, addr, NULL, 0, true);
        if (ret > 0)
        {
            mp_obj_list_append(list, MP_OBJ_NEW_SMALL_INT(addr));
            mp_printf(&mp_plat_print, "%d ", addr);
        }
    }
    mp_printf(&mp_plat_print, "]\r\n");
    return list;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(pyb_i2c_scan_obj, pyb_i2c_scan);

/// \method send(send, addr=0x00, timeout=5000)
/// Send data on the bus:
///
///   - `send` is the data to send (an integer to send, or a buffer object)
///   - `addr` is the address to send to (only required in controller mode)
///   - `timeout` is the timeout in milliseconds to wait for the send
///
/// Return value: `None`.
STATIC mp_obj_t pyb_i2c_send(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)
{
    static const mp_arg_t allowed_args[] =
    {
        { MP_QSTR_send,    MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_addr,    MP_ARG_INT, {.u_int = PYB_I2C_MASTER_ADDRESS} },
        { MP_QSTR_timeout, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 5000} },
    };

    // parse args
    pyb_i2c_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // get the buffer to send from
    mp_buffer_info_t bufinfo;
    uint8_t data[1];
    pyb_buf_get_for_send(args[0].u_obj, &bufinfo, data);

    if (args[1].u_int == PYB_I2C_MASTER_ADDRESS)
    {
        mp_raise_TypeError(MP_ERROR_TEXT("addr argument required"));
    }

    mp_uint_t i2c_addr = args[1].u_int;

    rt_size_t size;
    size = rt_i2c_master_send(self->i2c_bus, i2c_addr, 0, bufinfo.buf, bufinfo.len);
    if (size == 0)
    {
        mp_raise_OSError(1);
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(pyb_i2c_send_obj, 1, pyb_i2c_send);

/// \method recv(recv, addr=0x00, timeout=5000)
///
/// Receive data on the bus:
///
///   - `recv` can be an integer, which is the number of bytes to receive,
///     or a mutable buffer, which will be filled with received bytes
///   - `addr` is the address to receive from (only required in controller mode)
///   - `timeout` is the timeout in milliseconds to wait for the receive
///
/// Return value: if `recv` is an integer then a new buffer of the bytes received,
/// otherwise the same buffer that was passed in to `recv`.
STATIC mp_obj_t pyb_i2c_recv(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)
{
    static const mp_arg_t allowed_args[] =
    {
        { MP_QSTR_recv,    MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_addr,    MP_ARG_INT, {.u_int = PYB_I2C_MASTER_ADDRESS} },
        { MP_QSTR_timeout, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 5000} },
    };

    // parse args
    pyb_i2c_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // get the buffer to receive into
    vstr_t vstr;
    mp_obj_t o_ret = pyb_buf_get_for_recv(args[0].u_obj, &vstr);

    if (args[1].u_int == PYB_I2C_MASTER_ADDRESS)
    {
        mp_raise_TypeError(MP_ERROR_TEXT("addr argument required"));
    }

    mp_uint_t i2c_addr = args[1].u_int;

    rt_i2c_master_recv(self->i2c_bus, i2c_addr, 0, (uint8_t *)vstr.buf, vstr.len);

    // return the received data
    if (o_ret != MP_OBJ_NULL)
    {
        return o_ret;
    }
    else
    {
        return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(pyb_i2c_recv_obj, 1, pyb_i2c_recv);

/// \method mem_read(data, addr, memaddr, timeout=5000, addr_size=8)
///
/// Read from the memory of an I2C device:
///
///   - `data` can be an integer or a buffer to read into
///   - `addr` is the I2C device address
///   - `memaddr` is the memory location within the I2C device
///   - `timeout` is the timeout in milliseconds to wait for the read
///   - `addr_size` selects width of memaddr: 8 or 16 bits
///
/// Returns the read data.
/// This is only valid in controller mode.
STATIC const mp_arg_t pyb_i2c_mem_read_allowed_args[] =
{
    { MP_QSTR_data,    MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
    { MP_QSTR_addr,    MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
    { MP_QSTR_memaddr, MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
    { MP_QSTR_timeout, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 5000} },
    { MP_QSTR_addr_size, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 8} },
};

STATIC mp_obj_t pyb_i2c_mem_read(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)
{
    // parse args
    pyb_i2c_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);
    mp_arg_val_t args[MP_ARRAY_SIZE(pyb_i2c_mem_read_allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(pyb_i2c_mem_read_allowed_args), pyb_i2c_mem_read_allowed_args, args);

    // get the buffer to read into
    vstr_t vstr;
    mp_obj_t o_ret = pyb_buf_get_for_recv(args[0].u_obj, &vstr);

    // get the addresses
    mp_uint_t i2c_addr = args[1].u_int;
    mp_uint_t mem_addr = args[2].u_int;
    // determine width of mem_addr; default is 8 bits, entering any other value gives 16 bit width
    mp_uint_t mem_addr_size = 1;
    if (args[4].u_int != 8)
    {
        mem_addr_size = 2;
    }

    // do the transfer
    int ret = read_mem(self, i2c_addr, mem_addr,
                       mem_addr_size, (uint8_t *)vstr.buf, vstr.len);
    if (ret < 0)
    {
        mp_raise_OSError(-ret);
    }

    // return the read data
    if (o_ret != MP_OBJ_NULL)
    {
        return o_ret;
    }
    else
    {
        return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
    }

    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(pyb_i2c_mem_read_obj, 1, pyb_i2c_mem_read);

/// \method mem_write(data, addr, memaddr, timeout=5000, addr_size=8)
///
/// Write to the memory of an I2C device:
///
///   - `data` can be an integer or a buffer to write from
///   - `addr` is the I2C device address
///   - `memaddr` is the memory location within the I2C device
///   - `timeout` is the timeout in milliseconds to wait for the write
///   - `addr_size` selects width of memaddr: 8 or 16 bits
///
/// Returns `None`.
/// This is only valid in controller mode.
STATIC mp_obj_t pyb_i2c_mem_write(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)
{
    // parse args (same as mem_read)
    // parse args (same as mem_read)
    pyb_i2c_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);
    mp_arg_val_t args[MP_ARRAY_SIZE(pyb_i2c_mem_read_allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(pyb_i2c_mem_read_allowed_args), pyb_i2c_mem_read_allowed_args, args);

    // get the buffer to write from
    mp_buffer_info_t bufinfo;
    uint8_t data[1];
    pyb_buf_get_for_send(args[0].u_obj, &bufinfo, data);

    // get the addresses
    mp_uint_t i2c_addr = args[1].u_int;
    mp_uint_t mem_addr = args[2].u_int;
    // determine width of mem_addr; default is 8 bits, entering any other value gives 16 bit width
    mp_uint_t mem_addr_size = 1;
    if (args[4].u_int != 8)
    {
        mem_addr_size = 2;
    }

    // do the transfer
    int ret = write_mem(self, i2c_addr, mem_addr,
                        mem_addr_size, bufinfo.buf, bufinfo.len);
    if (ret < 0)
    {
        mp_raise_OSError(-ret);
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(pyb_i2c_mem_write_obj, 1, pyb_i2c_mem_write);

STATIC const mp_rom_map_elem_t pyb_i2c_locals_dict_table[] =
{
    // instance methods
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&pyb_i2c_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_deinit), MP_ROM_PTR(&pyb_i2c_deinit_obj) },
    { MP_ROM_QSTR(MP_QSTR_scan), MP_ROM_PTR(&pyb_i2c_scan_obj) },
    { MP_ROM_QSTR(MP_QSTR_send), MP_ROM_PTR(&pyb_i2c_send_obj) },
    { MP_ROM_QSTR(MP_QSTR_recv), MP_ROM_PTR(&pyb_i2c_recv_obj) },
    { MP_ROM_QSTR(MP_QSTR_mem_read), MP_ROM_PTR(&pyb_i2c_mem_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_mem_write), MP_ROM_PTR(&pyb_i2c_mem_write_obj) },

    // class constants
    /// \constant CONTROLLER - for initialising the bus to controller mode
    /// \constant PERIPHERAL - for initialising the bus to peripheral mode
    { MP_ROM_QSTR(MP_QSTR_CONTROLLER), MP_ROM_INT(PYB_I2C_MASTER) },
    { MP_ROM_QSTR(MP_QSTR_PERIPHERAL),  MP_ROM_INT(PYB_I2C_SLAVE) },
    // TODO - remove MASTER/SLAVE when CONTROLLER/PERIPHERAL gain wide adoption
    { MP_ROM_QSTR(MP_QSTR_MASTER), MP_ROM_INT(PYB_I2C_MASTER) },
    { MP_ROM_QSTR(MP_QSTR_SLAVE), MP_ROM_INT(PYB_I2C_SLAVE) },
};

STATIC MP_DEFINE_CONST_DICT(pyb_i2c_locals_dict, pyb_i2c_locals_dict_table);

const mp_obj_type_t pyb_i2c_type =
{
    { &mp_type_type },
    .name = MP_QSTR_I2C,
    .print = pyb_i2c_print,
    .make_new = pyb_i2c_make_new,
    .locals_dict = (mp_obj_dict_t *) &pyb_i2c_locals_dict,
};

#endif // MICROPY_PY_PYB_LEGACY && MICROPY_HW_ENABLE_HW_I2C
