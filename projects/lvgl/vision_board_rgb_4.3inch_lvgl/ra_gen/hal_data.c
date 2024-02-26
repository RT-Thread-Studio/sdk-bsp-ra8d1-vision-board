/* generated HAL source file - do not edit */
#include "hal_data.h"

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
