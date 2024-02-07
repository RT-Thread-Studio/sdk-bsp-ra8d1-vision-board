/* Copyright 2018 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS},
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "omv_boardconfig.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "omv_i2c.h"
#include "sensor.h"
#include "gc0328.h"
#include "gc0328_regs.h"
#include "py/mphal.h"

/** The default register settings**/
uint8_t sensor_default_regs[][2] =
{
    {0xfe, 0x80},   //reset
    {0xfe, 0x80},   //reset
    {0xfc, 0x16},   //set digital clock
    {0xfc, 0x16},   //set digital clock
    {0xfc, 0x16},   //set digital clock
    {0xfc, 0x16},   //set digital clock

    {0xFE, 0x00},   //select page0
    {0x4F, 0x00},   //AEC disable
    {0x42, 0x00},   //ABW disable
    {0x03, 0x00},   //clear Exposure time MSB[11:8]
    {0x04, 0xC0},   //clear Exposure time LSB[7:0]
    {0x77, 0x62},   //set AWB red gain
    {0x78, 0x40},   //set AWB green gain
    {0x79, 0x4D},   //set AWB blue gain

    {0x05, 0x00},   // HB high: [3:0] HBLANK high bit[11:8]
    {0x06, 0x80},   // HB low: HBLANK low bit[7:0]
    {0x07, 0x00},   // VB high: [3:0] VBLANK high bit[11:8]
    {0x08, 0x10},   // VB low: VBLANK low bit[7:0]

    {0xfe, 0x01},
    {0x29, 0x00},
    {0x2a, 0x96},

    // Exp level
    {0x2b, 0x02},    // AEC_exp_level_0
    {0x2c, 0xA0},
    {0x2D, 0x03},    // AEC_exp_level_1
    {0x2E, 0x00},
    {0x2F, 0x03},    // AEC_exp_level_2
    {0x30, 0xC0},
    {0x31, 0x05},    // AEC_exp_level_3
    {0x32, 0x40},
    {0xFE, 0x00},    // page 0

    {0xfe, 0x01},
    {0x4f, 0x00},
    {0x4c, 0x01},
    {0xfe, 0x00},
    //////////////////////////////
    ///////////AWB///////////
    ////////////////////////////////
    {0xfe, 0x01},
    {0x51, 0x80},
    {0x52, 0x12},
    {0x53, 0x80},
    {0x54, 0x60},
    {0x55, 0x01},
    {0x56, 0x06},
    {0x5b, 0x02},
    {0x61, 0xdc},
    {0x62, 0xdc},
    {0x7c, 0x71},
    {0x7d, 0x00},
    {0x76, 0x00},
    {0x79, 0x20},
    {0x7b, 0x00},
    {0x70, 0xFF},
    {0x71, 0x00},
    {0x72, 0x10},
    {0x73, 0x40},
    {0x74, 0x40},

    {0x50, 0x00},
    {0xfe, 0x01},
    {0x4f, 0x00},
    {0x4c, 0x01},
    {0x4f, 0x00},
    {0x4f, 0x00},
    {0x4f, 0x00},
    {0x4d, 0x36},
    {0x4e, 0x02},
    {0x4e, 0x02},
    {0x4d, 0x44},
    {0x4e, 0x02},
    {0x4e, 0x02},
    {0x4e, 0x02},
    {0x4e, 0x02},
    {0x4d, 0x53},
    {0x4e, 0x08},
    {0x4e, 0x08},
    {0x4e, 0x02},
    {0x4d, 0x63},
    {0x4e, 0x08},
    {0x4e, 0x08},
    {0x4d, 0x73},
    {0x4e, 0x20},
    {0x4d, 0x83},
    {0x4e, 0x20},
    {0x4f, 0x01},

    {0x50, 0x88},
    {0xfe, 0x00},

    ////////////////////////////////////////////////
    ////////////     BLK      //////////////////////
    ////////////////////////////////////////////////
    {0x27, 0x00},
    {0x2a, 0x40},
    {0x2b, 0x40},
    {0x2c, 0x40},
    {0x2d, 0x40},

    //////////////////////////////////////////////
    ////////// page  0    ////////////////////////
    //////////////////////////////////////////////
    {0xfe, 0x00},
    {0x0d, 0x01},
    {0x0e, 0xe8},
    {0x0f, 0x02},
    {0x10, 0x88},
    {0x09, 0x00},
    {0x0a, 0x00},
    {0x0b, 0x00},
    {0x0c, 0x00},
    {0x16, 0x00},   // Analog gain: [7] Analog gain enable
    {0x17, 0x16},   // Mirror Flip
    {0x18, 0x0e},
    {0x19, 0x06},

    {0x1b, 0x48},
    {0x1f, 0xC8},
    {0x20, 0x01},
    {0x21, 0x78},
    {0x22, 0xb0},
    {0x23, 0x04},
    {0x24, 0x11},   //output pin abilityof driver config
    {0x26, 0x00},

    //crop mode | It should be disable
    {0x50, 0x01},
    {0x51, 0x00},
    {0x52, 0x00},
    {0x53, 0x00},
    {0x54, 0x00},
    {0x55, 0x00},
    {0x56, 0xf0},
    {0x57, 0x01},
    {0x58, 0x40},
    {0x59, 0x22},
    {0x5a, 0x03},
    {0x5b, 0x00},
    {0x5c, 0x00},
    {0x5d, 0x00},
    {0x5e, 0x00},
    {0x5f, 0x00},
    {0x60, 0x00},
    {0x61, 0x00},
    {0x62, 0x00},

    //global gain for range
    {0x70, 0x85},

    ////////////////////////////////////////////////
    ////////////     block enable      /////////////
    ////////////////////////////////////////////////
    {0x40, 0x7f},
    {0x41, 0x26},
    {0x42, 0xff},
    {0x45, 0x00},
    {0x44, 0x26},
    {0x46, 0x03},

    {0x4b, 0x01},
    {0x50, 0x01},

    //DN & EEINTP
    {0x7e, 0x0a},
    {0x7f, 0x03},
    {0x80, 0x27},
    {0x81, 0x15},
    {0x82, 0x90},
    {0x83, 0x02},
    {0x84, 0x23},
    {0x90, 0x2c},
    {0x92, 0x02},
    {0x94, 0x02},
    {0x95, 0x35},

    ///////YCP
    {0xd1, 0x32},
    {0xd2, 0x32},
    {0xdd, 0x18},
    {0xde, 0x32},
    {0xe4, 0x88},
    {0xe5, 0x40},
    {0xd7, 0x0e},

    //ABB

    /////////////////////////////
    //////////////// GAMMA //////
    /////////////////////////////

    //GC0328_RGB_Gamma
    {0xFe, 0x00},
    {0xBF, 0x10},
    {0xC0, 0x1C},
    {0xC1, 0x33},
    {0xC2, 0x48},
    {0xC3, 0x5A},
    {0xC4, 0x6B},
    {0xC5, 0x7B},
    {0xC6, 0x95},
    {0xC7, 0xAB},
    {0xC8, 0xBF},
    {0xC9, 0xCD},
    {0xCA, 0xD9},
    {0xCB, 0xE3},
    {0xCC, 0xEB},
    {0xCD, 0xF7},
    {0xCE, 0xFD},
    {0xCF, 0xFF},

    ///Y gamma
    {0xfe, 0x00},
    {0x63, 0x00},
    {0x64, 0x05},
    {0x65, 0x0C},
    {0x66, 0x1A},
    {0x67, 0x29},
    {0x68, 0x39},
    {0x69, 0x4B},
    {0x6A, 0x5E},
    {0x6B, 0x82},
    {0x6C, 0xA4},
    {0x6D, 0xC5},
    {0x6E, 0xE5},
    {0x6F, 0xff},

    //////ASDE
    {0xfe, 0x01},
    {0x18, 0x02},
    {0xfe, 0x00},
    {0x98, 0x00},
    {0x9b, 0x20},
    {0x9c, 0x80},
    {0xa4, 0x10},
    {0xa8, 0xB0},
    {0xaa, 0x40},
    {0xa2, 0x23},
    {0xad, 0x01},

    //////////////////////////////////////////////
    ////////// AEC    ////////////////////////
    //////////////////////////////////////////////
    {0xfe, 0x01},
    {0x9c, 0x02},
    {0x08, 0xa0},
    {0x09, 0xe8},

    {0x10, 0x00},
    {0x11, 0x11},
    {0x12, 0x10},
    {0x13, 0x80},
    {0x15, 0xFC},
    {0x18, 0x03},
    {0x21, 0xC0},
    {0x22, 0x60},
    {0x23, 0x30},
    {0x25, 0x00},
    {0x24, 0x14},

    //////////////////////////////////////
    ////////////LSC//////////////////////
    //////////////////////////////////////
    //gc0328 Alight lsc reg setting list
    {0xfe, 0x01},
    {0xc0, 0x10},
    {0xc1, 0x0c},
    {0xc2, 0x0a},
    {0xc6, 0x0e},
    {0xc7, 0x0b},
    {0xc8, 0x0a},
    {0xba, 0x26},
    {0xbb, 0x1c},
    {0xbc, 0x1d},
    {0xb4, 0x23},
    {0xb5, 0x1c},
    {0xb6, 0x1a},
    {0xc3, 0x00},
    {0xc4, 0x00},
    {0xc5, 0x00},
    {0xc9, 0x00},
    {0xca, 0x00},
    {0xcb, 0x00},
    {0xbd, 0x00},
    {0xbe, 0x00},
    {0xbf, 0x00},
    {0xb7, 0x07},
    {0xb8, 0x05},
    {0xb9, 0x05},
    {0xa8, 0x07},
    {0xa9, 0x06},
    {0xaa, 0x00},
    {0xab, 0x04},
    {0xac, 0x00},
    {0xad, 0x02},
    {0xae, 0x0d},
    {0xaf, 0x05},
    {0xb0, 0x00},
    {0xb1, 0x07},
    {0xb2, 0x03},
    {0xb3, 0x00},
    {0xa4, 0x00},
    {0xa5, 0x00},
    {0xa6, 0x00},
    {0xa7, 0x00},
    {0xa1, 0x3c},
    {0xa2, 0x50},
    {0xfe, 0x00},

    ///cct
    {0xB1, 0x04},
    {0xB2, 0xfd},
    {0xB3, 0xfc},
    {0xB4, 0xf0},
    {0xB5, 0x05},
    {0xB6, 0xf0},

    {0xfe, 0x00},
    {0x27, 0xf7},
    {0x28, 0x7F},
    {0x29, 0x20},
    {0x33, 0x20},
    {0x34, 0x20},
    {0x35, 0x20},
    {0x36, 0x20},
    {0x32, 0x08},

    {0x47, 0x00},
    {0x48, 0x00},

    {0xfe, 0x01},
    {0x79, 0x00},
    {0x7d, 0x00},
    {0x50, 0x88},
    {0x5b, 0x0c},
    {0x76, 0x8f},
    {0x80, 0x70},
    {0x81, 0x70},
    {0x82, 0xb0},
    {0x70, 0xff},
    {0x71, 0x00},
    {0x72, 0x28},
    {0x73, 0x0b},
    {0x74, 0x0b},

    {0xfe, 0x00},
    {0x70, 0x45},
    {0x4f, 0x01},
    {0xf1, 0x07},
    {0xf2, 0x01},

    {0x00, 0x00},
};

static const uint8_t vga_config[][2] =
{
    {0xfe, 0x00},
    {0x4b, 0x8b},
    {0x50, 0x01},
    {0x51, 0x00},
    {0x52, 0x00},
    {0x53, 0x00},
    {0x54, 0x00},
    {0x55, 0x01},
    {0x56, 0xe0},
    {0x57, 0x02},
    {0x58, 0x80},
    {0x59, 0x11},
    {0x5a, 0x02},
    {0x5b, 0x00},
    {0x5c, 0x00},
    {0x5d, 0x00},
    {0x5e, 0x00},
    {0x5f, 0x00},
    {0x60, 0x00},
    {0x61, 0x00},
    {0x62, 0x00},
};

static const uint8_t qvga_config[][2] =
{
    {0xFE, 0x00},
    // window
    // windowing mode
    {0x09, 0x00},
    {0x0A, 0x00},
    {0x0B, 0x00},
    {0x0C, 0x00},
    {0x0D, 0x01},
    {0x0E, 0xE8},
    {0x0F, 0x02},
    {0x10, 0x88},
    //crop mode
    {0x50, 0x01},
    // {0x51, 0x00},
    // {0x52, 0x78},
    // {0x53, 0x00},
    // {0x54, 0xa0},
    // {0x55, 0x00},
    // {0x56, 0xf0},
    // {0x57, 0x01},
    // {0x58, 0x40},
    //subsample 1/2
    {0x59, 0x22},
    {0x5a, 0x00},
    {0x5b, 0x00},
    {0x5c, 0x00},
    {0x5d, 0x00},
    {0x5e, 0x00},
    {0x5f, 0x00},
    {0x60, 0x00},
    {0x61, 0x00},
    {0x62, 0x00},

    {0x00, 0x00}
};

static const uint8_t qqvga_config[][2] =
{
    {0xFE, 0x00},
    // window
    //windowing mode
    {0x09, 0x00},
    {0x0A, 0x00},
    {0x0B, 0x00},
    {0x0C, 0x00},
    {0x0D, 0x01},
    {0x0E, 0xE8},
    {0x0F, 0x02},
    {0x10, 0x88},
    //crop mode
    {0x50, 0x01},
    {0x51, 0x00},
    {0x52, 0x00},
    {0x53, 0x00},
    {0x54, 0x00},
    {0x55, 0x00},
    {0x56, 0x78},
    {0x57, 0x00},
    {0x58, 0xA0},
    //subsample 1/4
    {0x59, 0x44},
    {0x5a, 0x03},
    {0x5b, 0x00},
    {0x5c, 0x00},
    {0x5d, 0x00},
    {0x5e, 0x00},
    {0x5f, 0x00},
    {0x60, 0x00},
    {0x61, 0x00},
    {0x62, 0x00},

    {0x00, 0x00}
};

static const uint8_t B240X240_config[][2] =
{
    {0xfe, 0x00},
    // window
    //windowing mode
    {0x09, 0x00},
    {0x0a, 0x00},
    {0x0b, 0x00},
    {0x0c, 0x00},
    {0x0d, 0x01},
    {0x0e, 0xe8},
    {0x0f, 0x02},
    {0x10, 0x88},
    //crop mode
    {0x50, 0x01},
    {0x51, 0x00},
    {0x52, 0x00},
    {0x53, 0x00},
    {0x54, 0x28},
    {0x55, 0x00},
    {0x56, 0xf0},
    {0x57, 0x00},
    {0x58, 0xf0},
    //subsample 1/2
    {0x59, 0x22},
    {0x5a, 0x00},
    {0x5b, 0x00},
    {0x5c, 0x00},
    {0x5d, 0x00},
    {0x5e, 0x00},
    {0x5f, 0x00},
    {0x60, 0x00},
    {0x61, 0x00},
    {0x62, 0x00},

    {0x00, 0x00}
};

#define NUM_CONTRAST_LEVELS (9)
static uint8_t contrast_regs[NUM_CONTRAST_LEVELS][2] =
{
    {0x80, 0x38}, /* -4 */
    {0x80, 0x34}, /* -3 */
    {0x80, 0x38}, /* -2 */
    {0x80, 0x3C}, /* -1 */
    {0x80, 0x40}, /*  0 */
    {0x80, 0x44}, /*  1 */
    {0x80, 0x48}, /*  2 */
    {0x80, 0x4C}, /*  3 */
    {0x80, 0x50}, /*  4 */
};

#define NUM_SATURATION_LEVELS (9)
static uint8_t saturation_regs[NUM_SATURATION_LEVELS][3] =
{
    {0x00, 0x00, 0x00}, /* -4 */
    {0x10, 0x10, 0x10}, /* -3 */
    {0x20, 0x20, 0x20}, /* -2 */
    {0x30, 0x30, 0x30}, /* -1 */
    {0x40, 0x40, 0x40}, /*  0 */
    {0x50, 0x50, 0x50}, /* +1 */
    {0x60, 0x60, 0x60}, /* +2 */
    {0x70, 0x70, 0x70}, /* +3 */
    {0x80, 0x80, 0x80}, /* +4 */
};

static int read_reg(sensor_t *sensor, uint16_t reg_addr)
{
    uint8_t reg_data;
    if (omv_i2c_readb(&sensor->i2c_bus, sensor->slv_addr, reg_addr, &reg_data) != 0)
    {
        return -1;
    }
    return reg_data;
}

static int write_reg(sensor_t *sensor, uint16_t reg_addr, uint16_t reg_data)
{
    return omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, reg_addr, reg_data);
}

static int set_pixformat(sensor_t *sensor, pixformat_t pixformat)
{
    int ret = 0;
    uint8_t reg;

    // P0 regs
    ret |= omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0xFE, 0x00);

    // Read current output format reg
    ret |= omv_i2c_readb(&sensor->i2c_bus, sensor->slv_addr, REG_OUTPUT_FMT, &reg);

    switch (pixformat)
    {
    case PIXFORMAT_RGB565:
        ret |= omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr,
                              REG_OUTPUT_FMT, REG_OUTPUT_FMT_RGB565);
        break;
    case PIXFORMAT_YUV422:
    case PIXFORMAT_GRAYSCALE:
        ret |= omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr,
                              REG_OUTPUT_FMT, REG_OUTPUT_FMT_YCBYCR);
        break;
    case PIXFORMAT_BAYER:
        ret |= omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr,
                              REG_OUTPUT_FMT, REG_OUTPUT_FMT_BAYER);
        break;
    default:
        return -1;
    }

    return ret;
}

static int set_framesize(sensor_t *sensor, framesize_t framesize)
{
    int ret = 0;

    uint16_t w = resolution[framesize][0];
    uint16_t h = resolution[framesize][1];

    const uint8_t (*regs)[2];

    if ((w > 320) || (h > 240))
    {
        return -1;
    }

    switch (framesize)
    {
    case FRAMESIZE_VGA:
        regs = vga_config;
        break;
    case FRAMESIZE_QVGA:
        regs = qvga_config;
        break;
    case FRAMESIZE_QQVGA:
        regs = qqvga_config;
        break;
    case FRAMESIZE_QQQVGA:
        regs = B240X240_config;
        break;
    default:
        return -1;
    }

    for (int i = 0; regs[i][0]; i++)
    {
        ret |= omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, regs[i][0], regs[i][1]);
    }
    return ret;
}

static int set_contrast(sensor_t *sensor, int level)
{
    int ret = 0;

    level += (NUM_CONTRAST_LEVELS / 2);
    if (level < 0 || level >= NUM_CONTRAST_LEVELS)
    {
        return -1;
    }
    omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0xFE, 0x00);
    omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0xD4, contrast_regs[level][0]);
    omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0xD3, contrast_regs[level][1]);
    return ret;
}

static int set_brightness(sensor_t *sensor, int level)
{
    int ret = 0;
    return ret;
}

static int set_saturation(sensor_t *sensor, int level)
{
    int ret = 0;
    level += (NUM_CONTRAST_LEVELS / 2);
    if (level < 0 || level > NUM_SATURATION_LEVELS)
    {
        return -1;
    }
    omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0xFE, 0x00);
    omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0xD0, saturation_regs[level][0]);
    omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0xD1, saturation_regs[level][1]);
    omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0xD2, saturation_regs[level][2]);
    return ret;
}

static int set_gainceiling(sensor_t *sensor, gainceiling_t gainceiling)
{
    int ret = 0;

    return ret;
}

static int set_quality(sensor_t *sensor, int qs)
{
    int ret = 0;

    return ret;
}

static int set_colorbar(sensor_t *sensor, int enable)
{
    int ret = 0;
    return ret;
}

static int set_auto_gain(sensor_t *sensor, int enable, float gain_db, float gain_db_ceiling)
{
    int ret = 0;
    return ret;
}

static int get_gain_db(sensor_t *sensor, float *gain_db)
{
    int ret = 0;
    return ret;
}

static int set_auto_exposure(sensor_t *sensor, int enable, int exposure_us)
{
    int ret = 0;
    uint8_t temp;
    omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0xFE, 0x00);
    omv_i2c_readb(&sensor->i2c_bus, sensor->slv_addr, 0x4F, &temp);
    if (enable != 0)
    {
        omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0x4F, temp | 0x01); // enable
        if (exposure_us != -1)
        {
            omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0xFE, 0x01);
            omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0x2F, (uint8_t)(((uint16_t)exposure_us) >> 8));
            omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0x30, (uint8_t)(((uint16_t)exposure_us)));
        }
    }
    else
    {
        omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0x4F, temp & 0xFE); // disable
        if (exposure_us != -1)
        {
            omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0xFE, 0x01);
            omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0x2F, (uint8_t)(((uint16_t)exposure_us) >> 8));
            omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0x30, (uint8_t)(((uint16_t)exposure_us)));
        }
    }
    return ret;
}

static int get_exposure_us(sensor_t *sensor, int *exposure_us)
{
    int ret = 0;
    return ret;
}

static int set_auto_whitebal(sensor_t *sensor, int enable, float r_gain_db, float g_gain_db, float b_gain_db)
{
    int ret = 0;
    uint8_t temp;

    omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0xFE, 0x00);
    omv_i2c_readb(&sensor->i2c_bus, sensor->slv_addr, 0x42, &temp);

    if (enable != 0)
    {
        omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0x42, temp | 0x02); // enable
        if (!isnanf(r_gain_db))
            omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0x80, (uint8_t)r_gain_db); //limit
        if (!isnanf(g_gain_db))
            omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0x81, (uint8_t)g_gain_db);
        if (!isnanf(b_gain_db))
            omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0x82, (uint8_t)b_gain_db);
    }
    else
    {
        omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0x42, temp & 0xfd); // disable
        if (!isnanf(r_gain_db))
            omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0x77, (uint8_t)r_gain_db);
        if (!isnanf(g_gain_db))
            omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0x78, (uint8_t)g_gain_db);
        if (!isnanf(b_gain_db))
            omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, 0x79, (uint8_t)b_gain_db);
    }
    return ret;
}

static int get_rgb_gain_db(sensor_t *sensor, float *r_gain_db, float *g_gain_db, float *b_gain_db)
{
    int ret = 0;
    return ret;
}

static int set_hmirror(sensor_t *sensor, int enable)
{
    uint8_t data;
    omv_i2c_readb(&sensor->i2c_bus, GC0328_SLV_ADDR, 0x17, &data);
    data = data & 0xFC;
    if (enable)
    {
        data = data | 0x01 | (sensor->vflip ? 0x02 : 0x00);
    }
    else
    {
        data = data | (sensor->vflip ? 0x02 : 0x00);
    }
    return omv_i2c_writeb(&sensor->i2c_bus, GC0328_SLV_ADDR, 0x17, data);
}

static int set_vflip(sensor_t *sensor, int enable)
{
    uint8_t data;
    omv_i2c_readb(&sensor->i2c_bus, GC0328_SLV_ADDR, 0x17, &data);
    data = data & 0xFC;
    if (enable)
    {
        data = data | 0x02 | (sensor->hmirror ? 0x01 : 0x00);
    }
    else
    {
        data = data | (sensor->hmirror ? 0x01 : 0x00);
    }
    return omv_i2c_writeb(&sensor->i2c_bus, GC0328_SLV_ADDR, 0x17, data);
}

int reset(sensor_t *sensor)
{
    uint16_t index = 0;

    omv_i2c_writeb(&sensor->i2c_bus, GC0328_SLV_ADDR, 0xfe, 0x01);
    for (index = 0; sensor_default_regs[index][0]; index++)
    {
        if (sensor_default_regs[index][0] == 0xff)
        {
            mp_hal_delay_ms(sensor_default_regs[index][1]);
            continue;
        }
        omv_i2c_writeb(&sensor->i2c_bus, GC0328_SLV_ADDR, sensor_default_regs[index][0], sensor_default_regs[index][1]);
    }
    return 0;
}

int gc0328_init(sensor_t *sensor)
{
    //Initialize sensor structure.
    sensor->reset               = reset;
    sensor->read_reg            = read_reg;
    sensor->write_reg           = write_reg;
    sensor->set_pixformat       = set_pixformat;
    sensor->set_framesize       = set_framesize;
    sensor->set_contrast        = set_contrast;
    sensor->set_brightness      = set_brightness;
    sensor->set_saturation      = set_saturation;
    sensor->set_gainceiling     = set_gainceiling;
    sensor->set_quality         = set_quality;
    sensor->set_colorbar        = set_colorbar;
    sensor->set_auto_gain       = set_auto_gain;
    sensor->get_gain_db         = get_gain_db;
    sensor->set_auto_exposure   = set_auto_exposure;
    sensor->get_exposure_us     = get_exposure_us;
    sensor->set_auto_whitebal   = set_auto_whitebal;
    sensor->get_rgb_gain_db     = get_rgb_gain_db;
    sensor->set_hmirror         = set_hmirror;
    sensor->set_vflip           = set_vflip;

    // Set sensor flags
    sensor->hw_flags.vsync = 0;
    sensor->hw_flags.hsync = 0;
    sensor->hw_flags.pixck = 1;
    sensor->hw_flags.fsync = 0;
    sensor->hw_flags.jpege = 1;
    sensor->hw_flags.gs_bpp = 2;
    sensor->hw_flags.rgb_swap = 1;
    sensor->hw_flags.yuv_order = SENSOR_HW_FLAGS_YVU422;

    return 0;
}
