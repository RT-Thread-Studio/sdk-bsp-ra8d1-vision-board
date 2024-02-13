// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// $Log:$
//
// DESCRIPTION:
//	DOOM graphics stuff for X11, UNIX.
//
//-----------------------------------------------------------------------------
#include "config.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_event.h"
#include "d_main.h"
#include "i_video.h"
#include "z_zone.h"

#include "tables.h"
#include "doomkeys.h"

#include <stdbool.h>
#include <stdlib.h>

#include <fcntl.h>

#include <stdarg.h>

#include <sys/types.h>

#include <lcd_port.h>
#include <hal_data.h>

void I_GetEvent(void);

// The screen buffer; this is modified to draw things to the screen

byte *I_VideoBuffer = NULL;

// If true, game is running as a screensaver

boolean screensaver_mode = false;

// Flag indicating whether the screen is currently visible:
// when the screen isnt visible, don't render the screen

boolean screenvisible;

// Mouse acceleration
//
// This emulates some of the behavior of DOS mouse drivers by increasing
// the speed when the mouse is moved fast.
//
// The mouse input values are input directly to the game, but when
// the values exceed the value of mouse_threshold, they are multiplied
// by mouse_acceleration to increase the speed.

float mouse_acceleration = 2.0;
int mouse_threshold = 10;

// Gamma correction level to use

int usegamma = 0;

int usemouse = 0;

// If true, keyboard mapping is ignored, like in Vanilla Doom.
// The sensible thing to do is to disable this if you have a non-US
// keyboard.

int vanilla_keyboard_mapping = true;

typedef struct
{
    byte r;
    byte g;
    byte b;
} col_t;

// Palette converted to RGB565

static uint16_t rgb565_palette[256];

static rt_device_t lcd_device = RT_NULL;
static struct rt_device_graphic_info info;

void I_InitGraphics(void)
{
	rt_err_t result = RT_EOK;

    lcd_device = rt_device_find("lcd");
    RT_ASSERT(lcd_device != RT_NULL);

    result = rt_device_open(lcd_device, 0);
    RT_ASSERT(result == RT_EOK);

    /* get framebuffer address */
    result = rt_device_control(lcd_device, RTGRAPHIC_CTRL_GET_INFO, &info);
    RT_ASSERT(result == RT_EOK);

    I_VideoBuffer = (byte *)Z_Malloc(SCREENWIDTH * SCREENHEIGHT, PU_STATIC, NULL);

    screenvisible = true;
}

void I_ShutdownGraphics(void)
{
    Z_Free(I_VideoBuffer);
}

void I_StartFrame(void)
{
}

void I_StartTic(void)
{
//    I_GetEvent();
}

void I_UpdateNoBlit(void)
{
}

static inline void I_CopyFrameBufferRGB565(void)
{
    int x, y, pixel;
    byte index;

    uint16_t *fb16_t = (uint16_t *)info.framebuffer;
    for (y = 0; y < SCREENHEIGHT; y++)
    {
        for (x = 0; x < SCREENWIDTH; x++)
        {
            pixel = ((y * 2) * (SCREENWIDTH * 2)) + (x * 2);
            index = I_VideoBuffer[y * SCREENWIDTH + x];

            fb16_t[pixel] = rgb565_palette[index];
            fb16_t[pixel + 1] = rgb565_palette[index];

            fb16_t[(SCREENWIDTH * 2) + pixel] = rgb565_palette[index];
            fb16_t[(SCREENWIDTH * 2) + pixel + 1] = rgb565_palette[index];
        }
    }

    lcd_draw_jpg(0, 0, (uint8_t *)fb16_t, 640, 400);
}

//
// I_FinishUpdate
//
void I_FinishUpdate(void)
{
    I_CopyFrameBufferRGB565();
}

//
// I_ReadScreen
//
void I_ReadScreen(byte *scr)
{
    memcpy(scr, I_VideoBuffer, SCREENWIDTH * SCREENHEIGHT);
}

//
// I_SetPalette
//
void I_SetPalette(byte *palette)
{
    for (int i = 0; i < 256; i++)
    {
        uint16_t color = RGB565(palette[0], palette[1], palette[2]);
        rgb565_palette[i] = color;
        palette += 3;
    }
}

// Given an RGB value, find the closest matching palette index.
int I_GetPaletteIndex(int r, int g, int b)
{
    int best, best_diff, diff;
    int i;
    col_t color;

    best = 0;
    best_diff = INT_MAX;

    for (i = 0; i < 256; ++i)
    {
        color.r = RGB565_R(rgb565_palette[i]);
        color.g = RGB565_G(rgb565_palette[i]);
        color.b = RGB565_B(rgb565_palette[i]);

        diff = (r - color.r) * (r - color.r) + (g - color.g) * (g - color.g) + (b - color.b) * (b - color.b);

        if (diff < best_diff)
        {
            best = i;
            best_diff = diff;
        }

        if (diff == 0)
        {
            break;
        }
    }

    return best;
}

void I_BeginRead(void)
{
}

void I_EndRead(void)
{
}

void I_SetWindowTitle(char *title)
{

}

void I_GraphicsCheckCommandLine(void)
{
}

void I_SetGrabMouseCallback(grabmouse_callback_t func)
{
}

void I_EnableLoadingDisk(void)
{
}

void I_BindVideoVariables(void)
{
}

void I_DisplayFPSDots(boolean dots_on)
{
}

void I_CheckIsScreensaver(void)
{
}
