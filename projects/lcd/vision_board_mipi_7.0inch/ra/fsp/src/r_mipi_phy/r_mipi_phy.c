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
#include "r_mipi_phy.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define MIPI_PHY_OPEN        (0x4D504950)

#define MIPI_DSI_PRV_1MHZ    (1000000)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/

const mipi_phy_api_t g_mipi_phy =
{
    .open  = r_mipi_phy_open,
    .close = r_mipi_phy_close,
};

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup MIPI_PHY
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Initialize the MIPI DSI peripheral according to section 57.3.1 D-PHY Start-up Procedure, R01UH0995EJ0060
 *
 * @retval FSP_SUCCESS                  The channel was successfully opened.
 * @retval FSP_ERR_ASSERTION            One or both of the parameters was NULL.
 * @retval FSP_ERR_ALREADY_OPEN         The instance is already opened.
 * @retval FSP_ERR_INVALID_MODE         Power control mode must be high-speed-mode.
 **********************************************************************************************************************/
fsp_err_t r_mipi_phy_open (mipi_phy_ctrl_t * const p_api_ctrl, mipi_phy_cfg_t const * const p_cfg)
{
    mipi_phy_ctrl_t * p_ctrl = p_api_ctrl;

    /* Set DPHYREFCR to match PCLKA */
    uint32_t pclka_hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKA);
    R_DPHYCNT->DPHYREFCR_b.RFREQ = (uint8_t) ((pclka_hz / MIPI_DSI_PRV_1MHZ) - 1);

    /* Start D-PHY LDO and wait for startup */
    R_DPHYCNT->DPHYPWRCR_b.PWRSEN = 1;
    FSP_HARDWARE_REGISTER_WAIT((R_DPHYCNT->DPHYSFR & R_DPHYCNT_DPHYSFR_PWRSF_Msk), R_DPHYCNT_DPHYSFR_PWRSF_Msk);

    /* Configure D-PHY PLL and LP frequency divider */
    R_DPHYCNT->DPHYPLFCR          = p_cfg->pll_settings.raw;
    R_DPHYCNT->DPHYESCCR_b.ESCDIV = p_cfg->lp_divisor;

    /* Start D-PHY PLL and wait for startup */
    R_DPHYCNT->DPHYPLOCR_b.PLLSTP = 0;
    FSP_HARDWARE_REGISTER_WAIT((R_DPHYCNT->DPHYSFR & R_DPHYCNT_DPHYSFR_PLLSF_Msk), R_DPHYCNT_DPHYSFR_PLLSF_Msk);

    /* Set D-PHY timing parameters */
    mipi_phy_timing_t const * p_timing = p_cfg->p_timing;
    R_DPHYCNT->DPHYTIM1 = p_timing->t_init;
    R_DPHYCNT->DPHYTIM2 = p_timing->t_clk_prep;
    R_DPHYCNT->DPHYTIM3 = p_timing->t_hs_prep;
    R_DPHYCNT->DPHYTIM4 = p_timing->dphytim4;
    R_DPHYCNT->DPHYTIM5 = p_timing->dphytim5;
    R_DPHYCNT->DPHYTIM6 = p_timing->t_lp_exit;

    /* Enable D-PHY */
    R_DPHYCNT->DPHYOCR_b.DPHYEN = 1U;

    /* Keep track of p_cfg struct */
    p_ctrl->p_cfg = p_cfg;

    /* Mark control block as opened */
    p_ctrl->open = MIPI_PHY_OPEN;

    return FSP_SUCCESS;
}

/**********************************************************************************************************************
 * Stop filter operations and close the channel instance.
 *
 * NOTE: D_PHY must be stopped and LDO disabled before entering standby mode. See 56.3.2 D-PHY Stop Procedure, R01UH0995EJ0060
 *
 * @retval   FSP_SUCCESS           The channel is successfully closed.
 * @retval   FSP_ERR_ASSERTION     p_api_ctrl is NULL.
 * @retval   FSP_ERR_NOT_OPEN      Instance is not open.
 **********************************************************************************************************************/
fsp_err_t r_mipi_phy_close (mipi_phy_ctrl_t * const p_api_ctrl)
{
    mipi_phy_ctrl_t * p_ctrl = p_api_ctrl;

    /* Disable D-PHY */
    R_DPHYCNT->DPHYOCR_b.DPHYEN = 0;

    /* Disable D-PHY PLL */
    R_DPHYCNT->DPHYPLOCR_b.PLLSTP = 1;

    /* Disable MIPI LDO */
    R_DPHYCNT->DPHYPWRCR_b.PWRSEN = 0;

    /* Set control block to closed */
    p_ctrl->open = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup MIPI_PHY)
 **********************************************************************************************************************/
