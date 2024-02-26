/*
 * This file is part of the CmBacktrace Library.
 *
 * Copyright (c) 2016-2018, zylx, <1346773219@qq.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Initialize function and other general function.
 * Created on: 2016-12-15
 */
#include <rtthread.h>
#include <rthw.h>
#include <cm_backtrace.h>
#include <string.h>

#if defined(__CC_ARM)
    #pragma O1
#elif defined(__ARMCC_VERSION)
    #pragma optimize ("O1")
#elif defined(__ICCARM__)
    #pragma optimize=none
#elif defined(__GNUC__)
    #pragma GCC optimize ("O0")
#endif

#if defined(__CC_ARM)
    static __inline __asm void cmb_set_psp(uint32_t psp) {
        msr psp, r0
        bx lr
    }
#elif defined(__CLANG_ARM)
    __attribute__( (always_inline) ) static __inline void cmb_set_psp(uint32_t psp) {
        __asm volatile ("msr psp, %0" :: "r" (psp) );
    }
#elif defined(__ICCARM__)
/* IAR iccarm specific functions */
/* Close Raw Asm Code Warning */
#pragma diag_suppress=Pe940
    static void cmb_set_psp(uint32_t psp)
    {
      __asm("msr psp, r0");
      __asm("bx lr");
    }
#pragma diag_default=Pe940
#elif defined(__GNUC__)
    __attribute__( ( always_inline ) ) static inline void cmb_set_psp(uint32_t psp) {
        __asm volatile ("MSR psp, %0\n\t" :: "r" (psp) );
    }
#else
    #error "not supported compiler"
#endif

#if (CMB_CPU_PLATFORM_TYPE == CMB_CPU_ARM_CORTEX_M33)

void rt_cm_backtrace_exception_hook(void *context)
{
#define CMB_LR_WORD_OFFSET_START       0
#define CMB_LR_WORD_OFFSET_END         80

#define EXC_RETURN_SECURE_STATE_MASK     (1UL << 0)    /* always 0: Non-secure state */
#define EXC_RETURN_RESERVED_MASK         (1UL << 1)    /* reserved */
#define EXC_RETURN_RET_STACK_MASK        (1UL << 2)    /* 0: return to MSP, 1: return to PSP */
#define EXC_RETURN_RET_MODE_MASK         (1UL << 3)    /* 0: return to handler mode, 1: return to thread mode */
#define EXC_RETURN_FRAME_TYPE_MASK       (1UL << 4)    /* 0: 26 word(use FPU), 1: 8 word(not use FPU)*/
#define EXC_RETURN_STACKING_RULE_MASK    (1UL << 5)    /* always 1: default rules for stacking the callee register */
#define EXC_RETURN_SECURE_STACK_MASK     (1UL << 6)    /* always 0: Non-secure stack used */

    uint8_t lr_offset = 0;
    uint32_t lr;
    uint32_t *other_info_sp = 0;

    rt_enter_critical();

#ifdef RT_USING_FINSH
    extern long list_thread(void);
    list_thread();
#endif

    /*
     ******************************************************************************************************************
     *
     * 1. Return stack is PSP
     *
     * PSP:            [             sizeof(uint32_t) * 13                  ]
     * ---------------------------------------------------------------------|------------------------------------------
     *                 | EXC_RETURN | tz | lr | psplim | control | R4 - R11 | R0 | R1 | R2 | R3 | LR | PC | xPSR |
     * ---------------------------------------------------------------------|------------------------------------------
     *                 |<--- psp, need to change to point to context        |<--- context
     *                 |<--- other info sp
     *
     * MSP:                |       looking for EXC_RETURN               -------->\\\
     * --------------------|-----------------------------------------------------|-------------------------------------
     *                     |             x x x x x x x x x x                     | EXC_RETURN |
     * --------------------|-----------------------------------------------------|-------------------------------------
     *                     |<--- current SP                                      |<--- pressed before calling
     *                                                                           |     rt_hw_hard_fault_exception
     *
     ******************************************************************************************************************
     *
     * 2. Return stack is MSP
     *
     * MSP:       |   looking for EXC_RETURN       ------->\\\          [ sizeof(uint32_t) * 13 ]
     * -----------|----------------------------------------|------------|-----------------------|----------------------
     *            |      x x x x x x x x x x               | EXC_RETURN | EXC_RETURN ...... R11 | R0 ...... xPSR |
     * -----------|----------------------------------------|------------|-----------------------|----------------------
     *            |<--- current SP                         |<--- pressed before calling         |<--- context
     *                                                     |     rt_hw_hard_fault_exception
     *                                                                  |<--- other info sp
     *
     ******************************************************************************************************************
     */

    /*
     * Look for EXC_RETURN in MSP.
     *
     * Starting from the current SP(MSP), keep looking forward until you find the LR(EXC_RETURN)
     * that was pressed into the MSP before calling rt_hw_hard_fault_exception in the HardFault_Handler function
     * in the context_rvds.S file.
     */
    for (lr_offset = CMB_LR_WORD_OFFSET_START; lr_offset <= CMB_LR_WORD_OFFSET_END; lr_offset++)
    {
        lr = *((uint32_t *)(cmb_get_sp() + sizeof(uint32_t) * lr_offset));
        if ((lr == 0xFFFFFFBC) || (lr == 0xFFFFFFAC) || \
            (lr == 0xFFFFFFB0) || (lr == 0xFFFFFFA0) || (lr == 0xFFFFFFB8) ||(lr == 0xFFFFFFA8))
        {
            break;
        }
    }

    other_info_sp = (uint32_t *)((uint32_t)context - sizeof(uint32_t) * 13);    /* The stack grows down */

    if (lr & EXC_RETURN_RET_STACK_MASK)
    {
        /*
         * Return stack is psp
         *
         * the PSP is changed by RT-Thread HardFault_Handler, so restore it to HardFault context
         * PSP will be used in cm_backtrace_fault, so must restore it.
         *
         * 4 * 13: [ exc_return, tz, lr, psplim, control, r4 - r11 ], skip it, point to [R0 - xPSR]
         */
        cmb_set_psp(cmb_get_psp() + sizeof(uint32_t) * 13);
    }

    cm_backtrace_fault(lr, (uint32_t)context);

    /*
     * print other information
     */
    cmb_println("====================== other information =====================");
    cmb_println("  EXC_RETURN: %08x", *other_info_sp++);
    cmb_println("  tz        : %08x", *other_info_sp++);
    cmb_println("  lr        : %08x", *other_info_sp++);
    cmb_println("  psplim    : %08x", *other_info_sp++);
    cmb_println("  conrtol   : %08x", *other_info_sp++);
    cmb_println("  R4 : %08x  R5 : %08x  R6 : %08x  R7 : %08x  ", \
                   *other_info_sp, *(other_info_sp + 1), *(other_info_sp + 2), *(other_info_sp + 3));
    cmb_println("  R8 : %08x  R9 : %08x  R10: %08x  R11: %08x  ", \
                   *(other_info_sp + 4), *(other_info_sp + 5), *(other_info_sp + 6), *(other_info_sp + 7));
    other_info_sp += 8;
    cmb_println("==============================================================");

    cmb_println("Current system tick: %ld", rt_tick_get());
}

#else

void rt_cm_backtrace_exception_hook(void *context)
{
    uint8_t lr_offset = 0;
    uint32_t lr;

#define CMB_LR_WORD_OFFSET_START       6
#define CMB_LR_WORD_OFFSET_END         20
#define CMB_SP_WORD_OFFSET             (lr_offset + 1)

#if (CMB_CPU_PLATFORM_TYPE == CMB_CPU_ARM_CORTEX_M0) || (CMB_CPU_PLATFORM_TYPE == CMB_CPU_ARM_CORTEX_M3)
#define EXC_RETURN_MASK                0x0000000F // Bits[31:4]
#else
#define EXC_RETURN_MASK                0x0000000F // Bits[31:5]
#endif
        
    rt_enter_critical();

#ifdef RT_USING_FINSH
    extern long list_thread(void);
    list_thread();
#endif

    /* the PSP is changed by RT-Thread HardFault_Handler, so restore it to HardFault context */
#if (defined (__VFP_FP__) && !defined(__SOFTFP__)) || (defined (__ARMVFP__)) || (defined(__ARM_PCS_VFP) || defined(__TARGET_FPU_VFP))
    cmb_set_psp(cmb_get_psp() + 4 * 10);
#else
    cmb_set_psp(cmb_get_psp() + 4 * 9);
#endif

    /* auto calculate the LR offset */
    for (lr_offset = CMB_LR_WORD_OFFSET_START; lr_offset <= CMB_LR_WORD_OFFSET_END; lr_offset ++)
    {
        lr = *((uint32_t *)(cmb_get_sp() + sizeof(uint32_t) * lr_offset));
        /*
         * Cortex-M0: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0497a/Babefdjc.html
         * Cortex-M3: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/Babefdjc.html
         * Cortex-M4: http://infocenter.arm.com/help/topic/com.arm.doc.dui0553b/DUI0553.pdf P41
         * Cortex-M7: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0646c/Babefdjc.html
         */
        if ((lr == 0xFFFFFFF1) || (lr == 0xFFFFFFF9) || (lr == 0xFFFFFFFD) || (lr == 0xFFFFFFE1) || (lr == 0xFFFFFFE9) || (lr == 0xFFFFFFED))
        {
            break;
        }
    }

    cm_backtrace_fault(lr, cmb_get_sp() + sizeof(uint32_t) * CMB_SP_WORD_OFFSET);

    cmb_println("Current system tick: %ld", rt_tick_get());
}

#endif

void rt_cm_backtrace_assert_hook(const char* ex, const char* func, rt_size_t line)
{
    rt_enter_critical();

#ifdef RT_USING_FINSH
    extern long list_thread(void);
    list_thread();
#endif

    cmb_println("");
    cmb_println("(%s) has assert failed at %s:%ld.", ex, func, line);

    cm_backtrace_assert(cmb_get_sp());

    cmb_println("Current system tick: %ld", rt_tick_get());
}

rt_weak rt_err_t exception_hook(void *context) {
    volatile uint8_t _continue = 1;

    rt_cm_backtrace_exception_hook(context);

    while (_continue == 1);

    return RT_EOK;
}

rt_weak void assert_hook(const char* ex, const char* func, rt_size_t line) {
    volatile uint8_t _continue = 1;

    rt_cm_backtrace_assert_hook(ex, func, line);

    while (_continue == 1);
}

int rt_cm_backtrace_init(void) {
    static rt_bool_t is_init = RT_FALSE;

    if (is_init)
    {
        return 0;
    }

    cm_backtrace_init("rtthread","1.0","1.0");
    
    rt_hw_exception_install(exception_hook);

    rt_assert_set_hook(assert_hook);
    
    is_init = RT_TRUE;
    return 0;
}
INIT_DEVICE_EXPORT(rt_cm_backtrace_init);

long cmb_test(int argc, char **argv) {
    volatile int * SCB_CCR = (volatile int *) 0xE000ED14; // SCB->CCR
    int x, y, z;

    if (argc < 2)
    {
        rt_kprintf("Please input 'cmb_test <DIVBYZERO|UNALIGNED|ASSERT>' \n");
        return 0;
    }

    if (!strcmp(argv[1], "DIVBYZERO"))
    {
        *SCB_CCR |= (1 << 4); /* bit4: DIV_0_TRP. */
        x = 10;
        y = rt_strlen("");
        z = x / y;
        rt_kprintf("z:%d\n", z);
        
        return 0;
    }
    else if (!strcmp(argv[1], "UNALIGNED"))
    {
        volatile int * p;
        volatile int value;
        *SCB_CCR |= (1 << 3); /* bit3: UNALIGN_TRP. */

        p = (int *) 0x00;
        value = *p;
        rt_kprintf("addr:0x%02X value:0x%08X\r\n", (int) p, value);

        p = (int *) 0x04;
        value = *p;
        rt_kprintf("addr:0x%02X value:0x%08X\r\n", (int) p, value);

        p = (int *) 0x03;
        value = *p;
        rt_kprintf("addr:0x%02X value:0x%08X\r\n", (int) p, value);
        
        return 0;
    }
    else if (!strcmp(argv[1], "ASSERT"))
    {
        RT_ASSERT(0);
    }
    return 0;
}
MSH_CMD_EXPORT(cmb_test, cm_backtrace_test: cmb_test <DIVBYZERO|UNALIGNED|ASSERT> );
