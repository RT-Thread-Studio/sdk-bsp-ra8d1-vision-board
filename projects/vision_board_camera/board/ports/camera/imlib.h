/*
 * This file is part of the OpenMV project.
 *
 * Copyright (c) 2013-2021 Ibrahim Abdelkader <iabdalkader@openmv.io>
 * Copyright (c) 2013-2021 Kwabena W. Agyeman <kwagyeman@openmv.io>
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * Image processing library.
 */
#ifndef __IMLIB_H__
#define __IMLIB_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "fmath.h"
#include "cam_boardconfig.h"

#ifndef M_PI
#define M_PI                     3.14159265f
#define M_PI_2                   1.57079632f
#define M_PI_4                   0.78539816f
#endif

#define IM_LOG2_2(x)             (((x) & 0x2ULL) ? (2) :             1)                                // NO ({ ... }) !
#define IM_LOG2_4(x)             (((x) & 0xCULL) ? (2 + IM_LOG2_2((x) >> 2)) :  IM_LOG2_2(x))          // NO ({ ... }) !
#define IM_LOG2_8(x)             (((x) & 0xF0ULL) ? (4 + IM_LOG2_4((x) >> 4)) :  IM_LOG2_4(x))         // NO ({ ... }) !
#define IM_LOG2_16(x)            (((x) & 0xFF00ULL) ? (8 + IM_LOG2_8((x) >> 8)) :  IM_LOG2_8(x))       // NO ({ ... }) !
#define IM_LOG2_32(x)            (((x) & 0xFFFF0000ULL) ? (16 + IM_LOG2_16((x) >> 16)) : IM_LOG2_16(x)) // NO ({ ... }) !
#define IM_LOG2(x)               (((x) & 0xFFFFFFFF00000000ULL) ? (32 + IM_LOG2_32((x) >> 32)) : IM_LOG2_32(x)) // NO ({ ... }) !

#define IM_IS_SIGNED(a)          (__builtin_types_compatible_p(__typeof__(a), signed) || \
                                  __builtin_types_compatible_p(__typeof__(a), signed long))
#define IM_IS_UNSIGNED(a)        (__builtin_types_compatible_p(__typeof__(a), unsigned) || \
                                  __builtin_types_compatible_p(__typeof__(a), unsigned long))
#define IM_SIGN_COMPARE(a, b)    ((IM_IS_SIGNED(a) && IM_IS_UNSIGNED(b)) || \
                                  (IM_IS_SIGNED(b) && IM_IS_UNSIGNED(a)))

#define M_LN2                   0.69314718055994530942f
#define M_LN10                  2.30258509299404568402f

#define IM_MAX(a,b)     ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define IM_MIN(a,b)     ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })

#define IM_DIV(a, b)    ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _b ? (_a / _b) : 0; })
#define IM_MOD(a, b)    ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _b ? (_a % _b) : 0; })

#define INT8_T_BITS              (sizeof(int8_t) * 8)
#define INT8_T_MASK              (INT8_T_BITS - 1)
#define INT8_T_SHIFT             IM_LOG2(INT8_T_MASK)

#define INT16_T_BITS             (sizeof(int16_t) * 8)
#define INT16_T_MASK             (INT16_T_BITS - 1)
#define INT16_T_SHIFT            IM_LOG2(INT16_T_MASK)

#define INT32_T_BITS             (sizeof(int32_t) * 8)
#define INT32_T_MASK             (INT32_T_BITS - 1)
#define INT32_T_SHIFT            IM_LOG2(INT32_T_MASK)

#define INT64_T_BITS             (sizeof(int64_t) * 8)
#define INT64_T_MASK             (INT64_T_BITS - 1)
#define INT64_T_SHIFT            IM_LOG2(INT64_T_MASK)

#define UINT8_T_BITS             (sizeof(uint8_t) * 8)
#define UINT8_T_MASK             (UINT8_T_BITS - 1)
#define UINT8_T_SHIFT            IM_LOG2(UINT8_T_MASK)

#define UINT16_T_BITS            (sizeof(uint16_t) * 8)
#define UINT16_T_MASK            (UINT16_T_BITS - 1)
#define UINT16_T_SHIFT           IM_LOG2(UINT16_T_MASK)

#define UINT32_T_BITS            (sizeof(uint32_t) * 8)
#define UINT32_T_MASK            (UINT32_T_BITS - 1)
#define UINT32_T_SHIFT           IM_LOG2(UINT32_T_MASK)

#define UINT64_T_BITS            (sizeof(uint64_t) * 8)
#define UINT64_T_MASK            (UINT64_T_BITS - 1)
#define UINT64_T_SHIFT           IM_LOG2(UINT64_T_MASK)

#define IM_DEG2RAD(x)            (((x) * M_PI) / 180)
#define IM_RAD2DEG(x)            (((x) * 180) / M_PI)

inline bool isinff(float arg)
{
    return isinf(arg);
}

inline bool isnanf(float arg)
{
    return isnan(arg);
}


// Pixel format IDs.
typedef enum {
    PIXFORMAT_ID_BINARY = 1,
    PIXFORMAT_ID_GRAY   = 2,
    PIXFORMAT_ID_RGB565 = 3,
    PIXFORMAT_ID_BAYER  = 4,
    PIXFORMAT_ID_YUV422 = 5,
    PIXFORMAT_ID_JPEG   = 6,
    PIXFORMAT_ID_PNG    = 7,
    PIXFORMAT_ID_ARGB8  = 8,
    /* Note: Update PIXFORMAT_IS_VALID when adding new formats */
} pixformat_id_t;

// Pixel sub-format IDs.
typedef enum {
    SUBFORMAT_ID_GRAY8  = 0,
    SUBFORMAT_ID_GRAY16 = 1,
    SUBFORMAT_ID_BGGR   = 0,     // !!! Note: Make sure bayer sub-formats don't  !!!
    SUBFORMAT_ID_GBRG   = 1,     // !!! overflow the sensor.hw_flags.bayer field !!!
    SUBFORMAT_ID_GRBG   = 2,
    SUBFORMAT_ID_RGGB   = 3,
    SUBFORMAT_ID_YUV422 = 0,
    SUBFORMAT_ID_YVU422 = 1,
    /* Note: Update PIXFORMAT_IS_VALID when adding new formats */
} subformat_id_t;

// Pixel format Byte Per Pixel.
typedef enum {
    PIXFORMAT_BPP_BINARY = 0,
    PIXFORMAT_BPP_GRAY8  = 1,
    PIXFORMAT_BPP_GRAY16 = 2,
    PIXFORMAT_BPP_RGB565 = 2,
    PIXFORMAT_BPP_BAYER  = 1,
    PIXFORMAT_BPP_YUV422 = 2,
    PIXFORMAT_BPP_ARGB8  = 4,
    /* Note: Update PIXFORMAT_IS_VALID when adding new formats */
} pixformat_bpp_t;

// Pixel format flags.
#define PIXFORMAT_FLAGS_Y          (1 << 28) // YUV format.
#define PIXFORMAT_FLAGS_M          (1 << 27) // Mutable format.
#define PIXFORMAT_FLAGS_C          (1 << 26) // Colored format.
#define PIXFORMAT_FLAGS_J          (1 << 25) // Compressed format (JPEG/PNG).
#define PIXFORMAT_FLAGS_R          (1 << 24) // RAW/Bayer format.
#define PIXFORMAT_FLAGS_CY         (PIXFORMAT_FLAGS_C | PIXFORMAT_FLAGS_Y)
#define PIXFORMAT_FLAGS_CM         (PIXFORMAT_FLAGS_C | PIXFORMAT_FLAGS_M)
#define PIXFORMAT_FLAGS_CR         (PIXFORMAT_FLAGS_C | PIXFORMAT_FLAGS_R)
#define PIXFORMAT_FLAGS_CJ         (PIXFORMAT_FLAGS_C | PIXFORMAT_FLAGS_J)
#define IMLIB_IMAGE_MAX_SIZE(x)    ((x) & 0xFFFFFFFF)

// *INDENT-OFF*
// Each pixel format encodes flags, pixel format id and bpp as follows:
// 31......29  28  27  26  25  24  23..........16  15...........8  7.............0
// <RESERVED>  YF  MF  CF  JF  RF  <PIXFORMAT_ID>  <SUBFORMAT_ID>  <BYTES_PER_PIX>
// NOTE: Bit 31-30 must Not be used for pixformat_t to be used as mp_int_t.
typedef enum {
  PIXFORMAT_INVALID    = (0x00000000U),
  PIXFORMAT_BINARY     = (PIXFORMAT_FLAGS_M  | (PIXFORMAT_ID_BINARY << 16) | (0                   << 8) | PIXFORMAT_BPP_BINARY ),
  PIXFORMAT_GRAYSCALE  = (PIXFORMAT_FLAGS_M  | (PIXFORMAT_ID_GRAY   << 16) | (SUBFORMAT_ID_GRAY8  << 8) | PIXFORMAT_BPP_GRAY8  ),
  PIXFORMAT_RGB565     = (PIXFORMAT_FLAGS_CM | (PIXFORMAT_ID_RGB565 << 16) | (0                   << 8) | PIXFORMAT_BPP_RGB565 ),
  PIXFORMAT_ARGB8      = (PIXFORMAT_FLAGS_CM | (PIXFORMAT_ID_ARGB8  << 16) | (0                   << 8) | PIXFORMAT_BPP_ARGB8  ),
  PIXFORMAT_BAYER      = (PIXFORMAT_FLAGS_CR | (PIXFORMAT_ID_BAYER  << 16) | (SUBFORMAT_ID_BGGR   << 8) | PIXFORMAT_BPP_BAYER  ),
  PIXFORMAT_BAYER_BGGR = (PIXFORMAT_FLAGS_CR | (PIXFORMAT_ID_BAYER  << 16) | (SUBFORMAT_ID_BGGR   << 8) | PIXFORMAT_BPP_BAYER  ),
  PIXFORMAT_BAYER_GBRG = (PIXFORMAT_FLAGS_CR | (PIXFORMAT_ID_BAYER  << 16) | (SUBFORMAT_ID_GBRG   << 8) | PIXFORMAT_BPP_BAYER  ),
  PIXFORMAT_BAYER_GRBG = (PIXFORMAT_FLAGS_CR | (PIXFORMAT_ID_BAYER  << 16) | (SUBFORMAT_ID_GRBG   << 8) | PIXFORMAT_BPP_BAYER  ),
  PIXFORMAT_BAYER_RGGB = (PIXFORMAT_FLAGS_CR | (PIXFORMAT_ID_BAYER  << 16) | (SUBFORMAT_ID_RGGB   << 8) | PIXFORMAT_BPP_BAYER  ),
  PIXFORMAT_YUV        = (PIXFORMAT_FLAGS_CY | (PIXFORMAT_ID_YUV422 << 16) | (SUBFORMAT_ID_YUV422 << 8) | PIXFORMAT_BPP_YUV422 ),
  PIXFORMAT_YUV422     = (PIXFORMAT_FLAGS_CY | (PIXFORMAT_ID_YUV422 << 16) | (SUBFORMAT_ID_YUV422 << 8) | PIXFORMAT_BPP_YUV422 ),
  PIXFORMAT_YVU422     = (PIXFORMAT_FLAGS_CY | (PIXFORMAT_ID_YUV422 << 16) | (SUBFORMAT_ID_YVU422 << 8) | PIXFORMAT_BPP_YUV422 ),
  PIXFORMAT_JPEG       = (PIXFORMAT_FLAGS_CJ | (PIXFORMAT_ID_JPEG   << 16) | (0                   << 8) | 0                    ),
  PIXFORMAT_PNG        = (PIXFORMAT_FLAGS_CJ | (PIXFORMAT_ID_PNG    << 16) | (0                   << 8) | 0                    ),
  PIXFORMAT_LAST       = (0xFFFFFFFFU),
} pixformat_t;

// *INDENT-OFF*
#define PIXFORMAT_STRUCT            \
struct {                            \
  union {                           \
    struct {                        \
        uint32_t bpp            :8; \
        uint32_t subfmt_id      :8; \
        uint32_t pixfmt_id      :8; \
        uint32_t is_bayer       :1; \
        uint32_t is_compressed  :1; \
        uint32_t is_color       :1; \
        uint32_t is_mutable     :1; \
        uint32_t is_yuv         :1; \
        uint32_t /*reserved*/   :3; \
    };                              \
    uint32_t pixfmt;                \
  };                                \
  uint32_t size; /* for compressed images */ \
}

typedef struct image {
    int32_t w;
    int32_t h;
    PIXFORMAT_STRUCT;
    union {
        uint8_t *pixels;
        uint8_t *data;
    };
} image_t;

#endif //__IMLIB_H__
