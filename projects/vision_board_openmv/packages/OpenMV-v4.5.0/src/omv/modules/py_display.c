/*
 * This file is part of the OpenMV project.
 *
 * Copyright (c) 2013-2019 Ibrahim Abdelkader <iabdalkader@openmv.io>
 * Copyright (c) 2013-2019 Kwabena W. Agyeman <kwagyeman@openmv.io>
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * LCD Python module.
 */
#include <py/objstr.h>
#include "py/runtime.h"
#include <imlib.h>
#include "fb_alloc.h"
#include "py_assert.h"
#include "py_helper.h"
#include "py_image.h"
#include "omv_lcd.h"

static int width = 0;
static int height = 0;
static enum { LCD_NONE, LCD_SHIELD } type = LCD_NONE;
static bool backlight_init = false;

static ovmhal_lcd_t lcddev;

static mp_obj_t py_lcd_deinit(void)
{
    switch (type)
    {
    case LCD_NONE:
        return mp_const_none;
    case LCD_SHIELD:
        width = 0;
        height = 0;
        type = LCD_NONE;
        omvhal_lcd_deinit(&lcddev);
        if (backlight_init)
        {
            backlight_init = false;
        }
        return mp_const_none;
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(py_lcd_deinit_obj, py_lcd_deinit);

static mp_obj_t py_lcd_init(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    py_lcd_deinit();

    switch (py_helper_keyword_int(n_args, args, 0, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_type), LCD_SHIELD))
    {
    case LCD_NONE:
        return mp_const_none;
    case LCD_SHIELD:
    {
        type = LCD_SHIELD;

        omvhal_lcd_init(&lcddev, type, &width, &height);

        return mp_const_none;
    }
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(py_lcd_init_obj, 0, py_lcd_init);

static mp_obj_t py_lcd_width()
{
    if (type == LCD_NONE) return mp_const_none;
    return mp_obj_new_int(width);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(py_lcd_width_obj, py_lcd_width);

static mp_obj_t py_lcd_height()
{
    if (type == LCD_NONE) return mp_const_none;
    return mp_obj_new_int(height);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(py_lcd_height_obj, py_lcd_height);

static mp_obj_t py_lcd_type()
{
    if (type == LCD_NONE) return mp_const_none;
    return mp_obj_new_int(type);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(py_lcd_type_obj, py_lcd_type);

static mp_obj_t py_lcd_set_backlight(mp_obj_t state_obj)
{
    switch (type)
    {
    case LCD_NONE:
        return mp_const_none;
    case LCD_SHIELD:
    {
        bool bit = !!mp_obj_get_int(state_obj);
        if (!backlight_init)
        {

            backlight_init = true;
        }
        omvhal_set_backlight(&lcddev, bit);
        return mp_const_none;
    }
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(py_lcd_set_backlight_obj, py_lcd_set_backlight);

static mp_obj_t py_lcd_get_backlight()
{
    switch (type)
    {
    case LCD_NONE:
        return mp_const_none;
    case LCD_SHIELD:
        if (!backlight_init)
        {
            return mp_const_none;
        }
        return mp_obj_new_int(0);
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(py_lcd_get_backlight_obj, py_lcd_get_backlight);

STATIC mp_obj_t py_lcd_display(uint n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    image_t *arg_img = py_image_cobj(args[0]);

    int arg_x_off = 0;
    int arg_y_off = 0;
    uint offset = 1;
    if (n_args > 1)
    {
        if (MP_OBJ_IS_TYPE(args[1], &mp_type_tuple) || MP_OBJ_IS_TYPE(args[1], &mp_type_list))
        {
            mp_obj_t *arg_vec;
            mp_obj_get_array_fixed_n(args[1], 2, &arg_vec);
            arg_x_off = mp_obj_get_int(arg_vec[0]);
            arg_y_off = mp_obj_get_int(arg_vec[1]);
            offset = 2;
        }
        else if (n_args > 2)
        {
            arg_x_off = mp_obj_get_int(args[1]);
            arg_y_off = mp_obj_get_int(args[2]);
            offset = 3;
        }
        else if (n_args > 1)
        {
            mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Expected x and y offset!"));
        }
    }

    rectangle_t arg_roi;
    py_helper_keyword_rectangle_roi(arg_img, n_args, args, offset + 2, kw_args, &arg_roi);

    int arg_rgb_channel = py_helper_keyword_int(n_args, args, offset + 3, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_rgb_channel), -1);
    if ((arg_rgb_channel < -1) || (2 < arg_rgb_channel))
    {
        mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("-1 <= rgb_channel <= 2!"));
    }

    int arg_alpha = py_helper_keyword_int(n_args, args, offset + 4, kw_args, MP_OBJ_NEW_QSTR(MP_QSTR_alpha), 256);
    if ((arg_alpha < 0) || (256 < arg_alpha))
    {
        mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("0 <= alpha <= 256!"));
    }

    fb_alloc_mark();

    omvhal_lcd_draw(&lcddev, arg_x_off, arg_y_off, arg_img->w, arg_img->h, (uint16_t *)(arg_img->pixels));

    fb_alloc_free_till_mark();

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(py_lcd_display_obj, 1, py_lcd_display);

static mp_obj_t py_lcd_clear(uint n_args, const mp_obj_t *args)
{
    switch (type)
    {
    case LCD_NONE:
        return mp_const_none;
    case LCD_SHIELD:
        omvhal_lcd_clear(&lcddev, 0);
        return mp_const_none;
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(py_lcd_clear_obj, 0, 1, py_lcd_clear);


static const mp_map_elem_t globals_dict_table[] =
{
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__),        MP_OBJ_NEW_QSTR(MP_QSTR_lcd) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_init), (mp_obj_t) &py_lcd_init_obj          },
    { MP_OBJ_NEW_QSTR(MP_QSTR_deinit), (mp_obj_t) &py_lcd_deinit_obj        },
    { MP_OBJ_NEW_QSTR(MP_QSTR_width), (mp_obj_t) &py_lcd_width_obj         },
    { MP_OBJ_NEW_QSTR(MP_QSTR_height), (mp_obj_t) &py_lcd_height_obj        },
    { MP_OBJ_NEW_QSTR(MP_QSTR_type), (mp_obj_t) &py_lcd_type_obj          },
    { MP_OBJ_NEW_QSTR(MP_QSTR_set_backlight), (mp_obj_t) &py_lcd_set_backlight_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_get_backlight), (mp_obj_t) &py_lcd_get_backlight_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_display), (mp_obj_t) &py_lcd_display_obj       },
    { MP_OBJ_NEW_QSTR(MP_QSTR_clear), (mp_obj_t) &py_lcd_clear_obj         },
    { NULL, NULL },
};
STATIC MP_DEFINE_CONST_DICT(globals_dict, globals_dict_table);

const mp_obj_module_t lcd_module =
{
    .base = { &mp_type_module },
    .globals = (mp_obj_t) &globals_dict,
};
