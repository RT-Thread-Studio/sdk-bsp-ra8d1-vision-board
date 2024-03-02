/* generated vector source file - do not edit */
        #include "bsp_api.h"
        /* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
        #if VECTOR_DATA_IRQ_COUNT > 0
        #if __has_include("r_ioport.h")
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = sci_b_uart_rxi_isr, /* SCI9 RXI (Receive data full) */
            [1] = sci_b_uart_txi_isr, /* SCI9 TXI (Transmit data empty) */
            [2] = sci_b_uart_tei_isr, /* SCI9 TEI (Transmit end) */
            [3] = sci_b_uart_eri_isr, /* SCI9 ERI (Receive error) */
            [4] = ceu_isr, /* CEU CEUI (CEU interrupt) */
            [5] = glcdc_line_detect_isr, /* GLCDC LINE DETECT (Specified line) */
            [6] = mipi_dsi_seq0, /* DSI SEQ0 */
            [7] = mipi_dsi_seq1, /* DSI SEQ1 */
            [8] = mipi_dsi_vin1, /* DSI VIN1 */
            [9] = mipi_dsi_rcv, /* DSI RCV */
            [10] = mipi_dsi_ferr, /* DSI FERR */
            [11] = mipi_dsi_ppi, /* DSI PPI */
            [12] = drw_int_isr, /* DRW INT (DRW interrupt) */
            [13] = sci_b_uart_rxi_isr, /* SCI2 RXI (Receive data full) */
            [14] = sci_b_uart_txi_isr, /* SCI2 TXI (Transmit data empty) */
            [15] = sci_b_uart_tei_isr, /* SCI2 TEI (Transmit end) */
            [16] = sci_b_uart_eri_isr, /* SCI2 ERI (Receive error) */
            [17] = sdhimmc_accs_isr, /* SDHIMMC1 ACCS (Card access) */
            [18] = sdhimmc_card_isr, /* SDHIMMC1 CARD (Card detect) */
            [19] = dmac_int_isr, /* DMAC0 INT (DMAC0 transfer end 0) */
            [20] = sci_b_i2c_txi_isr, /* SCI3 TXI (Transmit data empty) */
            [21] = sci_b_i2c_tei_isr, /* SCI3 TEI (Transmit end) */
            [22] = spi_b_rxi_isr, /* SPI1 RXI (Receive buffer full) */
            [23] = spi_b_txi_isr, /* SPI1 TXI (Transmit buffer empty) */
            [24] = spi_b_tei_isr, /* SPI1 TEI (Transmission complete event) */
            [25] = spi_b_eri_isr, /* SPI1 ERI (Error) */
            [26] = usbfs_interrupt_handler, /* USBFS INT (USBFS interrupt) */
            [27] = usbfs_resume_handler, /* USBFS RESUME (USBFS resume interrupt) */
            [28] = usbfs_d0fifo_handler, /* USBFS FIFO 0 (DMA transfer request 0) */
            [29] = usbfs_d1fifo_handler, /* USBFS FIFO 1 (DMA transfer request 1) */
            [30] = usbhs_interrupt_handler, /* USBHS USB INT RESUME (USBHS interr) */
            [31] = usbhs_d0fifo_handler, /* USBHS FIFO 0 (DMA transfer request 0) */
            [32] = usbhs_d1fifo_handler, /* USBHS FIFO 1 (DMA transfer request 1) */
        };
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [0] = BSP_PRV_IELS_ENUM(EVENT_SCI9_RXI), /* SCI9 RXI (Receive data full) */
            [1] = BSP_PRV_IELS_ENUM(EVENT_SCI9_TXI), /* SCI9 TXI (Transmit data empty) */
            [2] = BSP_PRV_IELS_ENUM(EVENT_SCI9_TEI), /* SCI9 TEI (Transmit end) */
            [3] = BSP_PRV_IELS_ENUM(EVENT_SCI9_ERI), /* SCI9 ERI (Receive error) */
            [4] = BSP_PRV_IELS_ENUM(EVENT_CEU_CEUI), /* CEU CEUI (CEU interrupt) */
            [5] = BSP_PRV_IELS_ENUM(EVENT_GLCDC_LINE_DETECT), /* GLCDC LINE DETECT (Specified line) */
            [6] = BSP_PRV_IELS_ENUM(EVENT_MIPI_DSI_SEQ0), /* DSI SEQ0 */
            [7] = BSP_PRV_IELS_ENUM(EVENT_MIPI_DSI_SEQ1), /* DSI SEQ1 */
            [8] = BSP_PRV_IELS_ENUM(EVENT_MIPI_DSI_VIN1), /* DSI VIN1 */
            [9] = BSP_PRV_IELS_ENUM(EVENT_MIPI_DSI_RCV), /* DSI RCV */
            [10] = BSP_PRV_IELS_ENUM(EVENT_MIPI_DSI_FERR), /* DSI FERR */
            [11] = BSP_PRV_IELS_ENUM(EVENT_MIPI_DSI_PPI), /* DSI PPI */
            [12] = BSP_PRV_IELS_ENUM(EVENT_DRW_INT), /* DRW INT (DRW interrupt) */
            [13] = BSP_PRV_IELS_ENUM(EVENT_SCI2_RXI), /* SCI2 RXI (Receive data full) */
            [14] = BSP_PRV_IELS_ENUM(EVENT_SCI2_TXI), /* SCI2 TXI (Transmit data empty) */
            [15] = BSP_PRV_IELS_ENUM(EVENT_SCI2_TEI), /* SCI2 TEI (Transmit end) */
            [16] = BSP_PRV_IELS_ENUM(EVENT_SCI2_ERI), /* SCI2 ERI (Receive error) */
            [17] = BSP_PRV_IELS_ENUM(EVENT_SDHIMMC1_ACCS), /* SDHIMMC1 ACCS (Card access) */
            [18] = BSP_PRV_IELS_ENUM(EVENT_SDHIMMC1_CARD), /* SDHIMMC1 CARD (Card detect) */
            [19] = BSP_PRV_IELS_ENUM(EVENT_DMAC0_INT), /* DMAC0 INT (DMAC0 transfer end 0) */
            [20] = BSP_PRV_IELS_ENUM(EVENT_SCI3_TXI), /* SCI3 TXI (Transmit data empty) */
            [21] = BSP_PRV_IELS_ENUM(EVENT_SCI3_TEI), /* SCI3 TEI (Transmit end) */
            [22] = BSP_PRV_IELS_ENUM(EVENT_SPI1_RXI), /* SPI1 RXI (Receive buffer full) */
            [23] = BSP_PRV_IELS_ENUM(EVENT_SPI1_TXI), /* SPI1 TXI (Transmit buffer empty) */
            [24] = BSP_PRV_IELS_ENUM(EVENT_SPI1_TEI), /* SPI1 TEI (Transmission complete event) */
            [25] = BSP_PRV_IELS_ENUM(EVENT_SPI1_ERI), /* SPI1 ERI (Error) */
			[26] = BSP_PRV_IELS_ENUM(EVENT_USBFS_INT), /* USBFS INT (USBFS interrupt) */
            [27] = BSP_PRV_IELS_ENUM(EVENT_USBFS_RESUME), /* USBFS RESUME (USBFS resume interrupt) */
            [28] = BSP_PRV_IELS_ENUM(EVENT_USBFS_FIFO_0), /* USBFS FIFO 0 (DMA transfer request 0) */
            [29] = BSP_PRV_IELS_ENUM(EVENT_USBFS_FIFO_1), /* USBFS FIFO 1 (DMA transfer request 1) */
            [30] = BSP_PRV_IELS_ENUM(EVENT_USBHS_USB_INT_RESUME), /* USBHS USB INT RESUME (USBHS interr) */
            [31] = BSP_PRV_IELS_ENUM(EVENT_USBHS_FIFO_0), /* USBHS FIFO 0 (DMA transfer request 0) */
            [32] = BSP_PRV_IELS_ENUM(EVENT_USBHS_FIFO_1), /* USBHS FIFO 1 (DMA transfer request 1) */
        };
        #elif __has_include("r_ioport_b.h")
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_IRQ_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
            [BSP_PRV_IELS_ENUM(SCI9_RXI)] = sci_b_uart_rxi_isr, /* SCI9 RXI (Receive data full) */
            [BSP_PRV_IELS_ENUM(SCI9_TXI)] = sci_b_uart_txi_isr, /* SCI9 TXI (Transmit data empty) */
            [BSP_PRV_IELS_ENUM(SCI9_TEI)] = sci_b_uart_tei_isr, /* SCI9 TEI (Transmit end) */
            [BSP_PRV_IELS_ENUM(SCI9_ERI)] = sci_b_uart_eri_isr, /* SCI9 ERI (Receive error) */
            [BSP_PRV_IELS_ENUM(CEU_CEUI)] = ceu_isr, /* CEU CEUI (CEU interrupt) */
            [BSP_PRV_IELS_ENUM(GLCDC_LINE_DETECT)] = glcdc_line_detect_isr, /* GLCDC LINE DETECT (Specified line) */
            [BSP_PRV_IELS_ENUM(MIPI_DSI_SEQ0)] = mipi_dsi_seq0, /* DSI SEQ0 */
            [BSP_PRV_IELS_ENUM(MIPI_DSI_SEQ1)] = mipi_dsi_seq1, /* DSI SEQ1 */
            [BSP_PRV_IELS_ENUM(MIPI_DSI_VIN1)] = mipi_dsi_vin1, /* DSI VIN1 */
            [BSP_PRV_IELS_ENUM(MIPI_DSI_RCV)] = mipi_dsi_rcv, /* DSI RCV */
            [BSP_PRV_IELS_ENUM(MIPI_DSI_FERR)] = mipi_dsi_ferr, /* DSI FERR */
            [BSP_PRV_IELS_ENUM(MIPI_DSI_PPI)] = mipi_dsi_ppi, /* DSI PPI */
            [BSP_PRV_IELS_ENUM(DRW_INT)] = drw_int_isr, /* DRW INT (DRW interrupt) */
            [BSP_PRV_IELS_ENUM(SCI2_RXI)] = sci_b_uart_rxi_isr, /* SCI2 RXI (Receive data full) */
            [BSP_PRV_IELS_ENUM(SCI2_TXI)] = sci_b_uart_txi_isr, /* SCI2 TXI (Transmit data empty) */
            [BSP_PRV_IELS_ENUM(SCI2_TEI)] = sci_b_uart_tei_isr, /* SCI2 TEI (Transmit end) */
            [BSP_PRV_IELS_ENUM(SCI2_ERI)] = sci_b_uart_eri_isr, /* SCI2 ERI (Receive error) */
            [BSP_PRV_IELS_ENUM(SDHIMMC1_ACCS)] = sdhimmc_accs_isr, /* SDHIMMC1 ACCS (Card access) */
            [BSP_PRV_IELS_ENUM(SDHIMMC1_CARD)] = sdhimmc_card_isr, /* SDHIMMC1 CARD (Card detect) */
            [BSP_PRV_IELS_ENUM(DMAC0_INT)] = dmac_int_isr, /* DMAC0 INT (DMAC0 transfer end 0) */
            [BSP_PRV_IELS_ENUM(SCI3_TXI)] = sci_b_i2c_txi_isr, /* SCI3 TXI (Transmit data empty) */
            [BSP_PRV_IELS_ENUM(SCI3_TEI)] = sci_b_i2c_tei_isr, /* SCI3 TEI (Transmit end) */
            [BSP_PRV_IELS_ENUM(SPI1_RXI)] = spi_b_rxi_isr, /* SPI1 RXI (Receive buffer full) */
            [BSP_PRV_IELS_ENUM(SPI1_TXI)] = spi_b_txi_isr, /* SPI1 TXI (Transmit buffer empty) */
            [BSP_PRV_IELS_ENUM(SPI1_TEI)] = spi_b_tei_isr, /* SPI1 TEI (Transmission complete event) */
            [BSP_PRV_IELS_ENUM(SPI1_ERI)] = spi_b_eri_isr, /* SPI1 ERI (Error) */
			[BSP_PRV_IELS_ENUM(USBFS_INT)] = usbfs_interrupt_handler, /* USBFS INT (USBFS interrupt) */
            [BSP_PRV_IELS_ENUM(USBFS_RESUME)] = usbfs_resume_handler, /* USBFS RESUME (USBFS resume interrupt) */
            [BSP_PRV_IELS_ENUM(USBFS_FIFO_0)] = usbfs_d0fifo_handler, /* USBFS FIFO 0 (DMA transfer request 0) */
            [BSP_PRV_IELS_ENUM(USBFS_FIFO_1)] = usbfs_d1fifo_handler, /* USBFS FIFO 1 (DMA transfer request 1) */
            [BSP_PRV_IELS_ENUM(USBHS_USB_INT_RESUME)] = usbhs_interrupt_handler, /* USBHS USB INT RESUME (USBHS interr) */
            [BSP_PRV_IELS_ENUM(USBHS_FIFO_0)] = usbhs_d0fifo_handler, /* USBHS FIFO 0 (DMA transfer request 0) */
            [BSP_PRV_IELS_ENUM(USBHS_FIFO_1)] = usbhs_d1fifo_handler, /* USBHS FIFO 1 (DMA transfer request 1) */
        };
        #endif
        #endif