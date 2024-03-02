/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2023-02-22     Rbb666        First version
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <rtconfig.h>

#define LV_USE_SYSMON           1
#define LV_USE_PERF_MONITOR     1
#define LV_COLOR_DEPTH          16

#define LV_HOR_RES_MAX          480
#define LV_VER_RES_MAX          360
#define LV_COLOR_16_SWAP        1

#define LV_USE_DRAW_DAVE2D      0

/*turn-on helium acceleration when Arm-2D and the Helium-powered device are detected */
#if defined(__ARM_FEATURE_MVE) && __ARM_FEATURE_MVE
    #define LV_USE_DRAW_SW_ASM  LV_DRAW_SW_ASM_HELIUM
    #define LV_USE_DRAW_ARM2D       1
    #define LV_USE_DRAW_ARM2D_SYNC  1
#endif

#ifdef BSP_USING_LVGL_WIDGETS_DEMO
    #define LV_USE_DEMO_WIDGETS 1
    #define LV_DEMO_WIDGETS_SLIDESHOW   0
#endif  /* BSP_USING_LVGL_WIDGETS_DEMO */

/*Benchmark your system*/
#ifdef BSP_USING_LVGL_BENCHMARK_DEMO
    #define LV_USE_DEMO_BENCHMARK 1
    /*Use RGB565A8 images with 16 bit color depth instead of ARGB8565*/
    #define LV_DEMO_BENCHMARK_RGB565A8  1
    #define LV_FONT_MONTSERRAT_14       1
    #define LV_FONT_MONTSERRAT_24       1
#endif  /* BSP_USING_LVGL_BENCHMARK_DEMO */

/*Stress test for LVGL*/
#ifdef BSP_USING_LVGL_STRESS_DEMO
    #define LV_USE_DEMO_STRESS 1
#endif  /* BSP_USING_LVGL_STRESS_DEMO */

/*Render test for LVGL*/
#ifdef BSP_USING_LVGL_RENDER_DEMO
    #define LV_USE_DEMO_RENDER 1
#endif  /* BSP_USING_LVGL_RENDER_DEMO */

/*Music player demo*/
#ifdef BSP_USING_LVGL_MUSIC_DEMO
    #define LV_USE_DEMO_MUSIC 1
    #define LV_DEMO_MUSIC_SQUARE    1
    #define LV_DEMO_MUSIC_LANDSCAPE 0
    #define LV_DEMO_MUSIC_ROUND     0
    #define LV_DEMO_MUSIC_LARGE     0
    #define LV_DEMO_MUSIC_AUTO_PLAY 1
    #define LV_FONT_MONTSERRAT_12   1
    #define LV_FONT_MONTSERRAT_16   1
#endif  /* BSP_USING_LVGL_MUSIC_DEMO */

#endif
