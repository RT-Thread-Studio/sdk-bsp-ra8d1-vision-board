/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Armink (armink.ztl@gmail.com)
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

#ifndef _MODMACHINE_H
#define _MODMACHINE_H

#include "py/obj.h"
#include <rtthread.h>

extern const mp_obj_type_t machine_pin_type;

MP_DECLARE_CONST_FUN_OBJ_VAR_BETWEEN(machine_info_obj);
MP_DECLARE_CONST_FUN_OBJ_0(machine_unique_id_obj);
MP_DECLARE_CONST_FUN_OBJ_0(machine_reset_obj);
MP_DECLARE_CONST_FUN_OBJ_0(machine_bootloader_obj);
MP_DECLARE_CONST_FUN_OBJ_0(machine_freq_obj);
MP_DECLARE_CONST_FUN_OBJ_0(pyb_wfi_obj);
MP_DECLARE_CONST_FUN_OBJ_0(pyb_disable_irq_obj);
MP_DECLARE_CONST_FUN_OBJ_VAR_BETWEEN(pyb_enable_irq_obj);
MP_DECLARE_CONST_FUN_OBJ_0(machine_sleep_obj);
MP_DECLARE_CONST_FUN_OBJ_0(machine_deepsleep_obj);

typedef struct _machine_pin_obj_t {
    mp_obj_base_t base;
    char name[RT_NAME_MAX];
    uint32_t pin;
    mp_obj_t pin_isr_cb;
} machine_pin_obj_t;

#endif // _MODMACHINE_H
