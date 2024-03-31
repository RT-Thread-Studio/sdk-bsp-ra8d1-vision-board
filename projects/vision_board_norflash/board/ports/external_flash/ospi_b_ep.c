/***********************************************************************************************************************
 * File Name    : ospi_b_ep.c
 * Description  : Contains function definition.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
#include "ospi_b_commands.h"
#include "ospi_b_ep.h"

#include <rtthread.h>

#define RESET_VALUE             (0)

#define APP_ERR_RETURN(err, fn_, ...)  ({\
    if(err)\
    {\
        rt_kprintf ("[ERR] In Function: %s(), %s", __FUNCTION__, (fn_), ##__VA_ARGS__);\
        return err;\
    }\
})

/*******************************************************************************************************************//**
 * @addtogroup ospi_b_ep.c
 * @{
 **********************************************************************************************************************/

/* External variables */
extern spi_flash_direct_transfer_t g_ospi_b_direct_transfer [OSPI_B_TRANSFER_MAX];

/* Global variables */
uint8_t g_read_data [OSPI_B_APP_DATA_SIZE]  = {RESET_VALUE};
const uint8_t g_write_data [OSPI_B_APP_DATA_SIZE] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
};

/* function declarations*/
static fsp_err_t ospi_b_write_enable();
static fsp_err_t ospi_b_wait_operation(uint32_t timeout);
static fsp_err_t ospi_b_erase_operation(uint8_t *const p_address, uint32_t *const p_time);
static fsp_err_t ospi_b_write_operation(uint8_t *const p_address, uint32_t *const p_time);
static fsp_err_t ospi_b_read_operation(uint8_t *const p_address, uint32_t *const p_time);

/*******************************************************************************************************************//**
 * @brief       This functions enables write and verify the read data.
 * @param       None
 * @retval      FSP_SUCCESS     Upon successful operation
 * @retval      FSP_ERR_ABORTED Upon incorrect read data.
 * @retval      Any Other Error code apart from FSP_SUCCESS Unsuccessful operation
 **********************************************************************************************************************/
static fsp_err_t ospi_b_write_enable(void)
{
    fsp_err_t                   err             = FSP_SUCCESS;
    spi_flash_direct_transfer_t transfer        = {RESET_VALUE};

    /* Transfer write enable command */
    transfer = (SPI_FLASH_PROTOCOL_EXTENDED_SPI == g_ospi_b_ctrl.spi_protocol)
               ? g_ospi_b_direct_transfer[OSPI_B_TRANSFER_WRITE_ENABLE_SPI]
               : g_ospi_b_direct_transfer[OSPI_B_TRANSFER_WRITE_ENABLE_OPI];
    err = R_OSPI_B_DirectTransfer(&g_ospi_b_ctrl, &transfer, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
    APP_ERR_RETURN(err, "R_OSPI_B_DirectTransfer API FAILED \r\n");

    /* Read Status Register */
    transfer = (SPI_FLASH_PROTOCOL_EXTENDED_SPI == g_ospi_b_ctrl.spi_protocol)
               ? g_ospi_b_direct_transfer[OSPI_B_TRANSFER_READ_STATUS_SPI]
               : g_ospi_b_direct_transfer[OSPI_B_TRANSFER_READ_STATUS_OPI];
    err = R_OSPI_B_DirectTransfer(&g_ospi_b_ctrl, &transfer, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    APP_ERR_RETURN(err, "R_OSPI_B_DirectTransfer API FAILED \r\n");

    /* Check Write Enable bit in Status Register */
    if (OSPI_B_WEN_BIT_MASK != (transfer.data & OSPI_B_WEN_BIT_MASK))
    {
        APP_ERR_RETURN(FSP_ERR_ABORTED, "Write enable FAILED\r\n");
    }
    return err;
}

/*******************************************************************************************************************//**
 * @brief       This function wait until OSPI operation completes.
 * @param[in]   timeout         Maximum waiting time
 * @retval      FSP_SUCCESS     Upon successful wait OSPI operating
 * @retval      FSP_ERR_TIMEOUT Upon time out
 * @retval      Any Other Error code apart from FSP_SUCCESS Unsuccessful operation.
 **********************************************************************************************************************/
static fsp_err_t ospi_b_wait_operation(uint32_t timeout)
{
    fsp_err_t          err    = FSP_SUCCESS;
    spi_flash_status_t status = {RESET_VALUE};

    status.write_in_progress = true;
    while (status.write_in_progress)
    {
        /* Get device status */
        R_OSPI_B_StatusGet(&g_ospi_b_ctrl, &status);
        APP_ERR_RETURN(err, "R_OSPI_B_StatusGet API FAILED\r\n");
        if (RESET_VALUE == timeout)
        {
            APP_ERR_RETURN(FSP_ERR_TIMEOUT, "OSPI time out occurred\r\n");
        }
        rt_thread_mdelay(1);
        timeout --;
    }
    return err;
}

/**********************************************************************************************************************
 * @brief       This function performs an erase sector operation on the flash device.
 * @param[in]   *p_address  Pointer to flash device memory address
 * @param[out]  *p_time     Pointer will be used to store execute time
 * @retval      FSP_SUCCESS Upon successful erase operation
 * @retval      Any Other Error code apart from FSP_SUCCESS Unsuccessful operation
 **********************************************************************************************************************/
static fsp_err_t ospi_b_erase_operation(uint8_t *const p_address, uint32_t *const p_time)
{
    fsp_err_t   err             = FSP_SUCCESS;
    uint32_t    sector_size     = RESET_VALUE;
    uint32_t    erase_timeout   = RESET_VALUE;

    /* Check sector size according to input address pointer, described in S28HS512T data sheet */
    if (OSPI_B_SECTOR_4K_END_ADDRESS < (uint32_t)p_address)
    {
        sector_size = OSPI_B_SECTOR_SIZE_256K;
        erase_timeout = OSPI_B_TIME_ERASE_256K;
    }
    else
    {
        sector_size = OSPI_B_SECTOR_SIZE_4K;
        erase_timeout = OSPI_B_TIME_ERASE_4K;
    }

    /* Performs erase sector */
    err = R_OSPI_B_Erase(&g_ospi_b_ctrl, p_address, sector_size);
    APP_ERR_RETURN(err, "R_OSPI_B_Erase API FAILED\r\n");

    /* Wait till operation completes */
    err = ospi_b_wait_operation(erase_timeout);
    APP_ERR_RETURN(err, "ospi_b_wait_operation FAILED\r\n");

    return err;
}

/**********************************************************************************************************************
 * @brief       This function performs an write operation on the flash device.
 * @param[in]   *p_address      Pointer to flash device memory address
 * @param[out]  *p_time     Pointer will be used to store execute time
 * @retval      FSP_SUCCESS Upon successful write operation
 * @retval      Any Other Error code apart from FSP_SUCCESS Unsuccessful operation
 **********************************************************************************************************************/
static fsp_err_t ospi_b_write_operation(uint8_t *const p_address, uint32_t *const p_time)
{
    fsp_err_t   err         = FSP_SUCCESS;
    uint32_t    erase_time  = RESET_VALUE;

    /* Erase sector before write data to flash device */
    err = ospi_b_erase_operation(p_address, &erase_time);
    APP_ERR_RETURN(err, "ospi_b_erase_operation FAILED\r\n");

    /* Write data to flash device */
    err = R_OSPI_B_Write(&g_ospi_b_ctrl, g_write_data, p_address, OSPI_B_APP_DATA_SIZE);
    APP_ERR_RETURN(err, "R_OSPI_B_Write API FAILED\r\n");

    /* Wait until write operation completes */
    err = ospi_b_wait_operation(OSPI_B_TIME_WRITE);
    APP_ERR_RETURN(err, "ospi_b_wait_operation FAILED\r\n");

    return err;
}

/**********************************************************************************************************************
 * @brief       This function performs an read operation on the flash device.
 * @param[in]   *p_address  Pointer to flash device memory address
 * @param[out]  *p_time     Pointer will be used to store execute time
 * @retval      FSP_SUCCESS Upon successful read operation
 * @retval      Any Other Error code apart from FSP_SUCCESS Unsuccessful operation
 **********************************************************************************************************************/
static fsp_err_t ospi_b_read_operation(uint8_t *const p_address, uint32_t *const p_time)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Clean read buffer */
    memset(&g_read_data, RESET_VALUE, OSPI_B_APP_DATA_SIZE);

    /* Read data from flash device */
    memcpy(&g_read_data, p_address, OSPI_B_APP_DATA_SIZE);

    return err;
}

/*******************************************************************************************************************//**
 * @brief       This functions initializes OSPI module and Flash device.
 * @param       None
 * @retval      FSP_SUCCESS     Upon successful initialization of OSPI module and Flash device
 * @retval      Any Other Error code apart from FSP_SUCCESS  Unsuccessful open
 **********************************************************************************************************************/
fsp_err_t ospi_b_init(void)
{
    /* By default, the flash device is in SPI mode, so it is necessary to open the OSPI module in SPI mode */
    fsp_err_t                   err             = FSP_SUCCESS;
    spi_flash_direct_transfer_t transfer        = {RESET_VALUE};

    /* Open OSPI module */
    err = R_OSPI_B_Open(&g_ospi_b_ctrl, &g_ospi_b_cfg);
    APP_ERR_RETURN(err, "R_OSPI_B_Open API FAILED \r\n");

    /* Switch OSPI module to 1S-1S-1S mode to configure flash device */
    err = R_OSPI_B_SpiProtocolSet(&g_ospi_b_ctrl, SPI_FLASH_PROTOCOL_EXTENDED_SPI);
    APP_ERR_RETURN(err, "R_OSPI_B_SpiProtocolSet API FAILED \r\n");

    /* Reset flash device by driving OM_RESET pin */
    R_XSPI->LIOCTL_b.RSTCS0 = 0;
    R_BSP_SoftwareDelay(OSPI_B_TIME_RESET_PULSE, OSPI_B_TIME_UNIT);
    R_XSPI->LIOCTL_b.RSTCS0 = 1;
    R_BSP_SoftwareDelay(OSPI_B_TIME_RESET_SETUP, OSPI_B_TIME_UNIT);

    /* Transfer write enable command */
    err = ospi_b_write_enable();
    APP_ERR_RETURN(err, "ospi_b_write_enable FAILED \r\n");

    /* Write to CFR2V to configure Address Byte Length and Memory Array Read Latency */
    transfer = g_ospi_b_direct_transfer[OSPI_B_TRANSFER_WRITE_CFR2V_SPI];
    transfer.address_length = OSPI_B_ADDRESS_LENGTH_THREE;  // default Address Byte Length is 3 bytes
    err = R_OSPI_B_DirectTransfer(&g_ospi_b_ctrl, &transfer, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
    APP_ERR_RETURN(err, "R_OSPI_B_DirectTransfer API FAILED \r\n");

    /* Transfer write enable command */
    err = ospi_b_write_enable();
    APP_ERR_RETURN(err, "ospi_b_write_enable FAILED \r\n");

    /* Write to CFR3V to configure Volatile Register Read Latency */
    transfer = g_ospi_b_direct_transfer[OSPI_B_TRANSFER_WRITE_CFR3V_SPI];
    err = R_OSPI_B_DirectTransfer(&g_ospi_b_ctrl, &transfer, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
    APP_ERR_RETURN(err, "R_OSPI_B_DirectTransfer API FAILED \r\n");

    /* Read back and verify CFR2V register data */
    transfer = g_ospi_b_direct_transfer[OSPI_B_TRANSFER_READ_CFR2V_SPI];
    err = R_OSPI_B_DirectTransfer(&g_ospi_b_ctrl, &transfer, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    APP_ERR_RETURN(err, "R_OSPI_B_DirectTransfer API FAILED \r\n");
    if (OSPI_B_DATA_CFR2V_REGISTER != (uint8_t)transfer.data)
    {
        APP_ERR_RETURN(FSP_ERR_ABORTED, "Verify CFR2V register data FAILED\r\n");
    }

    /* Read back and verify CFR3V register data */
    transfer = g_ospi_b_direct_transfer[OSPI_B_TRANSFER_READ_CFR3V_SPI];
    err = R_OSPI_B_DirectTransfer(&g_ospi_b_ctrl, &transfer, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    APP_ERR_RETURN(err, "R_OSPI_B_DirectTransfer API FAILED \r\n");
    if (OSPI_B_DATA_CFR3V_REGISTER != (uint8_t)transfer.data)
    {
        APP_ERR_RETURN(FSP_ERR_ABORTED, "Verify CFR3V register data FAILED\r\n");
    }
    return err;
}

/*******************************************************************************************************************//**
 * @brief       This function configures ospi to extended spi mode.
 * @param[IN]   None
 * @retval      FSP_SUCCESS                  Upon successful transition to spi operating mode.
 * @retval      FSP_ERR_ABORTED              Upon incorrect read data.
 * @retval      Any Other Error code apart from FSP_SUCCESS  Unsuccessful operation
 **********************************************************************************************************************/
fsp_err_t ospi_b_set_protocol_to_spi()
{
    fsp_err_t                   err      = FSP_SUCCESS;
    spi_flash_direct_transfer_t transfer = {RESET_VALUE};

    if (SPI_FLASH_PROTOCOL_EXTENDED_SPI == g_ospi_b_ctrl.spi_protocol)
    {
        /* Do nothing */
    }
    else if (SPI_FLASH_PROTOCOL_8D_8D_8D == g_ospi_b_ctrl.spi_protocol)
    {
        /* Transfer write enable command */
        err = ospi_b_write_enable();
        APP_ERR_RETURN(err, "ospi_b_write_enable FAILED \r\n");

        /* Write to CFR5V Register to Configure flash device interface mode */
        transfer = g_ospi_b_direct_transfer[OSPI_B_TRANSFER_WRITE_CFR5V_OPI];
        transfer.data = OSPI_B_DATA_SET_SPI_CFR5V_REGISTER;
        err = R_OSPI_B_DirectTransfer(&g_ospi_b_ctrl, &transfer, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
        APP_ERR_RETURN(err, "R_OSPI_B_DirectTransfer API FAILED \r\n");

        /* Switch OSPI module mode to SPI mode */
        err = R_OSPI_B_SpiProtocolSet(&g_ospi_b_ctrl, SPI_FLASH_PROTOCOL_EXTENDED_SPI);
        APP_ERR_RETURN(err, "R_OSPI_SpiProtocolSet API FAILED \r\n");

        /* Read back and verify CFR5V register data */
        transfer = g_ospi_b_direct_transfer[OSPI_B_TRANSFER_READ_CFR5V_SPI];
        err = R_OSPI_B_DirectTransfer(&g_ospi_b_ctrl, &transfer, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
        APP_ERR_RETURN(err, "R_OSPI_B_DirectTransfer API FAILED \r\n");
        if (OSPI_B_DATA_SET_SPI_CFR5V_REGISTER != (uint8_t)transfer.data)
        {
            APP_ERR_RETURN(FSP_ERR_ABORTED, "Verify CFR5V register data FAILED\r\n");
        }
    }
    else
    {
        APP_ERR_RETURN(FSP_ERR_INVALID_MODE, "Flash device do not support this mode\r\n");
    }
    return err;
}

/*******************************************************************************************************************//**
 * @brief       This function configures ospi to opi mode.
 * @param[IN]   None
 * @retval      FSP_SUCCESS                  Upon successful transition to opi operating mode.
 * @retval      FSP_ERR_ABORTED              Upon incorrect read data.
 * @retval      Any Other Error code apart from FSP_SUCCESS  Unsuccessful operation
 **********************************************************************************************************************/
fsp_err_t ospi_b_set_protocol_to_opi()
{
    fsp_err_t                   err      = FSP_SUCCESS;
    spi_flash_direct_transfer_t transfer = {RESET_VALUE};

    if (SPI_FLASH_PROTOCOL_8D_8D_8D == g_ospi_b_ctrl.spi_protocol)
    {
        /* Do nothing */
    }
    else if (SPI_FLASH_PROTOCOL_EXTENDED_SPI == g_ospi_b_ctrl.spi_protocol)
    {
        /* Transfer write enable command */
        err = ospi_b_write_enable();
        APP_ERR_RETURN(err, "ospi_b_write_enable FAILED \r\n");

        /* Write to CFR5V Register to Configure flash device interface mode */
        transfer = g_ospi_b_direct_transfer[OSPI_B_TRANSFER_WRITE_CFR5V_SPI];
        transfer.data = OSPI_B_DATA_SET_OPI_CFR5V_REGISTER;
        err = R_OSPI_B_DirectTransfer(&g_ospi_b_ctrl, &transfer, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
        APP_ERR_RETURN(err, "R_OSPI_B_DirectTransfer API FAILED \r\n");

        /* Switch OSPI module mode to OPI mode */
        err = R_OSPI_B_SpiProtocolSet(&g_ospi_b_ctrl, SPI_FLASH_PROTOCOL_8D_8D_8D);
        APP_ERR_RETURN(err, "R_OSPI_SpiProtocolSet API FAILED \r\n");

        /* Read back and verify CFR5V register data */
        transfer = g_ospi_b_direct_transfer[OSPI_B_TRANSFER_READ_CFR5V_OPI];
        err = R_OSPI_B_DirectTransfer(&g_ospi_b_ctrl, &transfer, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
        APP_ERR_RETURN(err, "R_OSPI_B_DirectTransfer API FAILED \r\n");
        if (OSPI_B_DATA_SET_OPI_CFR5V_REGISTER != (uint8_t)transfer.data)
        {
            APP_ERR_RETURN(FSP_ERR_ABORTED, "Verify CFR5V register data FAILED\r\n");
        }
    }
    else
    {
        APP_ERR_RETURN(FSP_ERR_INVALID_MODE, "Flash device do not support this mode\r\n");
    }
    return err;
}

/**********************************************************************************************************************
 * @brief       This function reads flash device id
 * @param[out]  *p_device_id        Pointer will be used to store device id
 * @retval      FSP_SUCCESS         Upon successful direct transfer operation
 * @retval      FSP_ERR_ABORTED     On incorrect device id read.
 * @retval      Any Other Error code apart from FSP_SUCCESS  Unsuccessful operation
 **********************************************************************************************************************/
fsp_err_t ospi_b_read_device_id(uint32_t *const p_id)
{
    fsp_err_t                   err             = FSP_SUCCESS;
    spi_flash_direct_transfer_t transfer        = {RESET_VALUE};

    /* Read and check flash device ID */
    transfer = (SPI_FLASH_PROTOCOL_EXTENDED_SPI == g_ospi_b_ctrl.spi_protocol)
               ? g_ospi_b_direct_transfer[OSPI_B_TRANSFER_READ_DEVICE_ID_SPI]
               : g_ospi_b_direct_transfer[OSPI_B_TRANSFER_READ_DEVICE_ID_OPI];
    err = R_OSPI_B_DirectTransfer(&g_ospi_b_ctrl, &transfer, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
    APP_ERR_RETURN(err, "R_OSPI_B_DirectTransfer API FAILED \r\n");
    if (OSPI_B_DEVICE_ID != transfer.data)
    {
        APP_ERR_RETURN(FSP_ERR_ABORTED, "Device ID is incorrect\r\n");
    }

    /* Get flash device ID */
    *p_id = transfer.data;
    return err;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup ospi_b_ep.c)
 **********************************************************************************************************************/
int ospi_probe_test(void)
{
    static bool initialized = false;
    if (!initialized)
    {
        initialized = true;
        fsp_err_t err = FSP_SUCCESS;
        uint32_t flash_id = RESET_VALUE;

        err = ospi_b_init();
        APP_ERR_RETURN(err, "ospi_b_init FAILED \r\n");

        /* Read Flash device ID */
        err = ospi_b_read_device_id(&flash_id);
        APP_ERR_RETURN(err, "ospi_b_read_device_id FAILED\r\n");

        /* Print flash device ID */
        rt_kprintf("Device ID read successfully\n");
        rt_kprintf("Manufacturer ID = 0x%x, Memory Interface Type = 0x%x, Density = 0x%x\n",
                   (uint8_t)flash_id, (uint8_t)(flash_id >> 8), (uint8_t)(flash_id >> 16));
    }

    return 0;
}
MSH_CMD_EXPORT(ospi_probe_test, ospi flash probe)
