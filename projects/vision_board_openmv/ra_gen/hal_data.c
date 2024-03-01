/* generated HAL source file - do not edit */
#include "hal_data.h"

ceu_instance_ctrl_t g_ceu_hqvga_yuv_ctrl;
            const ceu_extended_cfg_t g_ceu_hqvga_yuv_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_ENABLE,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 1,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 38400,
                .interrupts_enabled   = R_CEU_CEIER_FWFIE_Msk | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_hqvga_yuv_cfg =
            {
                .x_capture_pixels      = 0,
                .y_capture_pixels      = 0,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_hqvga_yuv_extended_cfg,
            };

            const capture_instance_t g_ceu_hqvga_yuv =
            {
                .p_ctrl = &g_ceu_hqvga_yuv_ctrl,
                .p_cfg =  &g_ceu_hqvga_yuv_cfg,
                .p_api =  &g_ceu_on_capture,
            };
ceu_instance_ctrl_t g_ceu_qqqvga_yuv_ctrl;
            const ceu_extended_cfg_t g_ceu_qqqvga_yuv_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_ENABLE,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 1,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 4800,
                .interrupts_enabled   = R_CEU_CEIER_FWFIE_Msk | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_qqqvga_yuv_cfg =
            {
                .x_capture_pixels      = 0,
                .y_capture_pixels      = 0,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_qqqvga_yuv_extended_cfg,
            };

            const capture_instance_t g_ceu_qqqvga_yuv =
            {
                .p_ctrl = &g_ceu_qqqvga_yuv_ctrl,
                .p_cfg =  &g_ceu_qqqvga_yuv_cfg,
                .p_api =  &g_ceu_on_capture,
            };
ceu_instance_ctrl_t g_ceu_qqvga_yuv_ctrl;
            const ceu_extended_cfg_t g_ceu_qqvga_yuv_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_ENABLE,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 1,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 19200,
                .interrupts_enabled   = R_CEU_CEIER_FWFIE_Msk | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_qqvga_yuv_cfg =
            {
                .x_capture_pixels      = 0,
                .y_capture_pixels      = 0,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_qqvga_yuv_extended_cfg,
            };

            const capture_instance_t g_ceu_qqvga_yuv =
            {
                .p_ctrl = &g_ceu_qqvga_yuv_ctrl,
                .p_cfg =  &g_ceu_qqvga_yuv_cfg,
                .p_api =  &g_ceu_on_capture,
            };
ceu_instance_ctrl_t g_ceu_qvga_yuv_ctrl;
            const ceu_extended_cfg_t g_ceu_qvga_yuv_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_ENABLE,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 1,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 76800,
                .interrupts_enabled   = R_CEU_CEIER_FWFIE_Msk | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_qvga_yuv_cfg =
            {
                .x_capture_pixels      = 0,
                .y_capture_pixels      = 0,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_qvga_yuv_extended_cfg,
            };

            const capture_instance_t g_ceu_qvga_yuv =
            {
                .p_ctrl = &g_ceu_qvga_yuv_ctrl,
                .p_cfg =  &g_ceu_qvga_yuv_cfg,
                .p_api =  &g_ceu_on_capture,
            };
ceu_instance_ctrl_t g_ceu_vga_yuv_ctrl;
            const ceu_extended_cfg_t g_ceu_vga_yuv_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_ENABLE,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 1,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 307200,
                .interrupts_enabled   = R_CEU_CEIER_FWFIE_Msk | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_vga_yuv_cfg =
            {
                .x_capture_pixels      = 0,
                .y_capture_pixels      = 0,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_vga_yuv_extended_cfg,
            };

            const capture_instance_t g_ceu_vga_yuv =
            {
                .p_ctrl = &g_ceu_vga_yuv_ctrl,
                .p_cfg =  &g_ceu_vga_yuv_cfg,
                .p_api =  &g_ceu_on_capture,
            };
dtc_instance_ctrl_t g_transfer2_ctrl;

#if (1 == 1)
transfer_info_t g_transfer2_info =
{
    .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
    .transfer_settings_word_b.repeat_area    = TRANSFER_REPEAT_AREA_DESTINATION,
    .transfer_settings_word_b.irq            = TRANSFER_IRQ_END,
    .transfer_settings_word_b.chain_mode     = TRANSFER_CHAIN_MODE_DISABLED,
    .transfer_settings_word_b.src_addr_mode  = TRANSFER_ADDR_MODE_FIXED,
    .transfer_settings_word_b.size           = TRANSFER_SIZE_2_BYTE,
    .transfer_settings_word_b.mode           = TRANSFER_MODE_NORMAL,
    .p_dest                                  = (void *) NULL,
    .p_src                                   = (void const *) NULL,
    .num_blocks                              = 0,
    .length                                  = 0,
};

#elif (1 > 1)
/* User is responsible to initialize the array. */
transfer_info_t g_transfer2_info[1];
#else
/* User must call api::reconfigure before enable DTC transfer. */
#endif

const dtc_extended_cfg_t g_transfer2_cfg_extend =
{
    .activation_source   = VECTOR_NUMBER_SPI1_RXI,
};

const transfer_cfg_t g_transfer2_cfg =
{
#if (1 == 1)
    .p_info              = &g_transfer2_info,
#elif (1 > 1)
    .p_info              = g_transfer2_info,
#else
    .p_info = NULL,
#endif
    .p_extend            = &g_transfer2_cfg_extend,
};

/* Instance structure to use this module. */
const transfer_instance_t g_transfer2 =
{
    .p_ctrl        = &g_transfer2_ctrl,
    .p_cfg         = &g_transfer2_cfg,
    .p_api         = &g_transfer_on_dtc
};
dtc_instance_ctrl_t g_transfer1_ctrl;

#if (1 == 1)
transfer_info_t g_transfer1_info =
{
    .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
    .transfer_settings_word_b.repeat_area    = TRANSFER_REPEAT_AREA_SOURCE,
    .transfer_settings_word_b.irq            = TRANSFER_IRQ_END,
    .transfer_settings_word_b.chain_mode     = TRANSFER_CHAIN_MODE_DISABLED,
    .transfer_settings_word_b.src_addr_mode  = TRANSFER_ADDR_MODE_INCREMENTED,
    .transfer_settings_word_b.size           = TRANSFER_SIZE_2_BYTE,
    .transfer_settings_word_b.mode           = TRANSFER_MODE_NORMAL,
    .p_dest                                  = (void *) NULL,
    .p_src                                   = (void const *) NULL,
    .num_blocks                              = 0,
    .length                                  = 0,
};

#elif (1 > 1)
/* User is responsible to initialize the array. */
transfer_info_t g_transfer1_info[1];
#else
/* User must call api::reconfigure before enable DTC transfer. */
#endif

const dtc_extended_cfg_t g_transfer1_cfg_extend =
{
    .activation_source   = VECTOR_NUMBER_SPI1_TXI,
};

const transfer_cfg_t g_transfer1_cfg =
{
#if (1 == 1)
    .p_info              = &g_transfer1_info,
#elif (1 > 1)
    .p_info              = g_transfer1_info,
#else
    .p_info = NULL,
#endif
    .p_extend            = &g_transfer1_cfg_extend,
};

/* Instance structure to use this module. */
const transfer_instance_t g_transfer1 =
{
    .p_ctrl        = &g_transfer1_ctrl,
    .p_cfg         = &g_transfer1_cfg,
    .p_api         = &g_transfer_on_dtc
};
spi_b_instance_ctrl_t g_spi1_ctrl;

/** SPI extended configuration for SPI HAL driver */
const spi_b_extended_cfg_t g_spi1_ext_cfg =
{
    .spi_clksyn         = SPI_B_SSL_MODE_CLK_SYN,
    .spi_comm           = SPI_B_COMMUNICATION_FULL_DUPLEX,
    .ssl_polarity        = SPI_B_SSLP_LOW,
    .ssl_select          = SPI_B_SSL_SELECT_SSL0,
    .mosi_idle           = SPI_B_MOSI_IDLE_VALUE_FIXING_DISABLE,
    .parity              = SPI_B_PARITY_MODE_DISABLE,
    .byte_swap           = SPI_B_BYTE_SWAP_DISABLE,
    .clock_source        = SPI_B_CLOCK_SOURCE_PCLK,
    .spck_div            = {
        /* Actual calculated bitrate: 30000000. */ .spbr = 1, .brdv = 0
    },
    .spck_delay          = SPI_B_DELAY_COUNT_1,
    .ssl_negation_delay  = SPI_B_DELAY_COUNT_1,
    .next_access_delay   = SPI_B_DELAY_COUNT_1,

 };

/** SPI configuration for SPI HAL driver */
const spi_cfg_t g_spi1_cfg =
{
    .channel             = 1,

#if defined(VECTOR_NUMBER_SPI1_RXI)
    .rxi_irq             = VECTOR_NUMBER_SPI1_RXI,
#else
    .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI1_TXI)
    .txi_irq             = VECTOR_NUMBER_SPI1_TXI,
#else
    .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI1_TEI)
    .tei_irq             = VECTOR_NUMBER_SPI1_TEI,
#else
    .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI1_ERI)
    .eri_irq             = VECTOR_NUMBER_SPI1_ERI,
#else
    .eri_irq             = FSP_INVALID_VECTOR,
#endif

    .rxi_ipl             = (12),
    .txi_ipl             = (12),
    .tei_ipl             = (12),
    .eri_ipl             = (12),

    .operating_mode      = SPI_MODE_MASTER,

    .clk_phase           = SPI_CLK_PHASE_EDGE_ODD,
    .clk_polarity        = SPI_CLK_POLARITY_LOW,

    .mode_fault          = SPI_MODE_FAULT_ERROR_DISABLE,
    .bit_order           = SPI_BIT_ORDER_MSB_FIRST,
    .p_transfer_tx       = g_spi1_P_TRANSFER_TX,
    .p_transfer_rx       = g_spi1_P_TRANSFER_RX,
    .p_callback          = spi1_callback,

    .p_context           = NULL,
    .p_extend            = (void *)&g_spi1_ext_cfg,
};

/* Instance structure to use this module. */
const spi_instance_t g_spi1 =
{
    .p_ctrl        = &g_spi1_ctrl,
    .p_cfg         = &g_spi1_cfg,
    .p_api         = &g_spi_on_spi_b
};
gpt_instance_ctrl_t g_timer7_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer7_pwm_extend =
{
    .trough_ipl          = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT7_COUNTER_UNDERFLOW)
    .trough_irq          = VECTOR_NUMBER_GPT7_COUNTER_UNDERFLOW,
#else
    .trough_irq          = FSP_INVALID_VECTOR,
#endif
    .poeg_link           = GPT_POEG_LINK_POEG0,
    .output_disable      = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger         = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up  = 0,
    .dead_time_count_down = 0,
    .adc_a_compare_match = 0,
    .adc_b_compare_match = 0,
    .interrupt_skip_source = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc    = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer7_extend =
{
    .gtioca = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = true,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .start_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .stop_source         = (gpt_source_t) ( GPT_SOURCE_NONE),
    .clear_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_up_source     = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_down_source   = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_b_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_ipl       = (BSP_IRQ_DISABLED),
    .capture_b_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT7_CAPTURE_COMPARE_A)
    .capture_a_irq       = VECTOR_NUMBER_GPT7_CAPTURE_COMPARE_A,
#else
    .capture_a_irq       = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT7_CAPTURE_COMPARE_B)
    .capture_b_irq       = VECTOR_NUMBER_GPT7_CAPTURE_COMPARE_B,
#else
    .capture_b_irq       = FSP_INVALID_VECTOR,
#endif
    .capture_filter_gtioca       = GPT_CAPTURE_FILTER_NONE,
    .capture_filter_gtiocb       = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg                   = &g_timer7_pwm_extend,
#else
    .p_pwm_cfg                   = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) true,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
    .gtior_setting.gtior = 0U,
#endif
};

const timer_cfg_t g_timer7_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 4.166666666666667e-8 seconds. Actual duty: 40%. */ .period_counts = (uint32_t) 0x5, .duty_cycle_counts = 0x2, .source_div = (timer_source_div_t)0,
    .channel             = 7,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = &NULL,
#endif
    .p_extend            = &g_timer7_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT7_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT7_COUNTER_OVERFLOW,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer7 =
{
    .p_ctrl        = &g_timer7_ctrl,
    .p_cfg         = &g_timer7_cfg,
    .p_api         = &g_timer_on_gpt
};
#include "r_sci_b_i2c_cfg.h"
sci_b_i2c_instance_ctrl_t g_sci3_ctrl;
const sci_b_i2c_extended_cfg_t g_sci3_cfg_extend =
{
    /* Actual calculated bitrate: 399693. Actual SDA delay: 258.33 ns. */ .clock_settings.clk_divisor_value = 0, .clock_settings.brr_value = 6, .clock_settings.mddr_value = 191, .clock_settings.bitrate_modulation = true, .clock_settings.cycles_value = 31,
    .clock_settings.snfr_value     = (1),
    .clock_settings.clock_source   = SCI_B_I2C_CLOCK_SOURCE_PCLK
};

const i2c_master_cfg_t g_sci3_cfg =
{
    .channel             = 3,
    .rate                = I2C_MASTER_RATE_FAST,
    .slave               = 0x00,
    .addr_mode           = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
    .p_transfer_tx       = NULL,
#else
    .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
    .p_transfer_rx       = NULL,
#else
    .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
    .p_callback          = sci_i2c_irq_callback,
    .p_context           = NULL,
#if defined(VECTOR_NUMBER_SCI3_RXI) && SCI_B_I2C_CFG_DTC_ENABLE
    .rxi_irq             = VECTOR_NUMBER_SCI3_RXI,
#else
    .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI3_TXI)
    .txi_irq             = VECTOR_NUMBER_SCI3_TXI,
#else
    .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI3_TEI)
    .tei_irq             = VECTOR_NUMBER_SCI3_TEI,
#else
    .tei_irq             = FSP_INVALID_VECTOR,
#endif
    .ipl                 = (12),    /* (BSP_IRQ_DISABLED) is unused */
    .p_extend            = &g_sci3_cfg_extend,
};
/* Instance structure to use this module. */
const i2c_master_instance_t g_sci3 =
{
    .p_ctrl        = &g_sci3_ctrl,
    .p_cfg         = &g_sci3_cfg,
    .p_api         = &g_i2c_master_on_sci_b
};
gpt_instance_ctrl_t g_timer6_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer6_pwm_extend =
{
    .trough_ipl          = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT6_COUNTER_UNDERFLOW)
    .trough_irq          = VECTOR_NUMBER_GPT6_COUNTER_UNDERFLOW,
#else
    .trough_irq          = FSP_INVALID_VECTOR,
#endif
    .poeg_link           = GPT_POEG_LINK_POEG0,
    .output_disable      = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger         = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up  = 0,
    .dead_time_count_down = 0,
    .adc_a_compare_match = 0,
    .adc_b_compare_match = 0,
    .interrupt_skip_source = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc    = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer6_extend =
{
    .gtioca = { .output_enabled = true,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .start_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .stop_source         = (gpt_source_t) ( GPT_SOURCE_NONE),
    .clear_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_up_source     = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_down_source   = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_b_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_ipl       = (BSP_IRQ_DISABLED),
    .capture_b_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT6_CAPTURE_COMPARE_A)
    .capture_a_irq       = VECTOR_NUMBER_GPT6_CAPTURE_COMPARE_A,
#else
    .capture_a_irq       = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT6_CAPTURE_COMPARE_B)
    .capture_b_irq       = VECTOR_NUMBER_GPT6_CAPTURE_COMPARE_B,
#else
    .capture_b_irq       = FSP_INVALID_VECTOR,
#endif
    .capture_filter_gtioca       = GPT_CAPTURE_FILTER_NONE,
    .capture_filter_gtiocb       = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg                   = &g_timer6_pwm_extend,
#else
    .p_pwm_cfg                   = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) true,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
    .gtior_setting.gtior = 0U,
#endif
};

const timer_cfg_t g_timer6_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.00008333333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x2710, .duty_cycle_counts = 0x1388, .source_div = (timer_source_div_t)0,
    .channel             = 6,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = &NULL,
#endif
    .p_extend            = &g_timer6_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT6_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT6_COUNTER_OVERFLOW,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer6 =
{
    .p_ctrl        = &g_timer6_ctrl,
    .p_cfg         = &g_timer6_cfg,
    .p_api         = &g_timer_on_gpt
};
dmac_instance_ctrl_t g_transfer0_ctrl;
transfer_info_t g_transfer0_info =
{
    .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
    .transfer_settings_word_b.repeat_area    = TRANSFER_REPEAT_AREA_SOURCE,
    .transfer_settings_word_b.irq            = TRANSFER_IRQ_END,
    .transfer_settings_word_b.chain_mode     = TRANSFER_CHAIN_MODE_DISABLED,
    .transfer_settings_word_b.src_addr_mode  = TRANSFER_ADDR_MODE_INCREMENTED,
    .transfer_settings_word_b.size           = TRANSFER_SIZE_4_BYTE,
    .transfer_settings_word_b.mode           = TRANSFER_MODE_NORMAL,
    .p_dest                                  = (void *) NULL,
    .p_src                                   = (void const *) NULL,
    .num_blocks                              = 0,
    .length                                  = 128,
};
const dmac_extended_cfg_t g_transfer0_extend =
{
    .offset              = 1,
    .src_buffer_size     = 1,
#if defined(VECTOR_NUMBER_DMAC0_INT)
    .irq                 = VECTOR_NUMBER_DMAC0_INT,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
    .ipl                 = (12),
    .channel             = 0,
    .p_callback          = g_sdmmc1_dmac_callback,
    .p_context           = &g_sdmmc1_ctrl,
    .activation_source   = ELC_EVENT_SDHIMMC1_DMA_REQ,
};
const transfer_cfg_t g_transfer0_cfg =
{
    .p_info              = &g_transfer0_info,
    .p_extend            = &g_transfer0_extend,
};
/* Instance structure to use this module. */
const transfer_instance_t g_transfer0 =
{
    .p_ctrl        = &g_transfer0_ctrl,
    .p_cfg         = &g_transfer0_cfg,
    .p_api         = &g_transfer_on_dmac
};
#define RA_NOT_DEFINED (UINT32_MAX)
#if (RA_NOT_DEFINED) != (1)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
#include "r_dmac.h"
extern void r_sdhi_transfer_callback(sdhi_instance_ctrl_t * p_ctrl);

void g_sdmmc1_dmac_callback (dmac_callback_args_t * p_args)
{
    r_sdhi_transfer_callback((sdhi_instance_ctrl_t *) p_args->p_context);
}
#endif
#undef RA_NOT_DEFINED

sdhi_instance_ctrl_t g_sdmmc1_ctrl;
sdmmc_cfg_t g_sdmmc1_cfg =
{
    .bus_width              = SDMMC_BUS_WIDTH_4_BITS,
    .channel                = 1,
    .p_callback             = sdhi_callback,
    .p_context              = NULL,
    .block_size             = 512,
    .card_detect            = SDMMC_CARD_DETECT_CD,
    .write_protect          = SDMMC_WRITE_PROTECT_WP,

    .p_extend               = NULL,
    .p_lower_lvl_transfer   = &g_transfer0,

    .access_ipl             = (12),
    .sdio_ipl               = BSP_IRQ_DISABLED,
    .card_ipl               = (12),
    .dma_req_ipl            = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_SDHIMMC1_ACCS)
    .access_irq             = VECTOR_NUMBER_SDHIMMC1_ACCS,
#else
    .access_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SDHIMMC1_CARD)
    .card_irq               = VECTOR_NUMBER_SDHIMMC1_CARD,
#else
    .card_irq               = FSP_INVALID_VECTOR,
#endif
    .sdio_irq               = FSP_INVALID_VECTOR,
#if defined(VECTOR_NUMBER_SDHIMMC1_DMA_REQ)
    .dma_req_irq            = VECTOR_NUMBER_SDHIMMC1_DMA_REQ,
#else
    .dma_req_irq            = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const sdmmc_instance_t g_sdmmc1 =
{
    .p_ctrl        = &g_sdmmc1_ctrl,
    .p_cfg         = &g_sdmmc1_cfg,
    .p_api         = &g_sdmmc_on_sdhi
};
sci_b_uart_instance_ctrl_t     g_uart2_ctrl;

            sci_b_baud_setting_t               g_uart2_baud_setting =
            {
                /* Baud rate calculated with 0.160% error. */ .baudrate_bits_b.abcse = 0, .baudrate_bits_b.abcs = 0, .baudrate_bits_b.bgdm = 1, .baudrate_bits_b.cks = 1, .baudrate_bits_b.brr = 194, .baudrate_bits_b.mddr = (uint8_t) 256, .baudrate_bits_b.brme = false
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_b_uart_extended_cfg_t g_uart2_cfg_extend =
            {
                .clock                = SCI_B_UART_CLOCK_INT,
                .rx_edge_start          = SCI_B_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_B_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_B_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart2_baud_setting,
                .flow_control           = SCI_B_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting          = {
                    .enable = SCI_B_UART_RS485_DISABLE,
                    .polarity = SCI_B_UART_RS485_DE_POLARITY_HIGH,
                    .assertion_time = 1,
                    .negation_time = 1,
                }
            };

            /** UART interface configuration */
            const uart_cfg_t g_uart2_cfg =
            {
                .channel             = 2,
                .data_bits           = UART_DATA_BITS_8,
                .parity              = UART_PARITY_OFF,
                .stop_bits           = UART_STOP_BITS_1,
                .p_callback          = user_uart2_callback,
                .p_context           = NULL,
                .p_extend            = &g_uart2_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_tx       = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_rx       = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
                .rxi_ipl             = (12),
                .txi_ipl             = (12),
                .tei_ipl             = (12),
                .eri_ipl             = (12),
#if defined(VECTOR_NUMBER_SCI2_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI2_RXI,
#else
                .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI2_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI2_TXI,
#else
                .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI2_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI2_TEI,
#else
                .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI2_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI2_ERI,
#else
                .eri_irq             = FSP_INVALID_VECTOR,
#endif
            };

/* Instance structure to use this module. */
const uart_instance_t g_uart2 =
{
    .p_ctrl        = &g_uart2_ctrl,
    .p_cfg         = &g_uart2_cfg,
    .p_api         = &g_uart_on_sci_b
};
ceu_instance_ctrl_t g_ceu_64X32_ctrl;
            const ceu_extended_cfg_t g_ceu_64X32_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_SYNCHRONOUS,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 0,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 64 * 32 * 2,
                .interrupts_enabled   = 0 | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_64X32_cfg =
            {
                .x_capture_pixels      = 64,
                .y_capture_pixels      = 32,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_64X32_extended_cfg,
            };

            const capture_instance_t g_ceu_64X32 =
            {
                .p_ctrl = &g_ceu_64X32_ctrl,
                .p_cfg =  &g_ceu_64X32_cfg,
                .p_api =  &g_ceu_on_capture,
            };
ceu_instance_ctrl_t g_ceu_320X320_ctrl;
            const ceu_extended_cfg_t g_ceu_320X320_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_SYNCHRONOUS,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 0,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 320 * 320 * 2,
                .interrupts_enabled   = 0 | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_320X320_cfg =
            {
                .x_capture_pixels      = 320,
                .y_capture_pixels      = 320,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_320X320_extended_cfg,
            };

            const capture_instance_t g_ceu_320X320 =
            {
                .p_ctrl = &g_ceu_320X320_ctrl,
                .p_cfg =  &g_ceu_320X320_cfg,
                .p_api =  &g_ceu_on_capture,
            };
ceu_instance_ctrl_t g_ceu_160X160_ctrl;
            const ceu_extended_cfg_t g_ceu_160X160_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_SYNCHRONOUS,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 0,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 160 * 160 * 2,
                .interrupts_enabled   = 0 | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_160X160_cfg =
            {
                .x_capture_pixels      = 160,
                .y_capture_pixels      = 160,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_160X160_extended_cfg,
            };

            const capture_instance_t g_ceu_160X160 =
            {
                .p_ctrl = &g_ceu_160X160_ctrl,
                .p_cfg =  &g_ceu_160X160_cfg,
                .p_api =  &g_ceu_on_capture,
            };
ceu_instance_ctrl_t g_ceu_128X128_ctrl;
            const ceu_extended_cfg_t g_ceu_128X128_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_SYNCHRONOUS,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 0,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 128 * 128 * 2,
                .interrupts_enabled   = 0 | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_128X128_cfg =
            {
                .x_capture_pixels      = 128,
                .y_capture_pixels      = 128,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_128X128_extended_cfg,
            };

            const capture_instance_t g_ceu_128X128 =
            {
                .p_ctrl = &g_ceu_128X128_ctrl,
                .p_cfg =  &g_ceu_128X128_cfg,
                .p_api =  &g_ceu_on_capture,
            };
ceu_instance_ctrl_t g_ceu_128X64_ctrl;
            const ceu_extended_cfg_t g_ceu_128X64_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_SYNCHRONOUS,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 0,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 128 * 64 * 2,
                .interrupts_enabled   = 0 | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_128X64_cfg =
            {
                .x_capture_pixels      = 128,
                .y_capture_pixels      = 64,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_128X64_extended_cfg,
            };

            const capture_instance_t g_ceu_128X64 =
            {
                .p_ctrl = &g_ceu_128X64_ctrl,
                .p_cfg =  &g_ceu_128X64_cfg,
                .p_api =  &g_ceu_on_capture,
            };
ceu_instance_ctrl_t g_ceu_64X64_ctrl;
            const ceu_extended_cfg_t g_ceu_64X64_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_SYNCHRONOUS,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 0,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 64 * 64 * 2,
                .interrupts_enabled   = 0 | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_64X64_cfg =
            {
                .x_capture_pixels      = 64,
                .y_capture_pixels      = 64,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_64X64_extended_cfg,
            };

            const capture_instance_t g_ceu_64X64 =
            {
                .p_ctrl = &g_ceu_64X64_ctrl,
                .p_cfg =  &g_ceu_64X64_cfg,
                .p_api =  &g_ceu_on_capture,
            };
flash_hp_instance_ctrl_t g_flash_ctrl;
const flash_cfg_t g_flash_cfg =
{
    .data_flash_bgo      = false,
    .p_callback          = NULL,
    .p_context           = NULL,
#if defined(VECTOR_NUMBER_FCU_FRDYI)
    .irq                 = VECTOR_NUMBER_FCU_FRDYI,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_FCU_FIFERR)
    .err_irq             = VECTOR_NUMBER_FCU_FIFERR,
#else
    .err_irq             = FSP_INVALID_VECTOR,
#endif
    .err_ipl             = (BSP_IRQ_DISABLED),
    .ipl                 = (BSP_IRQ_DISABLED),
};
/* Instance structure to use this module. */
const flash_instance_t g_flash =
{
    .p_ctrl        = &g_flash_ctrl,
    .p_cfg         = &g_flash_cfg,
    .p_api         = &g_flash_on_flash_hp
};
ceu_instance_ctrl_t g_ceu_hqqvga_ctrl;
            const ceu_extended_cfg_t g_ceu_hqqvga_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_SYNCHRONOUS,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 0,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 120 * 80 * 2,
                .interrupts_enabled   = 0 | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_hqqvga_cfg =
            {
                .x_capture_pixels      = 120,
                .y_capture_pixels      = 80,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_hqqvga_extended_cfg,
            };

            const capture_instance_t g_ceu_hqqvga =
            {
                .p_ctrl = &g_ceu_hqqvga_ctrl,
                .p_cfg =  &g_ceu_hqqvga_cfg,
                .p_api =  &g_ceu_on_capture,
            };
ceu_instance_ctrl_t g_ceu_hvga_ctrl;
            const ceu_extended_cfg_t g_ceu_hvga_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_SYNCHRONOUS,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 0,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 480 * 320 * 2,
                .interrupts_enabled   = 0 | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_hvga_cfg =
            {
                .x_capture_pixels      = 480,
                .y_capture_pixels      = 320,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_hvga_extended_cfg,
            };

            const capture_instance_t g_ceu_hvga =
            {
                .p_ctrl = &g_ceu_hvga_ctrl,
                .p_cfg =  &g_ceu_hvga_cfg,
                .p_api =  &g_ceu_on_capture,
            };
ceu_instance_ctrl_t g_ceu_hqvga_ctrl;
            const ceu_extended_cfg_t g_ceu_hqvga_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_SYNCHRONOUS,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 0,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 240 * 160 * 2,
                .interrupts_enabled   = 0 | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_hqvga_cfg =
            {
                .x_capture_pixels      = 240,
                .y_capture_pixels      = 160,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_hqvga_extended_cfg,
            };

            const capture_instance_t g_ceu_hqvga =
            {
                .p_ctrl = &g_ceu_hqvga_ctrl,
                .p_cfg =  &g_ceu_hqvga_cfg,
                .p_api =  &g_ceu_on_capture,
            };
ceu_instance_ctrl_t g_ceu_qqqvga_ctrl;
            const ceu_extended_cfg_t g_ceu_qqqvga_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_SYNCHRONOUS,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 0,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 80 * 60 * 2,
                .interrupts_enabled   = 0 | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_qqqvga_cfg =
            {
                .x_capture_pixels      = 80,
                .y_capture_pixels      = 60,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_qqqvga_extended_cfg,
            };

            const capture_instance_t g_ceu_qqqvga =
            {
                .p_ctrl = &g_ceu_qqqvga_ctrl,
                .p_cfg =  &g_ceu_qqqvga_cfg,
                .p_api =  &g_ceu_on_capture,
            };
ceu_instance_ctrl_t g_ceu_svga_ctrl;
            const ceu_extended_cfg_t g_ceu_svga_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_SYNCHRONOUS,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 0,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 800 * 600 * 2,
                .interrupts_enabled   = 0 | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_svga_cfg =
            {
                .x_capture_pixels      = 800,
                .y_capture_pixels      = 600,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_svga_extended_cfg,
            };

            const capture_instance_t g_ceu_svga =
            {
                .p_ctrl = &g_ceu_svga_ctrl,
                .p_cfg =  &g_ceu_svga_cfg,
                .p_api =  &g_ceu_on_capture,
            };
ceu_instance_ctrl_t g_ceu_vga_ctrl;
            const ceu_extended_cfg_t g_ceu_vga_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_SYNCHRONOUS,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 0,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 640 * 480 * 2,
                .interrupts_enabled   = 0 | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_vga_cfg =
            {
                .x_capture_pixels      = 640,
                .y_capture_pixels      = 480,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_vga_extended_cfg,
            };

            const capture_instance_t g_ceu_vga =
            {
                .p_ctrl = &g_ceu_vga_ctrl,
                .p_cfg =  &g_ceu_vga_cfg,
                .p_api =  &g_ceu_on_capture,
            };
ceu_instance_ctrl_t g_ceu_qvga_ctrl;
            const ceu_extended_cfg_t g_ceu_qvga_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_SYNCHRONOUS,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 0,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 320 * 240 * 2,
                .interrupts_enabled   = 0 | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_qvga_cfg =
            {
                .x_capture_pixels      = 320,
                .y_capture_pixels      = 240,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_qvga_extended_cfg,
            };

            const capture_instance_t g_ceu_qvga =
            {
                .p_ctrl = &g_ceu_qvga_ctrl,
                .p_cfg =  &g_ceu_qvga_cfg,
                .p_api =  &g_ceu_on_capture,
            };
ceu_instance_ctrl_t g_ceu_qqvga_ctrl;
            const ceu_extended_cfg_t g_ceu_qqvga_extended_cfg =
            {
                .capture_format       = CEU_CAPTURE_FORMAT_DATA_SYNCHRONOUS,
                .data_bus_width       = CEU_DATA_BUS_SIZE_8_BIT,
                .edge_info.dsel       = 0,
                .edge_info.hdsel      = 0,
                .edge_info.vdsel      = 0,
                .hsync_polarity       = CEU_HSYNC_POLARITY_HIGH,
                .vsync_polarity       = CEU_VSYNC_POLARITY_HIGH,
                .byte_swapping        = {
                                        .swap_8bit_units  = (0x1 | 0x2 | 0x4 |  0x0) >> 0x00 & 0x01,
                                        .swap_16bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x01 & 0x01,
                                        .swap_32bit_units = (0x1 | 0x2 | 0x4 |  0x0) >> 0x02 & 0x01,
                                        },
                .burst_mode           = CEU_BURST_TRANSFER_MODE_X8,
                .image_area_size      = 160 * 120 * 2,
                .interrupts_enabled   = 0 | \
                                        R_CEU_CEIER_CPEIE_Msk | \
                                        0 | \
                                        R_CEU_CEIER_VDIE_Msk | \
                                        R_CEU_CEIER_CDTOFIE_Msk | \
                                        0 | \
                                        0 | \
                                        R_CEU_CEIER_VBPIE_Msk | \
                                        R_CEU_CEIER_NHDIE_Msk | \
                                        R_CEU_CEIER_NVDIE_Msk,
                .ceu_ipl              = (8),
                .ceu_irq              = VECTOR_NUMBER_CEU_CEUI,
            };

            const capture_cfg_t g_ceu_qqvga_cfg =
            {
                .x_capture_pixels      = 160,
                .y_capture_pixels      = 120,
                .x_capture_start_pixel = 0,
                .y_capture_start_pixel = 0,
                .bytes_per_pixel       = 2,
                .p_callback            = g_ceu_callback,
                .p_context             = NULL,
                .p_extend              = &g_ceu_qqvga_extended_cfg,
            };

            const capture_instance_t g_ceu_qqvga =
            {
                .p_ctrl = &g_ceu_qqvga_ctrl,
                .p_cfg =  &g_ceu_qqvga_cfg,
                .p_api =  &g_ceu_on_capture,
            };
sci_b_uart_instance_ctrl_t     g_uart9_ctrl;

            sci_b_baud_setting_t               g_uart9_baud_setting =
            {
                /* Baud rate calculated with 0.160% error. */ .baudrate_bits_b.abcse = 0, .baudrate_bits_b.abcs = 0, .baudrate_bits_b.bgdm = 1, .baudrate_bits_b.cks = 0, .baudrate_bits_b.brr = 64, .baudrate_bits_b.mddr = (uint8_t) 256, .baudrate_bits_b.brme = false
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_b_uart_extended_cfg_t g_uart9_cfg_extend =
            {
                .clock                = SCI_B_UART_CLOCK_INT,
                .rx_edge_start          = SCI_B_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_B_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_B_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart9_baud_setting,
                .flow_control           = SCI_B_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting          = {
                    .enable = SCI_B_UART_RS485_DISABLE,
                    .polarity = SCI_B_UART_RS485_DE_POLARITY_HIGH,
                    .assertion_time = 1,
                    .negation_time = 1,
                }
            };

            /** UART interface configuration */
            const uart_cfg_t g_uart9_cfg =
            {
                .channel             = 9,
                .data_bits           = UART_DATA_BITS_8,
                .parity              = UART_PARITY_OFF,
                .stop_bits           = UART_STOP_BITS_1,
                .p_callback          = user_uart9_callback,
                .p_context           = NULL,
                .p_extend            = &g_uart9_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_tx       = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_rx       = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
                .rxi_ipl             = (12),
                .txi_ipl             = (12),
                .tei_ipl             = (12),
                .eri_ipl             = (12),
#if defined(VECTOR_NUMBER_SCI9_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI9_RXI,
#else
                .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI9_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI9_TXI,
#else
                .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI9_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI9_TEI,
#else
                .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI9_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI9_ERI,
#else
                .eri_irq             = FSP_INVALID_VECTOR,
#endif
            };

/* Instance structure to use this module. */
const uart_instance_t g_uart9 =
{
    .p_ctrl        = &g_uart9_ctrl,
    .p_cfg         = &g_uart9_cfg,
    .p_api         = &g_uart_on_sci_b
};
void g_hal_init(void) {
g_common_init();
}
