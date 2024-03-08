/* generated vector header file - do not edit */
        #ifndef VECTOR_DATA_H
        #define VECTOR_DATA_H
        #ifdef __cplusplus
        extern "C" {
        #endif
                /* Number of interrupts allocated */
        #ifndef VECTOR_DATA_IRQ_COUNT
        #define VECTOR_DATA_IRQ_COUNT    (15)
        #endif
        /* ISR prototypes */
        void sci_b_uart_rxi_isr(void);
        void sci_b_uart_txi_isr(void);
        void sci_b_uart_tei_isr(void);
        void sci_b_uart_eri_isr(void);
        void glcdc_line_detect_isr(void);
        void mipi_dsi_seq0(void);
        void mipi_dsi_seq1(void);
        void mipi_dsi_vin1(void);
        void mipi_dsi_rcv(void);
        void mipi_dsi_ferr(void);
        void mipi_dsi_ppi(void);
        void drw_int_isr(void);
        void r_icu_isr(void);
        void sci_b_i2c_txi_isr(void);
        void sci_b_i2c_tei_isr(void);

        #if __has_include("r_ioport.h")
        /* Vector table allocations */
        #define VECTOR_NUMBER_SCI9_RXI ((IRQn_Type) 0) /* SCI9 RXI (Receive data full) */
        #define SCI9_RXI_IRQn          ((IRQn_Type) 0) /* SCI9 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI9_TXI ((IRQn_Type) 1) /* SCI9 TXI (Transmit data empty) */
        #define SCI9_TXI_IRQn          ((IRQn_Type) 1) /* SCI9 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI9_TEI ((IRQn_Type) 2) /* SCI9 TEI (Transmit end) */
        #define SCI9_TEI_IRQn          ((IRQn_Type) 2) /* SCI9 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI9_ERI ((IRQn_Type) 3) /* SCI9 ERI (Receive error) */
        #define SCI9_ERI_IRQn          ((IRQn_Type) 3) /* SCI9 ERI (Receive error) */
        #define VECTOR_NUMBER_GLCDC_LINE_DETECT ((IRQn_Type) 4) /* GLCDC LINE DETECT (Specified line) */
        #define GLCDC_LINE_DETECT_IRQn          ((IRQn_Type) 4) /* GLCDC LINE DETECT (Specified line) */
        #define VECTOR_NUMBER_MIPI_DSI_SEQ0 ((IRQn_Type) 5) /* DSI SEQ0 */
        #define MIPI_DSI_SEQ0_IRQn          ((IRQn_Type) 5) /* DSI SEQ0 */
        #define VECTOR_NUMBER_MIPI_DSI_SEQ1 ((IRQn_Type) 6) /* DSI SEQ1 */
        #define MIPI_DSI_SEQ1_IRQn          ((IRQn_Type) 6) /* DSI SEQ1 */
        #define VECTOR_NUMBER_MIPI_DSI_VIN1 ((IRQn_Type) 7) /* DSI VIN1 */
        #define MIPI_DSI_VIN1_IRQn          ((IRQn_Type) 7) /* DSI VIN1 */
        #define VECTOR_NUMBER_MIPI_DSI_RCV ((IRQn_Type) 8) /* DSI RCV */
        #define MIPI_DSI_RCV_IRQn          ((IRQn_Type) 8) /* DSI RCV */
        #define VECTOR_NUMBER_MIPI_DSI_FERR ((IRQn_Type) 9) /* DSI FERR */
        #define MIPI_DSI_FERR_IRQn          ((IRQn_Type) 9) /* DSI FERR */
        #define VECTOR_NUMBER_MIPI_DSI_PPI ((IRQn_Type) 10) /* DSI PPI */
        #define MIPI_DSI_PPI_IRQn          ((IRQn_Type) 10) /* DSI PPI */
        #define VECTOR_NUMBER_DRW_INT ((IRQn_Type) 11) /* DRW INT (DRW interrupt) */
        #define DRW_INT_IRQn          ((IRQn_Type) 11) /* DRW INT (DRW interrupt) */
        #define VECTOR_NUMBER_ICU_IRQ14 ((IRQn_Type) 12) /* ICU IRQ14 (External pin interrupt 14) */
        #define ICU_IRQ14_IRQn          ((IRQn_Type) 12) /* ICU IRQ14 (External pin interrupt 14) */
        #define VECTOR_NUMBER_SCI3_TXI ((IRQn_Type) 13) /* SCI3 TXI (Transmit data empty) */
        #define SCI3_TXI_IRQn          ((IRQn_Type) 13) /* SCI3 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI3_TEI ((IRQn_Type) 14) /* SCI3 TEI (Transmit end) */
        #define SCI3_TEI_IRQn          ((IRQn_Type) 14) /* SCI3 TEI (Transmit end) */
        #endif

        #ifdef __cplusplus
        }
        #endif
        #endif /* VECTOR_DATA_H */