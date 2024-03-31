/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-01-02     yuanjie     first version
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board.h"
#include "hal_data.h"

#include "drv_common.h"
#include "ospi_b_ep.h"
#include "ospi_b_commands.h"

#if defined(RT_USING_FAL)
#include "fal.h"
#endif

// #define DRV_DEBUG
#define LOG_TAG "drv.w25qxx"
#ifdef DRV_DEBUG
#define DBG_LVL DBG_LOG
#else
#define DBG_LVL DBG_INFO
#endif /* DRV_DEBUG */

#include <rtdbg.h>

/* External variables */
extern spi_flash_direct_transfer_t g_ospi_b_direct_transfer[OSPI_B_TRANSFER_MAX];

/**
 * @brief       This functions enables write and verify the read data.
 * @param       None
 * @retval      FSP_SUCCESS     Upon successful operation
 * @retval      FSP_ERR_ABORTED Upon incorrect read data.
 * @retval      Any Other Error code apart from FSP_SUCCESS Unsuccessful operation
 */
static fsp_err_t ospi_b_write_enable(void)
{
    fsp_err_t err = FSP_SUCCESS;
    spi_flash_direct_transfer_t transfer = {0};

    transfer = g_ospi_b_direct_transfer[OSPI_B_TRANSFER_WRITE_ENABLE_SPI];
    err = R_OSPI_B_DirectTransfer(&g_ospi0_ctrl, &transfer, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
    if (FSP_SUCCESS != err)
    {
        LOG_E("R_OSPI_B_DirectTransfer API FAILED");
        return err;
    }

    transfer = g_ospi_b_direct_transfer[OSPI_B_TRANSFER_READ_STATUS_SPI];
    err = R_OSPI_B_DirectTransfer(&g_ospi0_ctrl, &transfer, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    if (FSP_SUCCESS != err)
    {
        LOG_E("R_OSPI_B_DirectTransfer API FAILED");
        return err;
    }

    /* Check Write Enable bit in Status Register */
    if (OSPI_B_WEN_BIT_MASK != (transfer.data & OSPI_B_WEN_BIT_MASK))
    {
        LOG_E("Write enable FAILED");
        return FSP_ERR_ABORTED;
    }
    return err;
}

/**
 * @brief       This function wait until OSPI operation completes.
 * @param[in]   timeout         Maximum waiting time
 * @retval      FSP_SUCCESS     Upon successful wait OSPI operating
 * @retval      FSP_ERR_TIMEOUT Upon time out
 * @retval      Any Other Error code apart from FSP_SUCCESS Unsuccessful operation.
 */
static fsp_err_t ospi_b_wait_operation(uint32_t timeout)
{
    fsp_err_t err = FSP_SUCCESS;
    spi_flash_status_t status = {0};

    status.write_in_progress = true;
    while (status.write_in_progress)
    {
        /* Get device status */
        R_OSPI_B_StatusGet(&g_ospi0_ctrl, &status);
        if (FSP_SUCCESS != err)
        {
            LOG_E("R_OSPI_B_StatusGet API FAILED");
            return err;
        }
        if (0 == timeout)
        {
            LOG_E("OSPI time out occurred");
            return FSP_ERR_TIMEOUT;
        }
        rt_thread_mdelay(1);
        timeout--;
    }
    return err;
}

/**
 * @brief init w25q flash in spi mode using default RA HAL
 *
 * @return int
 * @todo QSPI mode testing
 */
static int _norflash_init(void)
{
    fsp_err_t err = FSP_SUCCESS;
    spi_flash_direct_transfer_t transfer = {0};
    // @todo page size settings
    /* Open OSPI module */
    err = R_OSPI_B_Open(&g_ospi0_ctrl, &g_ospi0_cfg);
    if (FSP_SUCCESS != err)
    {
        LOG_E("failed to open device");
    }

    /* Switch OSPI module to 1S-1S-1S mode to configure flash device */
    err = R_OSPI_B_SpiProtocolSet(&g_ospi0_ctrl, SPI_FLASH_PROTOCOL_EXTENDED_SPI);
    if (err != FSP_SUCCESS)
    {
        LOG_E("failed to probe SPI mode");
    }

    transfer = g_ospi_b_direct_transfer[OSPI_B_TRANSFER_READ_DEVICE_ID_SPI];
    err = R_OSPI_B_DirectTransfer(&g_ospi0_ctrl, &transfer, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    if (err != FSP_SUCCESS)
    {
        LOG_E("failed to read Flash id");
    }
    LOG_D("FLASH ID:%.2x, len:%d", transfer.data, transfer.data_length);

    /* Transfer write enable command */
    err = ospi_b_write_enable();
    if (err != FSP_SUCCESS)
    {
        LOG_E("unable to write");
    }
    LOG_D("norflash init success");
    return 0;
}

/**
 * @brief erase flash in sector region
 *
 * @param addr
 * @param size
 * @return fsp_err_t FSP_SUCCESS or other errors
 */
static fsp_err_t _ospi_erase(rt_uint32_t addr, size_t size)
{
    fsp_err_t err = FSP_SUCCESS;
    uint32_t sector_size = 0;
    int32_t erase_sector_count = 0;
    uint32_t erase_timeout = 0;
    uint8_t *p_addr_aligned_down = 0;
    //@todo massive erase support to erase faster

    sector_size = OSPI_B_SECTOR_SIZE_4K;
    erase_timeout = OSPI_B_TIME_ERASE_4K;

    p_addr_aligned_down = (uint8_t *)RT_ALIGN_DOWN(addr, OSPI_B_SECTOR_SIZE_4K);
    erase_sector_count = size / OSPI_B_SECTOR_SIZE_4K;
    if ((addr != (uint32_t)p_addr_aligned_down) || size < OSPI_B_SECTOR_SIZE_4K)
    {
        erase_sector_count++;
    }
    LOG_D("erase_sector_count:%d", erase_sector_count);
    LOG_D("p_addr_aligned_down:%.4x", p_addr_aligned_down);
    LOG_D("addr:%.4x", addr);
    LOG_D("size:%d", size);
    /* Performs erase sector */
    while (erase_sector_count--)
    {
        err = R_OSPI_B_Erase(&g_ospi0_ctrl, p_addr_aligned_down, sector_size);
        if (err != FSP_SUCCESS)
        {
            LOG_E("fR_OSPI_B_Erase API FAILED");
            return err;
        }
        /* Wait till operation completes */
        err = ospi_b_wait_operation(erase_timeout);
        if (err != FSP_SUCCESS)
        {
            LOG_E("ospi_b_wait_operation FAILED");
            return err;
        }
        LOG_D("rm:%04x,sz:%d", p_addr_aligned_down, sector_size);
        p_addr_aligned_down += sector_size;
    }
    return RT_EOK;
}

/**
 * @brief Read data from flash.
 * @note This operation's units is word.
 *
 * @param addr flash address
 * @param buf buffer to store read data
 * @param size read bytes size
 *
 * @return result
 * @todo word aligned issue
 */
static int _ospi_read(rt_uint32_t addr, rt_uint8_t *buf, size_t size)
{
    uint8_t *p_address = (uint8_t *)addr;
    LOG_D("rd:%04x:bf%04x,sz:%d", addr, buf, size);

#if BSP_CFG_DCACHE_ENABLED
    SCB_InvalidateDCache_by_Addr((volatile void *)p_address, size);
#endif
    /* Read data from flash device */
    rt_memcpy(buf, p_address, size);

    return size;
}

/**
 * @brief This function performs an write operation on the flash device.
 * @note This operation's units is word.
 * @note This operation must after erase. @see flash_erase.
 *
 * @param addr Pointer to flash device memory address
 * @param buf the write data buffer
 * @param size write bytes size
 *
 * @return result
 */
static int _ospi_write(rt_uint32_t addr, const rt_uint8_t *buf, size_t size)
{
    fsp_err_t err = FSP_SUCCESS;
    uint8_t *p_address = (uint8_t *)addr;
    size_t written_size = 0;

    while (written_size < size)
    {

        rt_enter_critical();
        /* Write data to flash device */
        err = R_OSPI_B_Write(&g_ospi0_ctrl, buf + written_size, p_address + written_size, 64);

        rt_exit_critical();

        if (FSP_SUCCESS != err)
        {
            LOG_E("R_OSPI_B_Write API FAILED");
            return err;
        }
        /* Wait until write operation completes */
        err = ospi_b_wait_operation(OSPI_B_TIME_WRITE);
        if (FSP_SUCCESS != err)
        {
            LOG_E("ospi_b_wait_operation FAILED");
            return err;
        }
        LOG_D("wr:%04x:fr%04x,sz:%d", p_address, buf, size);
        written_size += 64;
    }
    return err;
}

#if defined(RT_USING_FAL)

static int _norflash_read(long offset, rt_uint8_t *buf, size_t size)
{
    return _ospi_read(offset, buf, size);
}

static int _norflash_write(long offset, const rt_uint8_t *buf, size_t size)
{
    return _ospi_write(offset, buf, size);
}

static int _norflash_erase(long offset, size_t size)
{
    return _ospi_erase(offset, size);
}

const struct fal_flash_dev _onboard_norflash =
    {
        .name = "norflash",
        .addr = 0x0,
        .len = 8 * 1024 * 1024,
        .blk_size = 4096,
        .ops = {
            .init = _norflash_init,
            .read = _norflash_read,
            .write = _norflash_write,
            .erase = _norflash_erase},
        .write_gran = 0,
};

#endif /* RT_USING_FAL */
