/* generated common source file - do not edit */
#include "common_data.h"
const uint8_t DRW_INT_IPL = (2);
            d2_device *   d2_handle;
/* MIPI PHY Macros */
        #define MIPI_PHY_CLKSTPT   (1183)
        #define MIPI_PHY_CLKBFHT   (10 + 1)
        #define MIPI_PHY_CLKKPT    (22 + 4)
        #define MIPI_PHY_GOLPBKT   (40)

        #define MIPI_PHY_TINIT     (71999)
        #define MIPI_PHY_TCLKPREP  (8)
        #define MIPI_PHY_THSPREP   (5)
        #define MIPI_PHY_TCLKTRAIL (7)
        #define MIPI_PHY_TCLKPOST  (19)
        #define MIPI_PHY_TCLKPRE   (1)
        #define MIPI_PHY_TCLKZERO  (27)
        #define MIPI_PHY_THSEXIT   (11)
        #define MIPI_PHY_THSTRAIL  (8)
        #define MIPI_PHY_THSZERO   (19)
        #define MIPI_PHY_TLPEXIT   (7)


        /* MIPI PHY Structures */
        const mipi_phy_timing_t g_mipi_phy0_timing =
        {
            .t_init                   = 0x3FFFF & (uint32_t)MIPI_PHY_TINIT,
            .t_clk_prep               = (uint8_t)MIPI_PHY_TCLKPREP,
            .t_hs_prep                = (uint8_t)MIPI_PHY_THSPREP,
            .dphytim4_b.t_clk_trail   = (uint32_t)MIPI_PHY_TCLKTRAIL,
            .dphytim4_b.t_clk_post    = (uint32_t)MIPI_PHY_TCLKPOST,
            .dphytim4_b.t_clk_pre     = (uint32_t)MIPI_PHY_TCLKPRE,
            .dphytim4_b.t_clk_zero    = (uint32_t)MIPI_PHY_TCLKZERO,
            .dphytim5_b.t_hs_exit     = (uint32_t)MIPI_PHY_THSEXIT,
            .dphytim5_b.t_hs_trail    = (uint32_t)MIPI_PHY_THSTRAIL,
            .dphytim5_b.t_hs_zero     = (uint32_t)MIPI_PHY_THSZERO,
            .t_lp_exit                = (uint32_t)MIPI_PHY_TLPEXIT,
        };

        mipi_phy_ctrl_t g_mipi_phy0_ctrl;
        const mipi_phy_cfg_t g_mipi_phy0_cfg =
        {
            .pll_settings = /* Actual MIPI PHY PLL frequency: 20000000 Hz / 1 * 50.00 = 1000000000 Hz (error 0.00%) */{.div      = 1 - 1,.mul_int  = 50 - 1,.mul_frac = 0},
            .lp_divisor   = 5 - 1,
            .p_timing     = &g_mipi_phy0_timing,
        };
        /* Instance structure to use this module. */
        const mipi_phy_instance_t g_mipi_phy0 =
        {
            .p_ctrl       = &g_mipi_phy0_ctrl,
            .p_cfg        = &g_mipi_phy0_cfg,
            .p_api        = &g_mipi_phy
        };
mipi_dsi_instance_ctrl_t g_mipi_dsi0_ctrl;

        const mipi_dsi_timing_t g_mipi_dsi0_timing =
        {
        .clock_stop_time       = MIPI_PHY_CLKSTPT,
        .clock_beforehand_time = MIPI_PHY_CLKBFHT,
        .clock_keep_time       = MIPI_PHY_CLKKPT,
        .go_lp_and_back        = MIPI_PHY_GOLPBKT,
        };


        const mipi_dsi_extended_cfg_t g_mipi_dsi0_extended_cfg =
        {
            .dsi_seq0.ipl           = (12),
            .dsi_seq0.irq           = VECTOR_NUMBER_MIPI_DSI_SEQ0,

            .dsi_seq1.ipl           = (12),
            .dsi_seq1.irq           = VECTOR_NUMBER_MIPI_DSI_SEQ1,

            .dsi_vin1.ipl           = (12),
            .dsi_vin1.irq           = VECTOR_NUMBER_MIPI_DSI_VIN1,

            .dsi_rcv.ipl            = (12),
            .dsi_rcv.irq            = VECTOR_NUMBER_MIPI_DSI_RCV,

            .dsi_ferr.ipl           = (12),
            .dsi_ferr.irq           = VECTOR_NUMBER_MIPI_DSI_FERR,

            .dsi_ppi.ipl            = (12),
            .dsi_ppi.irq            = VECTOR_NUMBER_MIPI_DSI_PPI,

            .dsi_rxie               = R_DSILINK_RXIER_BTAREND_Msk | R_DSILINK_RXIER_LRXHTO_Msk | R_DSILINK_RXIER_TATO_Msk | R_DSILINK_RXIER_RXRESP_Msk | R_DSILINK_RXIER_RXEOTP_Msk | R_DSILINK_RXIER_RXTE_Msk | R_DSILINK_RXIER_RXACK_Msk | R_DSILINK_RXIER_EXTEDET_Msk | R_DSILINK_RXIER_MLFERR_Msk | R_DSILINK_RXIER_ECCERRM_Msk | R_DSILINK_RXIER_UNEXERR_Msk | R_DSILINK_RXIER_WCERR_Msk | R_DSILINK_RXIER_CRCERR_Msk | R_DSILINK_RXIER_IBERR_Msk | R_DSILINK_RXIER_RXOVFERR_Msk | R_DSILINK_RXIER_PRTOERR_Msk | R_DSILINK_RXIER_NORESERR_Msk | R_DSILINK_RXIER_RSIZEERR_Msk | R_DSILINK_RXIER_ECCERRS_Msk | R_DSILINK_RXIER_RXAKE_Msk |  0x0,
            .dsi_ferrie             = R_DSILINK_FERRIER_HTXTO_Msk | R_DSILINK_FERRIER_LRXHTO_Msk | R_DSILINK_FERRIER_TATO_Msk | R_DSILINK_FERRIER_ESCENT_Msk | R_DSILINK_FERRIER_SYNCESC_Msk | R_DSILINK_FERRIER_CTRL_Msk | R_DSILINK_FERRIER_CLP0_Msk | R_DSILINK_FERRIER_CLP1_Msk |  0x0,
            .dsi_plie               =  0x0,
            .dsi_vmie               = R_DSILINK_VMIER_VBUFUDF_Msk | R_DSILINK_VMIER_VBUFOVF_Msk |  0x0,
            .dsi_sqch0ie            = R_DSILINK_SQCH0IER_AACTFIN_Msk | R_DSILINK_SQCH0IER_ADESFIN_Msk | R_DSILINK_SQCH0IER_TXIBERR_Msk | R_DSILINK_SQCH0IER_RXFERR_Msk | R_DSILINK_SQCH0IER_RXFAIL_Msk | R_DSILINK_SQCH0IER_RXPFAIL_Msk | R_DSILINK_SQCH0IER_RXCORERR_Msk | R_DSILINK_SQCH0IER_RXAKE_Msk |  0x0,
            .dsi_sqch1ie            = R_DSILINK_SQCH1IER_AACTFIN_Msk | R_DSILINK_SQCH1IER_ADESFIN_Msk | R_DSILINK_SQCH1IER_SIZEERR_Msk | R_DSILINK_SQCH1IER_TXIBERR_Msk | R_DSILINK_SQCH1IER_RXFERR_Msk | R_DSILINK_SQCH1IER_RXFAIL_Msk | R_DSILINK_SQCH1IER_RXPFAIL_Msk | R_DSILINK_SQCH1IER_RXCORERR_Msk | R_DSILINK_SQCH1IER_RXAKE_Msk |  0x0,
        };

        const mipi_dsi_cfg_t g_mipi_dsi0_cfg =
        {
            .p_mipi_phy_instance      = &g_mipi_phy0,

            .p_timing                 = &g_mipi_dsi0_timing,

            .sync_pulse               = (0),
            .data_type                = MIPI_DSI_VIDEO_DATA_16RGB_PIXEL_STREAM,
            .virtual_channel_id       = 0,

            .vertical_active_lines    = 360,
            .vertical_sync_lines      = 4,
            .vertical_back_porch      = (10 - 4),
            .vertical_front_porch     = (382 - 360 - 10 - 4),
            .vertical_sync_polarity   = (DISPLAY_SIGNAL_POLARITY_LOACTIVE != DISPLAY_SIGNAL_POLARITY_HIACTIVE),


            .horizontal_active_lines  = 480,
            .horizontal_sync_lines    = 4,
            .horizontal_back_porch    = (20 - 4),
            .horizontal_front_porch   = (514 - 480 - 20 - 4),
            .horizontal_sync_polarity = (DISPLAY_SIGNAL_POLARITY_LOACTIVE != DISPLAY_SIGNAL_POLARITY_HIACTIVE),

            .video_mode_delay         = 206 /* This value was calculated by FSP. An override is available but not recommended for most users */,

            .hsa_no_lp                = (( 0x0) & R_DSILINK_VMSET0R_HSANOLP_Msk),
            .hbp_no_lp                = (( 0x0) & R_DSILINK_VMSET0R_HBPNOLP_Msk),
            .hfp_no_lp                = (( 0x0) & R_DSILINK_VMSET0R_HFPNOLP_Msk),

            .num_lanes                = 2,
            .ulps_wakeup_period       = 97,
            .continuous_clock         = (1),


            .hs_tx_timeout            = 0,
            .lp_rx_timeout            = 0,
            .turnaround_timeout       = 0,
            .bta_timeout              = 0,
            .lprw_timeout             = (0 << R_DSILINK_PRESPTOLPSETR_LPRTO_Pos) | 0,
            .hsrw_timeout             = (0 << R_DSILINK_PRESPTOHSSETR_HSRTO_Pos) | 0,

            .max_return_packet_size   = 1,
            .ecc_enable               = (1),
            .crc_check_mask           = (mipi_dsi_vc_t)( 0x0),
            .scramble_enable          = (0),
            .tearing_detect           = (0),
            .eotp_enable              = (1),

            .p_extend                 = &g_mipi_dsi0_extended_cfg,
            .p_callback               = mipi_dsi0_callback,
            .p_context                = NULL,
        };

        /* Instance structure to use this module. */
        const mipi_dsi_instance_t g_mipi_dsi0 =
        {
            .p_ctrl        = &g_mipi_dsi0_ctrl,
            .p_cfg         = &g_mipi_dsi0_cfg,
            .p_api         = &g_mipi_dsi
        };
/** Display framebuffer */
        #if GLCDC_CFG_LAYER_1_ENABLE
        uint8_t fb_background[2][DISPLAY_BUFFER_STRIDE_BYTES_INPUT0 * DISPLAY_VSIZE_INPUT0] BSP_ALIGN_VARIABLE(64) BSP_PLACE_IN_SECTION(".sdram");
        #else
        /** Graphics Layer 1 is specified not to be used when starting */
        #endif
        #if GLCDC_CFG_LAYER_2_ENABLE
        uint8_t fb_foreground[2][DISPLAY_BUFFER_STRIDE_BYTES_INPUT1 * DISPLAY_VSIZE_INPUT1] BSP_ALIGN_VARIABLE(64) BSP_PLACE_IN_SECTION(".sdram");
        #else
        /** Graphics Layer 2 is specified not to be used when starting */
        #endif

        #if GLCDC_CFG_CLUT_ENABLE
        /** Display CLUT buffer to be used for updating CLUT */
        static uint32_t CLUT_buffer[256];

        /** Display CLUT configuration(only used if using CLUT format) */
        display_clut_cfg_t g_display0_clut_cfg_glcdc =
        {
            .p_base              = (uint32_t *)CLUT_buffer,
            .start               = 0,   /* User have to update this setting when using */
            .size                = 256  /* User have to update this setting when using */
        };
        #else
        /** CLUT is specified not to be used */
        #endif

        #if (false)
         #define GLCDC_CFG_CORRECTION_GAMMA_TABLE_CONST   const
         #define GLCDC_CFG_CORRECTION_GAMMA_TABLE_CAST    (uint16_t *)
         #define GLCDC_CFG_CORRECTION_GAMMA_CFG_CAST      (display_gamma_correction_t *)
        #else
         #define GLCDC_CFG_CORRECTION_GAMMA_TABLE_CONST
         #define GLCDC_CFG_CORRECTION_GAMMA_TABLE_CAST
         #define GLCDC_CFG_CORRECTION_GAMMA_CFG_CAST
        #endif

        #if ((GLCDC_CFG_CORRECTION_GAMMA_ENABLE_R | GLCDC_CFG_CORRECTION_GAMMA_ENABLE_G | GLCDC_CFG_CORRECTION_GAMMA_ENABLE_B) && GLCDC_CFG_COLOR_CORRECTION_ENABLE)
        /** Gamma correction tables */
        #if GLCDC_CFG_CORRECTION_GAMMA_ENABLE_R
        static GLCDC_CFG_CORRECTION_GAMMA_TABLE_CONST uint16_t glcdc_gamma_r_gain[DISPLAY_GAMMA_CURVE_ELEMENT_NUM] = {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024};
        static GLCDC_CFG_CORRECTION_GAMMA_TABLE_CONST uint16_t glcdc_gamma_r_threshold[DISPLAY_GAMMA_CURVE_ELEMENT_NUM] = {0, 64, 128, 192, 256, 320, 384, 448, 512, 576, 640, 704, 768, 832, 896, 960};
        #endif
        #if GLCDC_CFG_CORRECTION_GAMMA_ENABLE_G
        static GLCDC_CFG_CORRECTION_GAMMA_TABLE_CONST uint16_t glcdc_gamma_g_gain[DISPLAY_GAMMA_CURVE_ELEMENT_NUM] = {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024};
        static GLCDC_CFG_CORRECTION_GAMMA_TABLE_CONST uint16_t glcdc_gamma_g_threshold[DISPLAY_GAMMA_CURVE_ELEMENT_NUM] = {0, 64, 128, 192, 256, 320, 384, 448, 512, 576, 640, 704, 768, 832, 896, 960};
        #endif
        #if GLCDC_CFG_CORRECTION_GAMMA_ENABLE_B
        static GLCDC_CFG_CORRECTION_GAMMA_TABLE_CONST uint16_t glcdc_gamma_b_gain[DISPLAY_GAMMA_CURVE_ELEMENT_NUM] = {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024};
        static GLCDC_CFG_CORRECTION_GAMMA_TABLE_CONST uint16_t glcdc_gamma_b_threshold[DISPLAY_GAMMA_CURVE_ELEMENT_NUM] = {0, 64, 128, 192, 256, 320, 384, 448, 512, 576, 640, 704, 768, 832, 896, 960};
        #endif
        GLCDC_CFG_CORRECTION_GAMMA_TABLE_CONST display_gamma_correction_t g_display0_gamma_cfg =
        {
            .r =
            {
                .enable      = GLCDC_CFG_CORRECTION_GAMMA_ENABLE_R,
        #if GLCDC_CFG_CORRECTION_GAMMA_ENABLE_R
                .gain        = GLCDC_CFG_CORRECTION_GAMMA_TABLE_CAST glcdc_gamma_r_gain,
                .threshold   = GLCDC_CFG_CORRECTION_GAMMA_TABLE_CAST glcdc_gamma_r_threshold
        #else
                .gain        = NULL,
                .threshold   = NULL
        #endif
            },
            .g =
            {
                .enable      = GLCDC_CFG_CORRECTION_GAMMA_ENABLE_G,
        #if GLCDC_CFG_CORRECTION_GAMMA_ENABLE_G
                .gain        = GLCDC_CFG_CORRECTION_GAMMA_TABLE_CAST glcdc_gamma_g_gain,
                .threshold   = GLCDC_CFG_CORRECTION_GAMMA_TABLE_CAST glcdc_gamma_g_threshold
        #else
                .gain        = NULL,
                .threshold   = NULL
        #endif
            },
            .b =
            {
                .enable      = GLCDC_CFG_CORRECTION_GAMMA_ENABLE_B,
        #if GLCDC_CFG_CORRECTION_GAMMA_ENABLE_B
                .gain        = GLCDC_CFG_CORRECTION_GAMMA_TABLE_CAST glcdc_gamma_b_gain,
                .threshold   = GLCDC_CFG_CORRECTION_GAMMA_TABLE_CAST glcdc_gamma_b_threshold
        #else
                .gain        = NULL,
                .threshold   = NULL
        #endif
            }
        };
        #endif

        #define RA_NOT_DEFINED (1)
        #if (RA_NOT_DEFINED != g_mipi_dsi0)
          const mipi_dsi_instance_t g_mipi_dsi0;
        #endif
        /** Display device extended configuration */
        const glcdc_extended_cfg_t g_display0_extend_cfg =
        {
            .tcon_hsync            = GLCDC_TCON_PIN_1,
            .tcon_vsync            = GLCDC_TCON_PIN_0,
            .tcon_de               = GLCDC_TCON_PIN_2,
            .correction_proc_order = GLCDC_CORRECTION_PROC_ORDER_BRIGHTNESS_CONTRAST2GAMMA,
            .clksrc                = GLCDC_CLK_SRC_INTERNAL,
            .clock_div_ratio       = GLCDC_PANEL_CLK_DIVISOR_8,
            .dithering_mode        = GLCDC_DITHERING_MODE_TRUNCATE,
            .dithering_pattern_A   = GLCDC_DITHERING_PATTERN_11,
            .dithering_pattern_B   = GLCDC_DITHERING_PATTERN_11,
            .dithering_pattern_C   = GLCDC_DITHERING_PATTERN_11,
            .dithering_pattern_D   = GLCDC_DITHERING_PATTERN_11,
        #if (RA_NOT_DEFINED != g_mipi_dsi0)
            .phy_layer             = (void*)&g_mipi_dsi0
        #else
            .phy_layer             = NULL
        #endif
        };
        #undef RA_NOT_DEFINED

        /** Display control block instance */
        glcdc_instance_ctrl_t g_display0_ctrl;

        /** Display interface configuration */
        const display_cfg_t g_display0_cfg =
        {
            /** Input1(Graphics1 layer) configuration */
            .input[0] =
            {
                #if GLCDC_CFG_LAYER_1_ENABLE
                .p_base              = (uint32_t *)&fb_background[0],
                #else
                .p_base              = NULL,
                #endif
                .hsize               = DISPLAY_HSIZE_INPUT0,
                .vsize               = DISPLAY_VSIZE_INPUT0,
                .hstride             = DISPLAY_BUFFER_STRIDE_PIXELS_INPUT0,
                .format              = DISPLAY_IN_FORMAT_16BITS_RGB565,
                .line_descending_enable = false,
                .lines_repeat_enable = false,
                .lines_repeat_times  = 0
            },

            /** Input2(Graphics2 layer) configuration */
            .input[1] =
            {
                #if GLCDC_CFG_LAYER_2_ENABLE
                .p_base              = (uint32_t *)&fb_foreground[0],
                #else
                .p_base              = NULL,
                #endif
                .hsize               = DISPLAY_HSIZE_INPUT1,
                .vsize               = DISPLAY_VSIZE_INPUT1,
                .hstride             = DISPLAY_BUFFER_STRIDE_PIXELS_INPUT1,
                .format              = DISPLAY_IN_FORMAT_16BITS_RGB565,
                .line_descending_enable = false,
                .lines_repeat_enable = false,
                .lines_repeat_times  = 0
             },

            /** Input1(Graphics1 layer) layer configuration */
            .layer[0] =
            {
                .coordinate = {
                        .x           = 0,
                        .y           = 0
                },
                .bg_color =
                {
                    .byte = {
                        .a           = 255,
                        .r           = 255,
                        .g           = 255,
                        .b           = 255
                    }
                },
                .fade_control        = DISPLAY_FADE_CONTROL_NONE,
                .fade_speed          = 0
            },

            /** Input2(Graphics2 layer) layer configuration */
            .layer[1] =
            {
                .coordinate = {
                        .x           = 0,
                        .y           = 0
                },
                .bg_color =
                {
                    .byte = {
                        .a           = 255,
                        .r           = 255,
                        .g           = 255,
                        .b           = 255
                    }
                },
                .fade_control        = DISPLAY_FADE_CONTROL_NONE,
                .fade_speed          = 0
            },

            /** Output configuration */
            .output =
            {
                .htiming =
                {
                    .total_cyc       = 514,
                    .display_cyc     = 480,
                    .back_porch      = 20,
                    .sync_width       = 4,
                    .sync_polarity   = DISPLAY_SIGNAL_POLARITY_LOACTIVE
                },
                .vtiming =
                {
                    .total_cyc       = 382,
                    .display_cyc     = 360,
                    .back_porch      = 10,
                    .sync_width       = 4,
                    .sync_polarity   = DISPLAY_SIGNAL_POLARITY_LOACTIVE
                },
                .format              = DISPLAY_OUT_FORMAT_16BITS_RGB565,
                .endian              = DISPLAY_ENDIAN_LITTLE,
                .color_order         = DISPLAY_COLOR_ORDER_RGB,
                .data_enable_polarity = DISPLAY_SIGNAL_POLARITY_HIACTIVE,
                .sync_edge           = DISPLAY_SIGNAL_SYNC_EDGE_RISING,
                .bg_color =
                {
                    .byte = {
                        .a           = 255,
                        .r           = 0,
                        .g           = 0,
                        .b           = 0
                    }
                },
#if (GLCDC_CFG_COLOR_CORRECTION_ENABLE)
                .brightness =
                {
                    .enable          = false,
                    .r               = 512,
                    .g               = 512,
                    .b               = 512
                },
                .contrast =
                {
                    .enable          = false,
                    .r               = 128,
                    .g               = 128,
                    .b               = 128
                },
#if (GLCDC_CFG_CORRECTION_GAMMA_ENABLE_R | GLCDC_CFG_CORRECTION_GAMMA_ENABLE_G | GLCDC_CFG_CORRECTION_GAMMA_ENABLE_B)
 #if false
                .p_gamma_correction  = GLCDC_CFG_CORRECTION_GAMMA_CFG_CAST (&g_display0_gamma_cfg),
 #else
                .p_gamma_correction  = &g_display0_gamma_cfg,
 #endif
#else
                .p_gamma_correction  = NULL,
#endif
#endif
                .dithering_on        = false
            },

            /** Display device callback function pointer */
            .p_callback              = DisplayVsyncCallback,
            .p_context               = NULL,

            /** Display device extended configuration */
            .p_extend                = (void *)(&g_display0_extend_cfg),

            .line_detect_ipl        = (12),
            .underflow_1_ipl        = (BSP_IRQ_DISABLED),
            .underflow_2_ipl        = (BSP_IRQ_DISABLED),

#if defined(VECTOR_NUMBER_GLCDC_LINE_DETECT)
            .line_detect_irq        = VECTOR_NUMBER_GLCDC_LINE_DETECT,
#else
            .line_detect_irq        = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GLCDC_UNDERFLOW_1)
            .underflow_1_irq        = VECTOR_NUMBER_GLCDC_UNDERFLOW_1,
#else
            .underflow_1_irq        = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GLCDC_UNDERFLOW_2)
            .underflow_2_irq        = VECTOR_NUMBER_GLCDC_UNDERFLOW_2,
#else
            .underflow_2_irq        = FSP_INVALID_VECTOR,
#endif
        };

#if GLCDC_CFG_LAYER_1_ENABLE
        /** Display on GLCDC run-time configuration(for the graphics1 layer) */
        display_runtime_cfg_t g_display0_runtime_cfg_bg =
        {
            .input =
            {
                #if (true)
                .p_base              = (uint32_t *)&fb_background[0],
                #else
                .p_base              = NULL,
                #endif
                .hsize               = DISPLAY_HSIZE_INPUT0,
                .vsize               = DISPLAY_VSIZE_INPUT0,
                .hstride             = DISPLAY_BUFFER_STRIDE_PIXELS_INPUT0,
                .format              = DISPLAY_IN_FORMAT_16BITS_RGB565,
                .line_descending_enable = false,
                .lines_repeat_enable = false,
                .lines_repeat_times  = 0
            },
            .layer =
            {
                .coordinate = {
                        .x           = 0,
                        .y           = 0
                },
                .bg_color            =
                {
                    .byte            =
                    {
                        .a           = 255,
                        .r           = 255,
                        .g           = 255,
                        .b           = 255
                    }
                },
                .fade_control        = DISPLAY_FADE_CONTROL_NONE,
                .fade_speed          = 0
            }
        };
#endif
#if GLCDC_CFG_LAYER_2_ENABLE
        /** Display on GLCDC run-time configuration(for the graphics2 layer) */
        display_runtime_cfg_t g_display0_runtime_cfg_fg =
        {
            .input =
            {
                #if (false)
                .p_base              = (uint32_t *)&fb_foreground[0],
                #else
                .p_base              = NULL,
                #endif
                .hsize               = DISPLAY_HSIZE_INPUT1,
                .vsize               = DISPLAY_VSIZE_INPUT1,
                .hstride             = DISPLAY_BUFFER_STRIDE_PIXELS_INPUT1,
                .format              = DISPLAY_IN_FORMAT_16BITS_RGB565,
                .line_descending_enable = false,
                .lines_repeat_enable = false,
                .lines_repeat_times  = 0
             },
            .layer =
            {
                .coordinate = {
                        .x           = 0,
                        .y           = 0
                },
                .bg_color            =
                {
                    .byte            =
                    {
                        .a           = 255,
                        .r           = 255,
                        .g           = 255,
                        .b           = 255
                    }
                },
                .fade_control        = DISPLAY_FADE_CONTROL_NONE,
                .fade_speed          = 0
            }
        };
#endif

/* Instance structure to use this module. */
const display_instance_t g_display0 =
{
    .p_ctrl        = &g_display0_ctrl,
    .p_cfg         = (display_cfg_t *)&g_display0_cfg,
    .p_api         = (display_api_t *)&g_display_on_glcdc
};
ioport_instance_ctrl_t g_ioport_ctrl;
const ioport_instance_t g_ioport =
        {
            .p_api = &g_ioport_on_ioport,
            .p_ctrl = &g_ioport_ctrl,
            .p_cfg = &g_bsp_pin_cfg,
        };
void g_common_init(void) {
}
