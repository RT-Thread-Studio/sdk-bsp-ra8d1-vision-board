/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-20     Sherman      the first version
 */
#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#include "hal_data.h"
#include "rtconfig.h"

#define FAL_DEV_NAME_MAX 12

#if defined(BSP_USING_ONCHIP_FLASH)
    extern const struct fal_flash_dev _onchip_flash_hp0;
    extern const struct fal_flash_dev _onchip_flash_hp1;
    extern const struct fal_flash_dev _onchip_flash_hp3;
#endif /* BSP_USING_ONCHIP_FLASH */

#if defined(BSP_USING_ONBOARD_NOR_FLASH)
    #define NORFLASH_START_ADDRESS     BSP_FEATURE_OSPI_B_DEVICE_1_START_ADDRESS
    extern const struct fal_flash_dev _onboard_norflash;
#endif /* BSP_USING_ONBOARD_NOR_FLASH */

/* flash device table */
#if defined(BSP_USING_ONCHIP_FLASH) && !defined(BSP_USING_ONBOARD_NOR_FLASH)
    #define FAL_FLASH_DEV_TABLE             \
    {                                       \
        &_onchip_flash_hp0,                 \
        &_onchip_flash_hp1,                 \
        &_onchip_flash_hp3,                 \
    }
#endif /* BSP_USING_ONCHIP_FLASH Only */

#if defined(BSP_USING_ONBOARD_NOR_FLASH) && !defined(BSP_USING_ONCHIP_FLASH)
    #define FAL_FLASH_DEV_TABLE             \
    {                                       \
        &_onboard_norflash,                 \
    }
#endif /* BSP_USING_ONBOARD_NOR_FLASH Only */

#if defined(BSP_USING_ONCHIP_FLASH) && defined(BSP_USING_ONBOARD_NOR_FLASH)
    #define FAL_FLASH_DEV_TABLE             \
    {                                       \
        &_onchip_flash_hp0,                 \
        &_onchip_flash_hp1,                 \
        &_onchip_flash_hp3,                 \
        &_onboard_norflash,                 \
    }
#endif /* BSP_USING_ONCHIP_FLASH && BSP_USING_ONBOARD_NOR_FLASH */

/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG

#if defined(BSP_USING_ONCHIP_FLASH) && !defined(BSP_USING_ONBOARD_NOR_FLASH)
/** partition table, The chip flash partition is defined in "\ra\fsp\src\bsp\mcu\ra6m4\bsp_feature.h".
 * More details can be found in the RA6M4 Group User Manual: Hardware section 47 Flash memory.*/
    #define FAL_PART_TABLE                                                                                                      \
    {                                                                                                                           \
        {FAL_PART_MAGIC_WROD, "boot", "onchip_flash_hp0", 0, BSP_FEATURE_FLASH_HP_CF_REGION0_SIZE, 0},                          \
        {FAL_PART_MAGIC_WROD, "app", "onchip_flash_hp1", 0, (BSP_ROM_SIZE_BYTES - BSP_FEATURE_FLASH_HP_CF_REGION0_SIZE), 0},    \
        {FAL_PART_MAGIC_WROD, "disk", "onchip_flash_hp3", 0, (BSP_DATA_FLASH_SIZE_BYTES), 0},                                   \
    }
#endif /* BSP_USING_ONCHIP_FLASH Only */

#if defined(BSP_USING_ONBOARD_NOR_FLASH) && !defined(BSP_USING_ONCHIP_FLASH)
    #define FAL_PART_TABLE                                                         \
    {                                                                              \
        {FAL_PART_MAGIC_WROD, "boot", "norflash", 0x90000000, 0x10000, 0},    \
        {FAL_PART_MAGIC_WROD,  "app", "norflash", 0x90010000, 0x10000, 0},    \
        {FAL_PART_MAGIC_WROD, "disk", "norflash", 0x90020000, 0x20000, 0},    \
    }
#endif /* BSP_USING_ONBOARD_NOR_FLASH Only */

// @TODO #if defined(BSP_USING_ONCHIP_FLASH) && defined(BSP_USING_ONBOARD_NOR_FLASH)


#endif /* FAL_PART_HAS_TABLE_CFG */
// #endif /* BSP_USING_ONCHIP_FLASH */

// #if defined(BSP_USING_ONBOARD_NOR_FLASH)


#endif /* _FAL_CFG_H_ */

