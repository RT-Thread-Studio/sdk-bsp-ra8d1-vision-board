/***********************************************************************************************************************
 * Copyright [2020-2023] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_spi_b.h"
#include "r_spi_b_cfg.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** "SPI" in ASCII, used to determine if channel is open. */
#define SPI_B_OPEN                        (0x52535049ULL)

/** SPI base register access macro.  */
#define SPI_B_REG(channel)    ((R_SPI_B0_Type *) ((uint32_t) R_SPI_B0 +                         \
                                                  ((uint32_t) R_SPI_B1 - (uint32_t) R_SPI_B0) * \
                                                  (channel)))

#define SPI_B_DTC_MAX_TRANSFER            (0x10000)

#define SPI_B_DTC_RX_TRANSFER_SETTINGS    ((TRANSFER_MODE_NORMAL << TRANSFER_SETTINGS_MODE_BITS) |         \
                                           (TRANSFER_SIZE_1_BYTE << TRANSFER_SETTINGS_SIZE_BITS) |         \
                                           (TRANSFER_ADDR_MODE_FIXED << TRANSFER_SETTINGS_SRC_ADDR_BITS) | \
                                           (TRANSFER_IRQ_END << TRANSFER_SETTINGS_IRQ_BITS) |              \
                                           (TRANSFER_ADDR_MODE_INCREMENTED << TRANSFER_SETTINGS_DEST_ADDR_BITS))

#define SPI_B_DTC_TX_TRANSFER_SETTINGS    ((TRANSFER_MODE_NORMAL << TRANSFER_SETTINGS_MODE_BITS) |               \
                                           (TRANSFER_SIZE_1_BYTE << TRANSFER_SETTINGS_SIZE_BITS) |               \
                                           (TRANSFER_ADDR_MODE_INCREMENTED << TRANSFER_SETTINGS_SRC_ADDR_BITS) | \
                                           (TRANSFER_IRQ_END << TRANSFER_SETTINGS_IRQ_BITS) |                    \
                                           (TRANSFER_ADDR_MODE_FIXED << TRANSFER_SETTINGS_DEST_ADDR_BITS))

#define SPI_B_CLK_N_DIV_MULTIPLIER        (512U)        ///< Maximum divider for N=0
#define SPI_B_CLK_MAX_DIV                 (4096U)       ///< Maximum SPI CLK divider
#define SPI_B_CLK_MIN_DIV                 (2U)          ///< Minimum SPI CLK divider

#define SPI_B_PRV_SPSRC_ALL_CLEAR         (0xFD800000U) ///< Clear all status flags

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
#if defined(__ARMCC_VERSION) || defined(__ICCARM__)
typedef void (BSP_CMSE_NONSECURE_CALL * spi_prv_ns_callback)(spi_callback_args_t * p_args);
#elif defined(__GNUC__)
typedef BSP_CMSE_NONSECURE_CALL void (*volatile spi_prv_ns_callback)(spi_callback_args_t * p_args);
#endif

/***********************************************************************************************************************
 * Private function declarations
 **********************************************************************************************************************/
static fsp_err_t r_spi_b_transfer_config(spi_cfg_t const * const p_cfg);
static void      r_spi_b_hw_config(spi_b_instance_ctrl_t * p_ctrl);
static void      r_spi_b_nvic_config(spi_b_instance_ctrl_t * p_ctrl);

static void      r_spi_b_bit_width_config(spi_b_instance_ctrl_t * p_ctrl);
static void      r_spi_b_start_transfer(spi_b_instance_ctrl_t * p_ctrl);
static fsp_err_t r_spi_b_write_read_common(spi_ctrl_t * const    p_api_ctrl,
                                           void const          * p_src,
                                           void                * p_dest,
                                           uint32_t const        length,
                                           spi_bit_width_t const bit_width);

static void r_spi_b_receive(spi_b_instance_ctrl_t * p_ctrl);
static void r_spi_b_transmit(spi_b_instance_ctrl_t * p_ctrl);
static void r_spi_b_call_callback(spi_b_instance_ctrl_t * p_ctrl, spi_event_t event);

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/
void spi_b_rxi_isr(void);
void spi_b_txi_isr(void);
void spi_b_tei_isr(void);
void spi_b_eri_isr(void);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/

/* SPI implementation of SPI interface. */
const spi_api_t g_spi_on_spi_b =
{
    .open        = R_SPI_B_Open,
    .read        = R_SPI_B_Read,
    .write       = R_SPI_B_Write,
    .writeRead   = R_SPI_B_WriteRead,
    .close       = R_SPI_B_Close,
    .callbackSet = R_SPI_B_CallbackSet
};

/*******************************************************************************************************************//**
 * @addtogroup SPI_B
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * This functions initializes a channel for SPI communication mode. Implements @ref spi_api_t::open.
 *
 * This function performs the following tasks:
 * - Performs parameter checking and processes error conditions.
 * - Configures the peripheral registers according to the configuration.
 * - Initialize the control structure for use in other @ref SPI_API functions.
 *
 * @retval     FSP_SUCCESS                     Channel initialized successfully.
 * @retval     FSP_ERR_ALREADY_OPEN            Instance was already initialized.
 * @retval     FSP_ERR_ASSERTION               An invalid argument was given in the configuration structure.
 * @retval     FSP_ERR_UNSUPPORTED             A requested setting is not possible on this device with the current build
 *                                             configuration.
 * @retval     FSP_ERR_IP_CHANNEL_NOT_PRESENT  The channel number is invalid.
 * @return     See @ref RENESAS_ERROR_CODES or functions called by this function for other possible return codes. This
 *             function calls: @ref transfer_api_t::open
 * @note       This function is reentrant.
 **********************************************************************************************************************/
fsp_err_t R_SPI_B_Open (spi_ctrl_t * p_api_ctrl, spi_cfg_t const * const p_cfg)
{
    fsp_err_t err = FSP_SUCCESS;

    spi_b_instance_ctrl_t * p_ctrl = (spi_b_instance_ctrl_t *) p_api_ctrl;

#if SPI_B_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(SPI_B_OPEN != p_ctrl->open, FSP_ERR_ALREADY_OPEN);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_callback);
    FSP_ASSERT(NULL != p_cfg->p_extend);
    FSP_ERROR_RETURN(BSP_FEATURE_SPI_MAX_CHANNEL > p_cfg->channel, FSP_ERR_IP_CHANNEL_NOT_PRESENT);
    FSP_ASSERT(p_cfg->rxi_irq >= 0);
    FSP_ASSERT(p_cfg->txi_irq >= 0);
    FSP_ASSERT(p_cfg->tei_irq >= 0);
    FSP_ASSERT(p_cfg->eri_irq >= 0);

 #if SPI_B_TRANSMIT_FROM_RXI_ISR == 1
    spi_b_extended_cfg_t * p_extend = (spi_b_extended_cfg_t *) p_cfg->p_extend;

    /* Half Duplex - Transmit Only mode is not supported when transmit interrupt is handled in the RXI ISR. */
    FSP_ERROR_RETURN(p_extend->spi_comm != SPI_B_COMMUNICATION_TRANSMIT_ONLY, FSP_ERR_UNSUPPORTED);

    /* When the TXI Interrupt is handled in the RXI ISR, a TX DTC instance must be present if there is a
     * RX DTC instance present otherwise the TXI Interrupts will not be processed. */
    if (p_cfg->p_transfer_rx)
    {
        FSP_ERROR_RETURN(0 != p_cfg->p_transfer_tx, FSP_ERR_UNSUPPORTED);
    }
 #endif
#endif

    /* Configure transfers if they are provided in p_cfg. */
    err = r_spi_b_transfer_config(p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Get the register address of the channel. */
    p_ctrl->p_cfg             = p_cfg;
    p_ctrl->p_callback        = p_cfg->p_callback;
    p_ctrl->p_context         = p_cfg->p_context;
    p_ctrl->p_callback_memory = NULL;

    p_ctrl->p_regs = SPI_B_REG(p_ctrl->p_cfg->channel);

    /* Configure hardware registers according to the r_spi_api configuration structure. */
    r_spi_b_hw_config(p_ctrl);

    /* Enable interrupts in NVIC. */
    r_spi_b_nvic_config(p_ctrl);

    p_ctrl->open = SPI_B_OPEN;

    return err;
}

/*******************************************************************************************************************//**
 * This function receives data from a SPI device. Implements @ref spi_api_t::read.
 *
 * The function performs the following tasks:
 * - Performs parameter checking and processes error conditions.
 * - Sets up the instance to complete a SPI read operation.
 *
 * @retval  FSP_SUCCESS                   Read operation successfully completed.
 * @retval  FSP_ERR_ASSERTION             NULL pointer to control or destination parameters or transfer length is zero.
 * @retval  FSP_ERR_NOT_OPEN              The channel has not been opened. Open channel first.
 * @retval  FSP_ERR_IN_USE                A transfer is already in progress.
 **********************************************************************************************************************/
fsp_err_t R_SPI_B_Read (spi_ctrl_t * const    p_api_ctrl,
                        void                * p_dest,
                        uint32_t const        length,
                        spi_bit_width_t const bit_width)
{
    return r_spi_b_write_read_common(p_api_ctrl, NULL, p_dest, length, bit_width);
}

/*******************************************************************************************************************//**
 * This function transmits data to a SPI device using the TX Only Communications Operation Mode.
 * Implements @ref spi_api_t::write.
 *
 * The function performs the following tasks:
 * - Performs parameter checking and processes error conditions.
 * - Sets up the instance to complete a SPI write operation.
 *
 * @retval  FSP_SUCCESS                     Write operation successfully completed.
 * @retval  FSP_ERR_ASSERTION               NULL pointer to control or source parameters or transfer length is zero.
 * @retval  FSP_ERR_NOT_OPEN                The channel has not been opened. Open the channel first.
 * @retval  FSP_ERR_IN_USE                  A transfer is already in progress.
 **********************************************************************************************************************/
fsp_err_t R_SPI_B_Write (spi_ctrl_t * const    p_api_ctrl,
                         void const          * p_src,
                         uint32_t const        length,
                         spi_bit_width_t const bit_width)
{
    return r_spi_b_write_read_common(p_api_ctrl, p_src, NULL, length, bit_width);
}

/*******************************************************************************************************************//**
 * This function simultaneously transmits and receive data. Implements @ref spi_api_t::writeRead.
 *
 * The function performs the following tasks:
 * - Performs parameter checking and processes error conditions.
 * - Sets up the instance to complete a SPI writeRead operation.
 *
 * @retval  FSP_SUCCESS                   Write operation successfully completed.
 * @retval  FSP_ERR_ASSERTION             NULL pointer to control, source or destination parameters or
 *                                        transfer length is zero.
 * @retval  FSP_ERR_NOT_OPEN              The channel has not been opened. Open the channel first.
 * @retval  FSP_ERR_IN_USE                A transfer is already in progress.
 *********************************************************************************************************************/
fsp_err_t R_SPI_B_WriteRead (spi_ctrl_t * const    p_api_ctrl,
                             void const          * p_src,
                             void                * p_dest,
                             uint32_t const        length,
                             spi_bit_width_t const bit_width)
{
#if SPI_B_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_src != NULL);
    FSP_ASSERT(p_dest != NULL);
#endif

    return r_spi_b_write_read_common(p_api_ctrl, p_src, p_dest, length, bit_width);
}

/*******************************************************************************************************************//**
 * Updates the user callback and has option of providing memory for callback structure.
 * Implements spi_api_t::callbackSet
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 * @retval  FSP_ERR_NO_CALLBACK_MEMORY   p_callback is non-secure and p_callback_memory is either secure or NULL.
 **********************************************************************************************************************/
fsp_err_t R_SPI_B_CallbackSet (spi_ctrl_t * const          p_api_ctrl,
                               void (                    * p_callback)(spi_callback_args_t *),
                               void const * const          p_context,
                               spi_callback_args_t * const p_callback_memory)
{
    spi_b_instance_ctrl_t * p_ctrl = (spi_b_instance_ctrl_t *) p_api_ctrl;

#if (SPI_B_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_callback);
    FSP_ERROR_RETURN(SPI_B_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

#if BSP_TZ_SECURE_BUILD

    /* Get security state of p_callback */
    bool callback_is_secure =
        (NULL == cmse_check_address_range((void *) p_callback, sizeof(void *), CMSE_AU_NONSECURE));

 #if SPI_B_CFG_PARAM_CHECKING_ENABLE

    /* In secure projects, p_callback_memory must be provided in non-secure space if p_callback is non-secure */
    spi_callback_args_t * const p_callback_memory_checked = cmse_check_pointed_object(p_callback_memory,
                                                                                      CMSE_AU_NONSECURE);
    FSP_ERROR_RETURN(callback_is_secure || (NULL != p_callback_memory_checked), FSP_ERR_NO_CALLBACK_MEMORY);
 #endif
#endif

    /* Store callback and context */
#if BSP_TZ_SECURE_BUILD
    p_ctrl->p_callback = callback_is_secure ? p_callback :
                         (void (*)(spi_callback_args_t *))cmse_nsfptr_create(p_callback);
#else
    p_ctrl->p_callback = p_callback;
#endif
    p_ctrl->p_context         = p_context;
    p_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * This function manages the closing of a channel by the following task. Implements @ref spi_api_t::close.
 *
 * Disables SPI operations by disabling the SPI bus.
 * - Disables the SPI peripheral.
 * - Disables all the associated interrupts.
 * - Update control structure so it will not work with @ref SPI_API functions.
 *
 * @retval  FSP_SUCCESS              Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION        A required pointer argument is NULL.
 * @retval  FSP_ERR_NOT_OPEN         The channel has not been opened. Open the channel first.
 **********************************************************************************************************************/
fsp_err_t R_SPI_B_Close (spi_ctrl_t * const p_api_ctrl)
{
    spi_b_instance_ctrl_t * p_ctrl = (spi_b_instance_ctrl_t *) p_api_ctrl;

#if SPI_B_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(SPI_B_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_ctrl->open = 0;

#if SPI_B_DTC_SUPPORT_ENABLE == 1
    if (NULL != p_ctrl->p_cfg->p_transfer_rx)
    {
        p_ctrl->p_cfg->p_transfer_rx->p_api->close(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
    }

    if (NULL != p_ctrl->p_cfg->p_transfer_tx)
    {
        p_ctrl->p_cfg->p_transfer_tx->p_api->close(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
    }
#endif

    /* Disable interrupts in NVIC. */
    R_BSP_IrqDisable(p_ctrl->p_cfg->txi_irq);
    R_BSP_IrqDisable(p_ctrl->p_cfg->rxi_irq);
    R_BSP_IrqDisable(p_ctrl->p_cfg->tei_irq);
    R_BSP_IrqDisable(p_ctrl->p_cfg->eri_irq);

    /* Disable the SPI Transfer. */
    p_ctrl->p_regs->SPCR_b.SPE = 0U;

    /* Clear the status register. */
    p_ctrl->p_regs->SPSRC = SPI_B_PRV_SPSRC_ALL_CLEAR;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Calculates the SPBR register value and the BRDV bits for a desired bitrate.
 * If the desired bitrate is faster than the maximum bitrate, than the bitrate is set to the
 * maximum bitrate. If the desired bitrate is slower than the minimum bitrate, than an error is returned.
 *
 * @param[in] bitrate             Desired bitrate
 * @param[in] clock_source        SPI communication clock source to be used
 * @param[out] spck_div           Memory location to store bitrate register settings.
 *
 * @retval FSP_SUCCESS            Valid spbr and brdv values were calculated
 * @retval FSP_ERR_UNSUPPORTED    Bitrate is not achievable
 **********************************************************************************************************************/
fsp_err_t R_SPI_B_CalculateBitrate (uint32_t bitrate, spi_b_clock_source_t clock_source, rspck_div_setting_t * spck_div)
{
    /* desired_divider = Smallest integer greater than or equal to TCLK / bitrate. */
    uint32_t desired_divider;
    if (SPI_B_CLOCK_SOURCE_PCLK == clock_source)
    {
        desired_divider = R_FSP_SystemClockHzGet(BSP_FEATURE_SPI_CLK);
    }
    else
    {
#if BSP_FEATURE_BSP_HAS_SCISPI_CLOCK
        desired_divider = R_FSP_SciSpiClockHzGet();
#elif BSP_FEATURE_BSP_HAS_SPI_CLOCK
        desired_divider = R_FSP_SpiClockHzGet();
#endif
    }

    desired_divider = (desired_divider + bitrate - 1) / bitrate;

    /* Can't achieve bitrate slower than desired. */
    if (desired_divider > SPI_B_CLK_MAX_DIV)
    {
        return FSP_ERR_UNSUPPORTED;
    }

    if (desired_divider < SPI_B_CLK_MIN_DIV)
    {
        /* Configure max bitrate (TCLK / 2) */
        spck_div->brdv = 0;
        spck_div->spbr = 0;

        return FSP_SUCCESS;
    }

    /*
     * Possible TCLK dividers for values of N:
     *   N = 0; div = [2,4,6,..,512]
     *   N = 1; div = [4,8,12,..,1024]
     *   N = 2; div = [8,16,32,..,2048]
     *   N = 3; div = [16,32,64,..,4096]
     */
    uint8_t i;
    for (i = 0; i < 4; i++)
    {
        /* Select smallest value for N possible. */

        /* div <= 512; N = 0
         * 512 < div <= 1024; N=1
         * ...
         */
        if (desired_divider <= (SPI_B_CLK_N_DIV_MULTIPLIER << i))
        {
            break;
        }
    }

    spck_div->brdv = i & 0x03U;

    /*
     * desired_divider = 2 * (spbr + 1) * 2^i.
     *
     * With desired_divider and i known, solve for spbr.
     *
     * spbr = TCLK_DIV / (2 * 2^i) - 1
     */
    uint32_t spbr_divisor = (2U * (1U << i));

    /* spbr = (Smallest integer greater than or equal to TCLK_DIV / (2 * 2^i)) - 1. */
    spck_div->spbr = (uint8_t) (((desired_divider + spbr_divisor - 1U) / spbr_divisor) - 1U) & UINT8_MAX;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup SPI_B)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Configure the given transfer instances for receiving and transmitting data without CPU intervention.
 *
 * @param      p_cfg           Configuration structure with references to receive and transmit transfer instances.
 *
 * @retval     FSP_SUCCESS     The given transfer instances were configured successfully.
 * @return                     See @ref RENESAS_ERROR_CODES for other possible return codes. This function internally
 *                             calls @ref transfer_api_t::open.
 **********************************************************************************************************************/
static fsp_err_t r_spi_b_transfer_config (spi_cfg_t const * const p_cfg)
{
    fsp_err_t err = FSP_SUCCESS;

#if SPI_B_DTC_SUPPORT_ENABLE == 1
    const transfer_instance_t * p_transfer_tx = p_cfg->p_transfer_tx;
    void * p_spdr = (void *) &(SPI_B_REG(p_cfg->channel)->SPDR);
    if (p_transfer_tx)
    {
        p_transfer_tx->p_cfg->p_info->transfer_settings_word = SPI_B_DTC_TX_TRANSFER_SETTINGS;
        p_transfer_tx->p_cfg->p_info->p_dest                 = p_spdr;

        err = p_transfer_tx->p_api->open(p_transfer_tx->p_ctrl, p_transfer_tx->p_cfg);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    const transfer_instance_t * p_transfer_rx = p_cfg->p_transfer_rx;
    if (p_transfer_rx)
    {
        p_transfer_rx->p_cfg->p_info->transfer_settings_word = SPI_B_DTC_RX_TRANSFER_SETTINGS;
        p_transfer_rx->p_cfg->p_info->p_src = p_spdr;

        err = p_transfer_rx->p_api->open(p_transfer_rx->p_ctrl, p_transfer_rx->p_cfg);

        if ((FSP_SUCCESS != err) && p_transfer_tx)
        {
            p_transfer_tx->p_api->close(p_transfer_tx->p_ctrl);
        }
    }

#else
    FSP_PARAMETER_NOT_USED(p_cfg);
#endif

    return err;
}

/*******************************************************************************************************************//**
 * Hardware configuration for settings given by the configuration structure.
 *
 * @param[in]  p_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_spi_b_hw_config (spi_b_instance_ctrl_t * p_ctrl)
{
    uint32_t spcr   = 0;
    uint32_t spcr2  = 0;
    uint32_t spcr3  = 0;
    uint32_t spdecr = 0;
    uint32_t spcmd0 = 0;
    uint32_t spdcr  = 0;

#if SPI_B_TRANSMIT_FROM_RXI_ISR
    if (p_ctrl->p_cfg->p_transfer_tx)
#endif
    {
        /* Transmit Buffer Empty IRQ must be enabled for DTC even if TRANSMIT_FROM_RXI is enabled. */
        spcr |= R_SPI_B0_SPCR_SPTIE_Msk;
    }

    /* Enable Error interrupt. */
    spcr |= R_SPI_B0_SPCR_SPEIE_Msk;

    /* Enable Communication End interrupt. */
    spcr |= R_SPI_B0_SPCR_CENDIE_Msk;

    /* Configure Master Mode setting. */
    spcr |= (uint32_t) (SPI_MODE_MASTER == p_ctrl->p_cfg->operating_mode) << R_SPI_B0_SPCR_MSTR_Pos;

    /* Enable SCK Auto Stop setting in order to prevent RX Overflow in Master Mode */
    spcr |= (uint32_t) (SPI_MODE_MASTER == p_ctrl->p_cfg->operating_mode) << R_SPI_B0_SPCR_SCKASE_Pos;

    /* Configure CPHA setting. */
    spcmd0 |= (uint32_t) p_ctrl->p_cfg->clk_phase << R_SPI_B0_SPCMD0_CPHA_Pos;

    /* Configure CPOL setting. */
    spcmd0 |= (uint32_t) p_ctrl->p_cfg->clk_polarity << R_SPI_B0_SPCMD0_CPOL_Pos;

    /* Configure Bit Order (MSB,LSB) */
    spcmd0 |= (uint32_t) p_ctrl->p_cfg->bit_order << R_SPI_B0_SPCMD0_LSBF_Pos;

    spi_b_extended_cfg_t * p_extend = ((spi_b_extended_cfg_t *) p_ctrl->p_cfg->p_extend);

    /* Set clock source */
    spcr |= (uint32_t) (p_extend->clock_source << R_SPI_B0_SPCR_BPEN_Pos);

    if (SPI_B_SSL_MODE_SPI == p_extend->spi_clksyn)
    {
        /* Configure SSL Level Keep Setting. */
        spcmd0 |= (uint32_t) (!p_ctrl->p_cfg->operating_mode << R_SPI_B0_SPCMD0_SSLKP_Pos);
    }
    else
    {
        /* Configure 3-Wire Mode Setting. */
        spcr |= R_SPI_B0_SPCR_SPMS_Msk;
    }

    if (SPI_B_COMMUNICATION_FULL_DUPLEX == p_extend->spi_comm)
    {
        /* Enable Receive Buffer Full interrupt. */
        spcr |= R_SPI_B0_SPCR_SPRIE_Msk;
    }

    /* Set TX Only or Full Duplex */
    spcr |= (uint32_t) (p_extend->spi_comm << R_SPI_B0_SPCR_TXMD_Pos);

    /* Configure SSLn polarity setting. */
    spcr3 &= ~0x0FU;
    spcr3 |= (uint32_t) p_extend->ssl_polarity << p_extend->ssl_select;

    /* Configure bitrate */
    spcr3 |= ((uint32_t) p_extend->spck_div.spbr) << R_SPI_B0_SPCR3_SPBR_Pos;

    /* Configure SSLn setting. (SSL0, SSL1, SSL2, SSL3)*/
    spcmd0 &= ~R_SPI_B0_SPCMD0_SSLA_Msk;
    spcmd0 |= (uint32_t) p_extend->ssl_select << R_SPI_B0_SPCMD0_SSLA_Pos;

    if (SPI_B_MOSI_IDLE_VALUE_FIXING_DISABLE != p_extend->mosi_idle)
    {
        /* Enable MOSI value fixing */
        spcr2 |= R_SPI_B0_SPCR2_MOIFE_Msk;

        if (SPI_B_MOSI_IDLE_VALUE_FIXING_HIGH == p_extend->mosi_idle)
        {
            spcr2 |= R_SPI_B0_SPCR2_MOIFV_Msk;
        }
    }

    if (SPI_B_PARITY_MODE_DISABLE != p_extend->parity)
    {
        /* Enable Parity Mode. */
        spcr |= R_SPI_B0_SPCR_SPPE_Msk;

        if (SPI_B_PARITY_MODE_ODD == p_extend->parity)
        {
            /* Configure ODD Parity Setting. */
            spcr |= R_SPI_B0_SPCR_SPOE_Msk;
        }
    }

    /* Configure byte swapping for 16/32-Bit mode. */
    spdcr |= p_extend->byte_swap;

    /* Configure the Bit Rate Division Setting */
    spcmd0 |= (uint32_t) p_extend->spck_div.brdv << R_SPI_B0_SPCMD0_BRDV_Pos;

    /* Enable all delay settings. */
    if (SPI_MODE_MASTER == p_ctrl->p_cfg->operating_mode)
    {
        /* Note that disabling delay settings is same as setting delay to 1. */
        spcmd0 |= (uint32_t) R_SPI_B0_SPCMD0_SPNDEN_Msk | R_SPI_B0_SPCMD0_SLNDEN_Msk | R_SPI_B0_SPCMD0_SCKDEN_Msk;

        spdecr = (uint32_t) ((p_extend->spck_delay << R_SPI_B0_SPDECR_SCKDL_Pos) |
                             (p_extend->ssl_negation_delay << R_SPI_B0_SPDECR_SLNDL_Pos) |
                             (p_extend->next_access_delay << R_SPI_B0_SPDECR_SPNDL_Pos));
    }

    /* Enable the SPI module. */
    R_BSP_MODULE_START(FSP_IP_SPI, p_ctrl->p_cfg->channel);

    /* Clear the status register. */
    p_ctrl->p_regs->SPSRC = SPI_B_PRV_SPSRC_ALL_CLEAR;

    /* Write registers */
    p_ctrl->p_regs->SPCR3  = spcr3;
    p_ctrl->p_regs->SPDECR = spdecr;
    p_ctrl->p_regs->SPCR2  = spcr2;
    p_ctrl->p_regs->SPCMD0 = spcmd0;
    p_ctrl->p_regs->SPDCR  = spdcr;

    /* Set all SPCR bits except MSTR */
    p_ctrl->p_regs->SPCR = spcr & ~R_SPI_B0_SPCR_MSTR_Msk;

    /* Read back SPCR to ensure 1 TCLK has passed
     * (see Figure 30.64 Note 1 in the RA6T2 User's Manual (R01UH0951EJ0100)) */
    p_ctrl->p_regs->SPCR;

    /* Include MSTR bit */
    p_ctrl->p_regs->SPCR = spcr;
}

/*******************************************************************************************************************//**
 * Enable Receive Buffer Full, Transmit Buffer Empty, and Error Interrupts in the NVIC.
 *
 * @param[in]  p_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_spi_b_nvic_config (spi_b_instance_ctrl_t * p_ctrl)
{
    R_BSP_IrqCfgEnable(p_ctrl->p_cfg->txi_irq, p_ctrl->p_cfg->txi_ipl, p_ctrl);
    R_BSP_IrqCfgEnable(p_ctrl->p_cfg->rxi_irq, p_ctrl->p_cfg->rxi_ipl, p_ctrl);
    R_BSP_IrqCfgEnable(p_ctrl->p_cfg->eri_irq, p_ctrl->p_cfg->eri_ipl, p_ctrl);

    R_BSP_IrqCfg(p_ctrl->p_cfg->tei_irq, p_ctrl->p_cfg->tei_ipl, p_ctrl);

    /* Note tei_irq is not enabled until the last data frame is transfered. */
}

/*******************************************************************************************************************//**
 * Setup the bit width configuration for a transfer.
 *
 * @param[in]  p_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_spi_b_bit_width_config (spi_b_instance_ctrl_t * p_ctrl)
{
    uint32_t spcmd0 = p_ctrl->p_regs->SPCMD0;

    /* Configure data length based on the selected bit width . */
    spcmd0 &= ~R_SPI_B0_SPCMD0_SPB_Msk;
    spcmd0 |= (uint32_t) (p_ctrl->bit_width) << R_SPI_B0_SPCMD0_SPB_Pos;

    p_ctrl->p_regs->SPCMD0 = spcmd0;
}

/*******************************************************************************************************************//**
 * Initiates a SPI transfer by setting the SPE bit in SPCR.
 *
 * @param[in]  p_ctrl          pointer to control structure.
 *
 * Note: When not using the DTC to transmit, this function pre-loads the SPI shift-register and shift-register-buffer
 * instead of waiting for the transmit buffer empty interrupt. This is required when transmitting from the
 * Receive Buffer Full interrupt, but it does not interfere with transmitting when using the transmit buffer empty
 * interrupt.
 **********************************************************************************************************************/
static void r_spi_b_start_transfer (spi_b_instance_ctrl_t * p_ctrl)
{
    /* Clear FIFOs */
    p_ctrl->p_regs->SPFCR = 1;

#if SPI_B_TRANSMIT_FROM_RXI_ISR == 1
    if (!p_ctrl->p_cfg->p_transfer_tx)
    {
        /* Handle the first two transmit empty events here because transmit interrupt may not be enabled. */

        /* Critical section required so that the txi interrupt can be handled here instead of in the ISR. */
        FSP_CRITICAL_SECTION_DEFINE;
        FSP_CRITICAL_SECTION_ENTER;

        /* Enable the SPI Transfer. */
        p_ctrl->p_regs->SPCR_b.SPE = 1;

        /* Must call transmit to kick off transfer when transmitting from rxi ISR. */
        r_spi_b_transmit(p_ctrl);      ///< First data immediately copied into the SPI shift register.

        /* Second transmit significantly improves slave mode performance. */
        r_spi_b_transmit(p_ctrl);      ///< Second data copied into the SPI transmit buffer.

        /* Must clear the txi IRQ status (The interrupt was handled here). */
        R_BSP_IrqEnable(p_ctrl->p_cfg->txi_irq);

        FSP_CRITICAL_SECTION_EXIT;
    }
    else
    {
        /* Enable the SPI Transfer. */
        p_ctrl->p_regs->SPCR_b.SPE = 1;
    }

#else

    /* Enable the SPI Transfer. */
    p_ctrl->p_regs->SPCR_b.SPE = 1;
#endif
}

/*******************************************************************************************************************//**
 * Configures the driver state and initiates a SPI transfer for all modes of operation.
 *
 * @param[in]  p_api_ctrl        pointer to control structure.
 * @param      p_src             Buffer to transmit data from.
 * @param      p_dest            Buffer to store received data in.
 * @param[in]  length            Number of transfers
 * @param[in]  bit_width         Data frame size (8-Bit, 16-Bit, 32-Bit)
 *
 * @retval     FSP_SUCCESS       Transfer was started successfully.
 * @retval     FSP_ERR_ASSERTION An argument is invalid.
 * @retval     FSP_ERR_NOT_OPEN  The instance has not been initialized.
 * @retval     FSP_ERR_IN_USE    A transfer is already in progress.
 * @return                       See @ref RENESAS_ERROR_CODES for other possible return codes. This function internally
 *                               calls @ref transfer_api_t::reconfigure.
 **********************************************************************************************************************/
static fsp_err_t r_spi_b_write_read_common (spi_ctrl_t * const    p_api_ctrl,
                                            void const          * p_src,
                                            void                * p_dest,
                                            uint32_t const        length,
                                            spi_bit_width_t const bit_width)
{
    spi_b_instance_ctrl_t * p_ctrl = (spi_b_instance_ctrl_t *) p_api_ctrl;

#if SPI_B_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(SPI_B_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(p_src || p_dest);
    FSP_ASSERT(0 != length);
    if (p_ctrl->p_cfg->p_transfer_tx || p_ctrl->p_cfg->p_transfer_rx)
    {
        FSP_ASSERT(length <= SPI_B_DTC_MAX_TRANSFER);
    }
#endif

    /* Check to ensure SPE is cleared after the last transmission.
     * (see RA6T2 User's Manual (R01UH0951EJ0100) Figure 30.64 "Transmission flow in master mode") */
    FSP_ERROR_RETURN(0 == p_ctrl->p_regs->SPPSR, FSP_ERR_IN_USE);

    p_ctrl->p_tx_data = p_src;
    p_ctrl->p_rx_data = p_dest;
    p_ctrl->tx_count  = 0;
    p_ctrl->rx_count  = 0;
    p_ctrl->count     = length;
    p_ctrl->bit_width = bit_width;

#if SPI_B_DTC_SUPPORT_ENABLE == 1

    /* Determine DTC transfer size */
    transfer_size_t size;
    if (p_ctrl->bit_width > SPI_BIT_WIDTH_16_BITS)     /* Bit Widths of 17-32 bits */
    {
        size = TRANSFER_SIZE_4_BYTE;
    }
    else if (p_ctrl->bit_width > SPI_BIT_WIDTH_8_BITS) /* Bit Widths of 9-16 bits*/
    {
        size = TRANSFER_SIZE_2_BYTE;
    }
    else                                               /* Bit Widths of 4-8 bits */
    {
        size = TRANSFER_SIZE_1_BYTE;
    }

    if (p_ctrl->p_cfg->p_transfer_rx)
    {
        /* When the rxi interrupt is called, all transfers will be finished. */
        p_ctrl->rx_count = length;

        transfer_instance_t * p_transfer_rx = (transfer_instance_t *) p_ctrl->p_cfg->p_transfer_rx;
        transfer_info_t     * p_info        = p_transfer_rx->p_cfg->p_info;

        /* Configure the receive DMA instance. */
        p_info->transfer_settings_word_b.size = size;
        p_info->length = (uint16_t) length;
        p_info->transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED;
        p_info->p_dest = p_dest;

        if (NULL == p_dest)
        {
            static uint32_t dummy_rx;
            p_info->transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED;
            p_info->p_dest = &dummy_rx;
        }

        fsp_err_t err = p_transfer_rx->p_api->reconfigure(p_transfer_rx->p_ctrl, p_info);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    if (p_ctrl->p_cfg->p_transfer_tx)
    {
        /* When the txi interrupt is called, all transfers will be finished. */
        p_ctrl->tx_count = length;

        transfer_instance_t * p_transfer_tx = (transfer_instance_t *) p_ctrl->p_cfg->p_transfer_tx;
        transfer_info_t     * p_info        = p_transfer_tx->p_cfg->p_info;

        /* Configure the transmit DMA instance. */
        p_info->transfer_settings_word_b.size = size;
        p_info->length = (uint16_t) length;
        p_info->transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED;
        p_info->p_src = p_src;

        if (NULL == p_src)
        {
            static uint32_t dummy_tx = 0;
            p_info->transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED;
            p_info->p_src = &dummy_tx;
        }

        fsp_err_t err = p_transfer_tx->p_api->reconfigure(p_transfer_tx->p_ctrl, p_info);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
#endif

    r_spi_b_bit_width_config(p_ctrl);
    r_spi_b_start_transfer(p_ctrl);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Copy configured bit width from the SPI data register to the current rx data location.
 * If the receive buffer is NULL, just read the SPI data register.
 * If the total transfer length has already been received than do nothing.
 *
 * @param[in]  p_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_spi_b_receive (spi_b_instance_ctrl_t * p_ctrl)
{
    uint32_t rx_count = p_ctrl->rx_count;
    if (rx_count == p_ctrl->count)
    {
        return;
    }

    if (0 == p_ctrl->p_rx_data)
    {
        /* Read the received data but do nothing with it. */
        p_ctrl->p_regs->SPDR;
    }
    else
    {
        if (p_ctrl->bit_width > SPI_BIT_WIDTH_16_BITS)     /* Bit Widths of 17-32 bits */
        {
            ((uint32_t *) (p_ctrl->p_rx_data))[rx_count] = p_ctrl->p_regs->SPDR;
        }
        else if (p_ctrl->bit_width > SPI_BIT_WIDTH_8_BITS) /* Bit Widths of 9-16 bits*/
        {
            ((uint16_t *) (p_ctrl->p_rx_data))[rx_count] = (uint16_t) p_ctrl->p_regs->SPDR;
        }
        else                                               /* Bit Widths of 4-8 bits */
        {
            ((uint8_t *) (p_ctrl->p_rx_data))[rx_count] = (uint8_t) p_ctrl->p_regs->SPDR;
        }
    }

    /* Clear Receive Full flag */
    p_ctrl->p_regs->SPSRC = R_SPI_B0_SPSRC_SPRFC_Msk;

    p_ctrl->rx_count = rx_count + 1;
}

/*******************************************************************************************************************//**
 * Copy configured bit width from the current tx data location into the SPI data register.
 * If the transmit buffer is NULL, than write zero to the SPI data register.
 * If the total transfer length has already been transmitted than do nothing.
 *
 * @param[in]  p_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_spi_b_transmit (spi_b_instance_ctrl_t * p_ctrl)
{
    uint32_t tx_count = p_ctrl->tx_count;
    if (tx_count == p_ctrl->count)
    {
        return;
    }

    if (0 == p_ctrl->p_tx_data)
    {
        /* Transmit zero if no tx buffer present. */
        p_ctrl->p_regs->SPDR = 0;
    }
    else
    {
        if (p_ctrl->bit_width > SPI_BIT_WIDTH_16_BITS)     /* Bit Widths of 17-32 bits */
        {
            p_ctrl->p_regs->SPDR = ((uint32_t *) p_ctrl->p_tx_data)[tx_count];
        }
        else if (p_ctrl->bit_width > SPI_BIT_WIDTH_8_BITS) /* Bit Widths of 9-16 bits*/
        {
            p_ctrl->p_regs->SPDR = ((uint16_t *) p_ctrl->p_tx_data)[tx_count];
        }
        else                                               /* Bit Widths of 4-8 bits */
        {
            p_ctrl->p_regs->SPDR = ((uint8_t *) p_ctrl->p_tx_data)[tx_count];
        }
    }

    /* Clear Transmit Empty flag */
    p_ctrl->p_regs->SPSRC = R_SPI_B0_SPSRC_SPTEFC_Msk;

    p_ctrl->tx_count = tx_count + 1;
}

/*******************************************************************************************************************//**
 * Calls user callback.
 *
 * @param[in]     p_ctrl     Pointer to SPI instance control block
 * @param[in]     event      Event code
 **********************************************************************************************************************/
static void r_spi_b_call_callback (spi_b_instance_ctrl_t * p_ctrl, spi_event_t event)
{
    spi_callback_args_t args;

    /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
     * stored in non-secure memory so they can be accessed by a non-secure callback function. */
    spi_callback_args_t * p_args = p_ctrl->p_callback_memory;
    if (NULL == p_args)
    {
        /* Store on stack */
        p_args = &args;
    }
    else
    {
        /* Save current arguments on the stack in case this is a nested interrupt. */
        args = *p_args;
    }

    p_args->channel   = p_ctrl->p_cfg->channel;
    p_args->event     = event;
    p_args->p_context = p_ctrl->p_context;

#if BSP_TZ_SECURE_BUILD

    /* p_callback can point to a secure function or a non-secure function. */
    if (!cmse_is_nsfptr(p_ctrl->p_callback))
    {
        /* If p_callback is secure, then the project does not need to change security state. */
        p_ctrl->p_callback(p_args);
    }
    else
    {
        /* If p_callback is Non-secure, then the project must change to Non-secure state in order to call the callback. */
        spi_prv_ns_callback p_callback = (spi_prv_ns_callback) (p_ctrl->p_callback);
        p_callback(p_args);
    }

#else

    /* If the project is not Trustzone Secure, then it will never need to change security state in order to call the callback. */
    p_ctrl->p_callback(p_args);
#endif
    if (NULL != p_ctrl->p_callback_memory)
    {
        /* Restore callback memory in case this is a nested interrupt. */
        *p_ctrl->p_callback_memory = args;
    }
}

/*******************************************************************************************************************//**
 * ISR called when data is loaded into SPI data register from the shift register.
 **********************************************************************************************************************/
void spi_b_rxi_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    spi_b_instance_ctrl_t * p_ctrl = (spi_b_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    r_spi_b_receive(p_ctrl);

#if SPI_B_TRANSMIT_FROM_RXI_ISR == 1

    /* It is a little faster to handle the transmit buffer empty event in the receive buffer full ISR.
     * Note that this is only possible when the instance is not using a transfer instance to receive data. */
    r_spi_b_transmit(p_ctrl);
#endif

    if (p_ctrl->rx_count == p_ctrl->count)
    {
        /* If the transmit and receive ISRs are too slow to keep up at high bitrates,
         * the hardware will generate an interrupt before all of the transfers are completed.
         * By enabling the transfer end ISR here, all of the transfers are guaranteed to be completed. */
        R_BSP_IrqEnableNoClear(p_ctrl->p_cfg->tei_irq);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * ISR called when data is copied from the SPI data register into the SPI shift register.
 **********************************************************************************************************************/
void spi_b_txi_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

#if SPI_B_TRANSMIT_FROM_RXI_ISR == 0
    spi_b_instance_ctrl_t * p_ctrl = (spi_b_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    spi_b_extended_cfg_t * p_extend = ((spi_b_extended_cfg_t *) p_ctrl->p_cfg->p_extend);

    if (SPI_B_COMMUNICATION_TRANSMIT_ONLY == p_extend->spi_comm)
    {
        /* Only enable the transfer end ISR if there are no receive buffer full interrupts expected to be handled
         * after this interrupt. */
        if (p_ctrl->tx_count == p_ctrl->count - 1)
        {
            /* If the transmit and receive ISRs are too slow to keep up at high bitrates,
             * the hardware will generate an interrupt before all of the transfers are completed.
             * By enabling the transfer end ISR here, all of the transfers are guaranteed to be completed. */
            R_BSP_IrqEnable(p_ctrl->p_cfg->tei_irq);
        }
        else if (p_ctrl->p_cfg->p_transfer_tx)
        {
            /* If DMA is used to transmit data, enable the interrupt after all the data has been transfered, but do not
             * clear the IRQ Pending Bit. */
            R_BSP_IrqEnableNoClear(p_ctrl->p_cfg->tei_irq);
        }
        else
        {
        }
    }

    /* Transmit happens after checking if the last transfer has been written to the transmit buffer in order
     * to ensure that the end interrupt is not enabled while there is data still in the transmit buffer. */
    r_spi_b_transmit(p_ctrl);
#endif

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * ISR called when the SPI peripheral transitions from the transferring state to the IDLE state.
 **********************************************************************************************************************/
void spi_b_tei_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    spi_b_instance_ctrl_t * p_ctrl = (spi_b_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    if ((0 == p_ctrl->p_regs->SPSR_b.IDLNF) || (SPI_MODE_SLAVE == p_ctrl->p_cfg->operating_mode))
    {
        R_BSP_IrqDisable(irq);

        /* Writing 0 to SPE generatates a TXI IRQ. Disable the TXI IRQ.
         * (See Section 38.2.1 SPI Control Register in the RA6T2 manual R01UH0886EJ0100). */
        R_BSP_IrqDisable(p_ctrl->p_cfg->txi_irq);

        /* Disable the SPI Transfer. */
        p_ctrl->p_regs->SPCR_b.SPE = 0;

        /* Re-enable the TXI IRQ and clear the pending IRQ. */
        R_BSP_IrqEnable(p_ctrl->p_cfg->txi_irq);

        /* Signal that a transfer has completed. */
        r_spi_b_call_callback(p_ctrl, SPI_EVENT_TRANSFER_COMPLETE);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * ISR called in the event that an error occurs (Ex: RX_OVERFLOW).
 **********************************************************************************************************************/
void spi_b_eri_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type               irq    = R_FSP_CurrentIrqGet();
    spi_b_instance_ctrl_t * p_ctrl = (spi_b_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    /* Writing 0 to SPE generatates a TXI IRQ. Disable the TXI IRQ.
     * (See Section 38.2.1 SPI Control Register in the RA6T2 manual R01UH0886EJ0100). */
    R_BSP_IrqDisable(p_ctrl->p_cfg->txi_irq);

    /* Disable the SPI Transfer. */
    p_ctrl->p_regs->SPCR_b.SPE = 0;

    /* Re-enable the TXI IRQ and clear the pending IRQ. */
    R_BSP_IrqEnable(p_ctrl->p_cfg->txi_irq);

    /* Read the status register. */
    uint32_t status = p_ctrl->p_regs->SPSR;

    /* Clear the status register. */
    p_ctrl->p_regs->SPSRC = SPI_B_PRV_SPSRC_ALL_CLEAR;

    /* Check if the error is a Parity Error. */
    if (R_SPI_B0_SPSR_PERF_Msk & status)
    {
        r_spi_b_call_callback(p_ctrl, SPI_EVENT_ERR_PARITY);
    }

    /* Check if the error is a Receive Buffer Overflow Error. */
    if (R_SPI_B0_SPSR_OVRF_Msk & status)
    {
        r_spi_b_call_callback(p_ctrl, SPI_EVENT_ERR_READ_OVERFLOW);
    }

    /* Check if the error is a Mode Fault Error. */
    if (R_SPI_B0_SPSR_MODF_Msk & status)
    {
        /* Check if the error is a Transmit Buffer Underflow Error. */
        if (R_SPI_B0_SPSR_UDRF_Msk & status)
        {
            r_spi_b_call_callback(p_ctrl, SPI_EVENT_ERR_MODE_UNDERRUN);
        }
    }

    R_BSP_IrqStatusClear(irq);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/* End of file R_SPI. */
