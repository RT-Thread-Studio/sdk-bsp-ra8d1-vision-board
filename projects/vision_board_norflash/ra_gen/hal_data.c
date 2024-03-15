/* generated HAL source file - do not edit */
#include "hal_data.h"
ospi_b_instance_ctrl_t g_ospi0_ctrl;

            static const spi_flash_erase_command_t g_ospi0_erase_command_list[] =
            {
            #if ((0x20 > 0) && (4096 > 0))
                {.command = 0x20,     .size = 4096 },
            #endif
            #if ((0xD8 > 0) && (65536 > 0))
                {.command = 0xD8,      .size = 65536  },
            #endif
            #if (0x60 > 0)
                {.command = 0x60,       .size  = SPI_FLASH_ERASE_SIZE_CHIP_ERASE        },
            #endif
            };

            static ospi_b_timing_setting_t g_ospi0_timing_settings =
            {
                .command_to_command_interval = OSPI_B_COMMAND_INTERVAL_CLOCKS_2,
                .cs_pullup_lag               = OSPI_B_COMMAND_CS_PULLUP_CLOCKS_NO_EXTENSION,
                .cs_pulldown_lead            = OSPI_B_COMMAND_CS_PULLDOWN_CLOCKS_NO_EXTENSION
            };

            #if !(0)

             #if (1)
            static const spi_flash_erase_command_t g_ospi0_high_speed_erase_command_list[] =
            {
              #if ((0x2020 > 0) && (4096 > 0))
                {.command = 0x2020,     .size = 4096 },
              #endif
              #if ((0xD8D8 > 0) && (4096 > 0))
                {.command = 0xD8D8,      .size = 65536  },
              #endif
              #if (0x6060 > 0)
                {.command = 0x6060,       .size  = SPI_FLASH_ERASE_SIZE_CHIP_ERASE        },
              #endif
            };
             #endif

            const ospi_b_xspi_command_set_t g_ospi0_high_speed_command_set =
            {
                .protocol             = SPI_FLASH_PROTOCOL_1S_4S_4S,
                .command_bytes        = OSPI_B_COMMAND_BYTES_1,
                .read_command         = 0xEBEB,
                .page_program_command = 0x3232,
                .write_enable_command = 0x0606,
                .status_command       = 0x0505,
                .read_dummy_cycles    = 6,
                .program_dummy_cycles = 0, /* Unused by OSPI Flash */
                .status_dummy_cycles  = 0,
            #if (1)
                .p_erase_command_list      = g_ospi0_high_speed_erase_command_list,
                .erase_command_list_length = sizeof(g_ospi0_high_speed_erase_command_list)/sizeof(g_ospi0_high_speed_erase_command_list[0]),
            #else
                .p_erase_command_list = NULL, /* Use the erase commands specified in spi_flash_cfg_t */
            #endif
            };
            #endif

            #if OSPI_B_CFG_DOTF_SUPPORT_ENABLE
            extern uint32_t g_ospi_dotf_iv[];
            extern uint32_t g_ospi_dotf_key[];

            static ospi_b_dotf_cfg_t g_ospi_dotf_cfg=
            {
                .key_type       = 0,
                .p_start_addr   = (uint32_t *)0x90000000,
                .p_end_addr     = (uint32_t *)0x90001FFF,
                .p_key          = (uint32_t *)g_ospi_dotf_key,
                .p_iv           = (uint32_t *)g_ospi_dotf_iv,
            };
            #endif

            static const ospi_b_extended_cfg_t g_ospi0_extended_cfg =
            {
                .channel                                 = (ospi_b_device_number_t) 1,
                .data_latch_delay_clocks                 = 0x08,
                .p_timing_settings                       = &g_ospi0_timing_settings,
            #if (0)
                .p_xspi_command_set_list                 = ,
                .xspi_command_set_list_length            = 0,
            #else
                .p_xspi_command_set_list                 = &g_ospi0_high_speed_command_set,
                .xspi_command_set_list_length            = 1U,
            #endif
                .p_autocalibration_preamble_pattern_addr = (uint8_t *) 0x00,
            #if OSPI_B_CFG_DMAC_SUPPORT_ENABLE
                .p_lower_lvl_transfer                    = &RA_NOT_DEFINED,
            #endif
            #if OSPI_B_CFG_DOTF_SUPPORT_ENABLE
                .p_dotf_cfg                              = &g_ospi_dotf_cfg,
            #endif
                .read_dummy_cycles                       = 0,
                .program_dummy_cycles                    = 0, /* Unused by OSPI Flash */
                .status_dummy_cycles                     = 0,
            };
            const spi_flash_cfg_t g_ospi0_cfg =
            {
                .spi_protocol                = SPI_FLASH_PROTOCOL_EXTENDED_SPI,
                .read_mode                   = SPI_FLASH_READ_MODE_STANDARD, /* Unused by OSPI Flash */
                .address_bytes               = SPI_FLASH_ADDRESS_BYTES_3,
                .dummy_clocks                = SPI_FLASH_DUMMY_CLOCKS_DEFAULT, /* Unused by OSPI Flash */
                .page_program_address_lines  = (spi_flash_data_lines_t) 0U, /* Unused by OSPI Flash */
                .page_size_bytes             = 64,
                .write_status_bit            = 0,
                .write_enable_bit            = 1,
                .page_program_command        = 0x02,
                .write_enable_command        = 0x06,
                .status_command              = 0x05,
                .read_command                = 0x03,
            #if OSPI_B_CFG_XIP_SUPPORT_ENABLE
                .xip_enter_command           = 0,
                .xip_exit_command            = 0,
            #else
                .xip_enter_command           = 0U,
                .xip_exit_command            = 0U,
            #endif
                .erase_command_list_length   = sizeof(g_ospi0_erase_command_list) / sizeof(g_ospi0_erase_command_list[0]),
                .p_erase_command_list        = &g_ospi0_erase_command_list[0],
                .p_extend                    = &g_ospi0_extended_cfg,
            };
            /** This structure encompasses everything that is needed to use an instance of this interface. */
            const spi_flash_instance_t g_ospi0 =
            {
                .p_ctrl = &g_ospi0_ctrl,
                .p_cfg =  &g_ospi0_cfg,
                .p_api =  &g_ospi_b_on_spi_flash,
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
