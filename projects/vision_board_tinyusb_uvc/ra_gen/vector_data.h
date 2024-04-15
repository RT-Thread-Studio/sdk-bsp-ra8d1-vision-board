/* generated vector header file - do not edit */
        #ifndef VECTOR_DATA_H
        #define VECTOR_DATA_H
        #ifdef __cplusplus
        extern "C" {
        #endif
                /* Number of interrupts allocated */
        #ifndef VECTOR_DATA_IRQ_COUNT
        #define VECTOR_DATA_IRQ_COUNT    (20)
        #endif
        /* ISR prototypes */
        void sci_b_uart_rxi_isr(void);
        void sci_b_uart_txi_isr(void);
        void sci_b_uart_tei_isr(void);
        void sci_b_uart_eri_isr(void);
        void ceu_isr(void);
        void glcdc_line_detect_isr(void);
        void mipi_dsi_seq0(void);
        void mipi_dsi_seq1(void);
        void mipi_dsi_vin1(void);
        void mipi_dsi_rcv(void);
        void mipi_dsi_ferr(void);
        void mipi_dsi_ppi(void);
        void drw_int_isr(void);
        void usbfs_interrupt_handler(void);
        void usbfs_resume_handler(void);
        void usbfs_d0fifo_handler(void);
        void usbfs_d1fifo_handler(void);
        void usbhs_interrupt_handler(void);
        void usbhs_d0fifo_handler(void);
        void usbhs_d1fifo_handler(void);

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
        #define VECTOR_NUMBER_CEU_CEUI ((IRQn_Type) 4) /* CEU CEUI (CEU interrupt) */
        #define CEU_CEUI_IRQn          ((IRQn_Type) 4) /* CEU CEUI (CEU interrupt) */
        #define VECTOR_NUMBER_GLCDC_LINE_DETECT ((IRQn_Type) 5) /* GLCDC LINE DETECT (Specified line) */
        #define GLCDC_LINE_DETECT_IRQn          ((IRQn_Type) 5) /* GLCDC LINE DETECT (Specified line) */
        #define VECTOR_NUMBER_MIPI_DSI_SEQ0 ((IRQn_Type) 6) /* DSI SEQ0 */
        #define MIPI_DSI_SEQ0_IRQn          ((IRQn_Type) 6) /* DSI SEQ0 */
        #define VECTOR_NUMBER_MIPI_DSI_SEQ1 ((IRQn_Type) 7) /* DSI SEQ1 */
        #define MIPI_DSI_SEQ1_IRQn          ((IRQn_Type) 7) /* DSI SEQ1 */
        #define VECTOR_NUMBER_MIPI_DSI_VIN1 ((IRQn_Type) 8) /* DSI VIN1 */
        #define MIPI_DSI_VIN1_IRQn          ((IRQn_Type) 8) /* DSI VIN1 */
        #define VECTOR_NUMBER_MIPI_DSI_RCV ((IRQn_Type) 9) /* DSI RCV */
        #define MIPI_DSI_RCV_IRQn          ((IRQn_Type) 9) /* DSI RCV */
        #define VECTOR_NUMBER_MIPI_DSI_FERR ((IRQn_Type) 10) /* DSI FERR */
        #define MIPI_DSI_FERR_IRQn          ((IRQn_Type) 10) /* DSI FERR */
        #define VECTOR_NUMBER_MIPI_DSI_PPI ((IRQn_Type) 11) /* DSI PPI */
        #define MIPI_DSI_PPI_IRQn          ((IRQn_Type) 11) /* DSI PPI */
        #define VECTOR_NUMBER_DRW_INT ((IRQn_Type) 12) /* DRW INT (DRW interrupt) */
        #define DRW_INT_IRQn          ((IRQn_Type) 12) /* DRW INT (DRW interrupt) */
        #define VECTOR_NUMBER_USBFS_INT ((IRQn_Type) 13) /* USBFS INT (USBFS interrupt) */
        #define USBFS_INT_IRQn          ((IRQn_Type) 13) /* USBFS INT (USBFS interrupt) */
        #define VECTOR_NUMBER_USBFS_RESUME ((IRQn_Type) 14) /* USBFS RESUME (USBFS resume interrupt) */
        #define USBFS_RESUME_IRQn          ((IRQn_Type) 14) /* USBFS RESUME (USBFS resume interrupt) */
        #define VECTOR_NUMBER_USBFS_FIFO_0 ((IRQn_Type) 15) /* USBFS FIFO 0 (DMA transfer request 0) */
        #define USBFS_FIFO_0_IRQn          ((IRQn_Type) 15) /* USBFS FIFO 0 (DMA transfer request 0) */
        #define VECTOR_NUMBER_USBFS_FIFO_1 ((IRQn_Type) 16) /* USBFS FIFO 1 (DMA transfer request 1) */
        #define USBFS_FIFO_1_IRQn          ((IRQn_Type) 16) /* USBFS FIFO 1 (DMA transfer request 1) */
        #define VECTOR_NUMBER_USBHS_USB_INT_RESUME ((IRQn_Type) 17) /* USBHS USB INT RESUME (USBHS interr) */
        #define USBHS_USB_INT_RESUME_IRQn          ((IRQn_Type) 17) /* USBHS USB INT RESUME (USBHS interr) */
        #define VECTOR_NUMBER_USBHS_FIFO_0 ((IRQn_Type) 18) /* USBHS FIFO 0 (DMA transfer request 0) */
        #define USBHS_FIFO_0_IRQn          ((IRQn_Type) 18) /* USBHS FIFO 0 (DMA transfer request 0) */
        #define VECTOR_NUMBER_USBHS_FIFO_1 ((IRQn_Type) 19) /* USBHS FIFO 1 (DMA transfer request 1) */
        #define USBHS_FIFO_1_IRQn          ((IRQn_Type) 19) /* USBHS FIFO 1 (DMA transfer request 1) */
        #endif

        #ifdef __cplusplus
        }
        #endif
        #endif /* VECTOR_DATA_H */