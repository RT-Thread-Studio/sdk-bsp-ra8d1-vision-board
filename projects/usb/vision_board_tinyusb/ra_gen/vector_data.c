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
            [4] = usbfs_interrupt_handler, /* USBFS INT (USBFS interrupt) */
            [5] = usbfs_resume_handler, /* USBFS RESUME (USBFS resume interrupt) */
            [6] = usbfs_d0fifo_handler, /* USBFS FIFO 0 (DMA transfer request 0) */
            [7] = usbfs_d1fifo_handler, /* USBFS FIFO 1 (DMA transfer request 1) */
            [8] = usbhs_interrupt_handler, /* USBHS USB INT RESUME (USBHS interr) */
            [9] = usbhs_d0fifo_handler, /* USBHS FIFO 0 (DMA transfer request 0) */
            [10] = usbhs_d1fifo_handler, /* USBHS FIFO 1 (DMA transfer request 1) */
        };
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [0] = BSP_PRV_IELS_ENUM(EVENT_SCI9_RXI), /* SCI9 RXI (Receive data full) */
            [1] = BSP_PRV_IELS_ENUM(EVENT_SCI9_TXI), /* SCI9 TXI (Transmit data empty) */
            [2] = BSP_PRV_IELS_ENUM(EVENT_SCI9_TEI), /* SCI9 TEI (Transmit end) */
            [3] = BSP_PRV_IELS_ENUM(EVENT_SCI9_ERI), /* SCI9 ERI (Receive error) */
            [4] = BSP_PRV_IELS_ENUM(EVENT_USBFS_INT), /* USBFS INT (USBFS interrupt) */
            [5] = BSP_PRV_IELS_ENUM(EVENT_USBFS_RESUME), /* USBFS RESUME (USBFS resume interrupt) */
            [6] = BSP_PRV_IELS_ENUM(EVENT_USBFS_FIFO_0), /* USBFS FIFO 0 (DMA transfer request 0) */
            [7] = BSP_PRV_IELS_ENUM(EVENT_USBFS_FIFO_1), /* USBFS FIFO 1 (DMA transfer request 1) */
            [8] = BSP_PRV_IELS_ENUM(EVENT_USBHS_USB_INT_RESUME), /* USBHS USB INT RESUME (USBHS interr) */
            [9] = BSP_PRV_IELS_ENUM(EVENT_USBHS_FIFO_0), /* USBHS FIFO 0 (DMA transfer request 0) */
            [10] = BSP_PRV_IELS_ENUM(EVENT_USBHS_FIFO_1), /* USBHS FIFO 1 (DMA transfer request 1) */
        };
        #elif __has_include("r_ioport_b.h")
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_IRQ_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
            [BSP_PRV_IELS_ENUM(SCI9_RXI)] = sci_b_uart_rxi_isr, /* SCI9 RXI (Receive data full) */
            [BSP_PRV_IELS_ENUM(SCI9_TXI)] = sci_b_uart_txi_isr, /* SCI9 TXI (Transmit data empty) */
            [BSP_PRV_IELS_ENUM(SCI9_TEI)] = sci_b_uart_tei_isr, /* SCI9 TEI (Transmit end) */
            [BSP_PRV_IELS_ENUM(SCI9_ERI)] = sci_b_uart_eri_isr, /* SCI9 ERI (Receive error) */
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