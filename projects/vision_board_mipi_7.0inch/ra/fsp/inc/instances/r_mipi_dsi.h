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

#ifndef R_MIPI_DSI_H
#define R_MIPI_DSI_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#include "bsp_api.h"
#include "r_mipi_dsi_cfg.h"
#include "r_mipi_dsi_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/*******************************************************************************************************************//**
 * @addtogroup MIPI_DSI
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** MIPI DSI interrupt configuration */
typedef struct st_mipi_dsi_irq_cfg
{
    uint8_t   ipl;                     ///< Interrupt priority
    IRQn_Type irq;                     ///< Interrupt vector number
} mipi_dsi_irq_cfg_t;

/** Extended configuration structure for MIPI DSI. */
typedef struct st_mipi_dsi_extended_cfg
{
    /* Interrupt configuration */
    mipi_dsi_irq_cfg_t dsi_seq0;       ///< Sequence 0 interrupt
    mipi_dsi_irq_cfg_t dsi_seq1;       ///< Sequence 1 interrupt
    mipi_dsi_irq_cfg_t dsi_ferr;       ///< DSI Fatal Error interrupt
    mipi_dsi_irq_cfg_t dsi_ppi;        ///< D-PHY PPI interrupt
    mipi_dsi_irq_cfg_t dsi_rcv;        ///< Receive interrupt
    mipi_dsi_irq_cfg_t dsi_vin1;       ///< Video Input Operation interrupt

    uint32_t dsi_rxie;                 ///< Receive interrupt enable configuration
    uint32_t dsi_ferrie;               ///< Fatal error interrupt enable configuration
    uint32_t dsi_plie;                 ///< Physical lane interrupt enable configuration
    uint32_t dsi_vmie;                 ///< Video mode interrupt enable configuration
    uint32_t dsi_sqch0ie;              ///< Sequence Channel 0 interrupt enable configuration
    uint32_t dsi_sqch1ie;              ///< Sequence Channel 1 interrupt enable configuration
} mipi_dsi_extended_cfg_t;

/** MIPI DSI instance control block. */
typedef struct st_mipi_dsi_instance_ctrl
{
    uint32_t               open;                     ///< Interface is open
    bool                   data_ulps_active;         ///< Data lane ULPS status
    bool                   clock_ulps_active;        ///< Data lane ULPS status
    mipi_dsi_lane_t        ulps_status;              ///< Ultra-low Power State active status
    mipi_dsi_cfg_t const * p_cfg;                    ///< Pointer to configuration structure used to open the interface
    void (* p_callback)(mipi_dsi_callback_args_t *); ///< Pointer to callback that is called when an adc_event_t occurs.
    void const               * p_context;            ///< Pointer to context to be passed into callback function
    mipi_dsi_callback_args_t * p_callback_memory;    ///< Pointer to non-secure memory that can be used to pass arguments to a callback in non-secure memory.
} mipi_dsi_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const mipi_dsi_api_t g_mipi_dsi;

/** @endcond */

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t R_MIPI_DSI_Open(mipi_dsi_ctrl_t * const p_api_ctrl, mipi_dsi_cfg_t const * const p_cfg);
fsp_err_t R_MIPI_DSI_Close(mipi_dsi_ctrl_t * const p_api_ctrl);
fsp_err_t R_MIPI_DSI_Start(mipi_dsi_ctrl_t * const p_api_ctrl);
fsp_err_t R_MIPI_DSI_UlpsEnter(mipi_dsi_ctrl_t * const p_api_ctrl, mipi_dsi_lane_t lane);
fsp_err_t R_MIPI_DSI_UlpsExit(mipi_dsi_ctrl_t * const p_api_ctrl, mipi_dsi_lane_t lane);
fsp_err_t R_MIPI_DSI_Stop(mipi_dsi_ctrl_t * const p_api_ctrl);
fsp_err_t R_MIPI_DSI_Command(mipi_dsi_ctrl_t * const p_api_ctrl, mipi_dsi_cmd_t * p_cmd);
fsp_err_t R_MIPI_DSI_StatusGet(mipi_dsi_ctrl_t * const p_api_ctrl, mipi_dsi_status_t * p_status);

/*******************************************************************************************************************//**
 * @} (end defgroup MIPI_DSI)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 // R_MIPI_DSI_H
