/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020-2021 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>

#include "py/compile.h"
#include "py/runtime.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "py/mphal.h"
#include "py/stackctrl.h"
#include "shared/readline/readline.h"
#include "shared/runtime/gchelper.h"
#include "shared/runtime/pyexec.h"
#include "tusb.h"
#include "uart.h"
#include "modmachine.h"
#include "modrp2.h"
#include "genhdr/mpversion.h"
#include "pendsv.h"

#if MICROPY_PY_BLUETOOTH
#include "extmod/modbluetooth.h"
#include "mpbthciport.h"
#endif

#if MICROPY_PY_NETWORK
#include "extmod/modnetwork.h"
#endif

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/rtc.h"
#include "hardware/structs/rosc.h"
#include "pico/unique_id.h"
#include "pico/bootrom.h"

#include "omv_boardconfig.h"
#include "framebuffer.h"
#include "cambus.h"
#include "sensor.h"
#include "usbdbg.h"
#include "py_fir.h"
#if MICROPY_PY_AUDIO
#include "py_audio.h"
#endif

#if MICROPY_VFS_FAT && MICROPY_HW_USB_MSC
#include "extmod/vfs.h"
#include "extmod/vfs_fat.h"
#include "common/factoryreset.h"
#endif

extern uint8_t __StackTop, __StackBottom;
static char OMV_ATTR_SECTION(OMV_ATTR_ALIGNED(gc_heap[OMV_HEAP_SIZE], 4), ".heap");

uint8_t *OMV_UNIQUE_ID_ADDR;
static pico_unique_board_id_t OMV_ATTR_ALIGNED(pico_unique_id, 4);

// Embed version info in the binary in machine readable form
bi_decl(bi_program_version_string(MICROPY_GIT_TAG));

// Add a section to the picotool output similar to program features, but for frozen modules
// (it will aggregate BINARY_INFO_ID_MP_FROZEN binary info)
bi_decl(bi_program_feature_group_with_flags(BINARY_INFO_TAG_MICROPYTHON,
    BINARY_INFO_ID_MP_FROZEN, "frozen modules",
    BI_NAMED_GROUP_SEPARATE_COMMAS | BI_NAMED_GROUP_SORT_ALPHA));

void __fatal_error()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        gpio_put(LED_PIN, 1);
        sleep_ms(100);
        gpio_put(LED_PIN, 0);
        sleep_ms(100);
    }
}

void pico_reset_to_bootloader(void)
{
    reset_usb_boot(0, 0);
}

void exec_boot_script(const char *path, bool interruptible)
{
    nlr_buf_t nlr;
    bool interrupted = false;
    if (nlr_push(&nlr) == 0) {
        // Enable IDE interrupts if allowed.
        if (interruptible) {
            usbdbg_set_irq_enabled(true);
            usbdbg_set_script_running(true);
        }

        // Parse, compile and execute the script.
        pyexec_file_if_exists(path, true);
        nlr_pop();
    } else {
        interrupted = true;
    }

    // Disable IDE interrupts
    usbdbg_set_irq_enabled(false);
    usbdbg_set_script_running(false);

    if (interrupted) {
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
    }
}

int main(int argc, char **argv) {
    bool first_soft_reset = true;

    #if MICROPY_HW_ENABLE_UART_REPL
    bi_decl(bi_program_feature("UART REPL"))
    setup_default_uart();
    mp_uart_init();
    #endif

    #if MICROPY_HW_ENABLE_USBDEV
    bi_decl(bi_program_feature("USB REPL"))
    #endif

    #if MICROPY_PY_THREAD
    bi_decl(bi_program_feature("thread support"))
    mp_thread_init();
    #endif

    // Start and initialise the RTC
    datetime_t t = {
        .year = 2021,
        .month = 1,
        .day = 1,
        .dotw = 4, // 0 is Monday, so 4 is Friday
        .hour = 0,
        .min = 0,
        .sec = 0,
    };
    rtc_init();
    rtc_set_datetime(&t);

    // Set board unique ID from flash for USB debugging.
    OMV_UNIQUE_ID_ADDR = pico_unique_id.id;
    pico_get_unique_board_id(&pico_unique_id);

soft_reset:
    // Initialise stack extents and GC heap.
    mp_stack_set_top(&__StackTop);
    mp_stack_set_limit(&__StackTop - &__StackBottom - 256);
    gc_init(&gc_heap[0], &gc_heap[MP_ARRAY_SIZE(gc_heap)]);

    // Initialise MicroPython runtime.
    mp_init();
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_lib));

    // Initialise sub-systems.
    mp_hal_init();
    readline_init0();
    machine_pin_init();
    rp2_pio_init();

    #if MICROPY_PY_BLUETOOTH
    mp_bluetooth_hci_init();
    #endif
    #if MICROPY_PY_NETWORK
    mod_network_init();
    #endif

    pendsv_init();
    usbdbg_init();

    fb_alloc_init0();
    framebuffer_init0();

    py_fir_init0();

    #if MICROPY_PY_SENSOR
    if (sensor_init() != 0) {
        printf("sensor init failed!\n");
    }
    #endif

    #if MICROPY_VFS_FAT && MICROPY_HW_USB_MSC
    // Mount or create a fresh filesystem.
    mp_obj_t mount_point = MP_OBJ_NEW_QSTR(MP_QSTR__slash_);
    mp_obj_t bdev = rp2_flash_type.make_new(&rp2_flash_type, 0, 0, NULL);
    if (mp_vfs_mount_and_chdir_protected(bdev, mount_point) == -MP_ENODEV) {
        // Create a fresh filesystem.
        fs_user_mount_t *vfs  = mp_fat_vfs_type.make_new(&mp_fat_vfs_type, 1, 0, &bdev);
        if (factoryreset_create_filesystem(vfs) == 0) {
            mp_vfs_mount_and_chdir_protected(bdev, mount_point);
        }
    }
    #else
    pyexec_frozen_module("_boot.py", false);
    #endif

    // Execute user scripts.
    exec_boot_script("boot.py", false);

    if (first_soft_reset) {
        exec_boot_script("main.py", true);
    }

    // If there's no script ready, just re-exec REPL
    while (!usbdbg_script_ready()) {
        nlr_buf_t nlr;

        if (nlr_push(&nlr) == 0) {
            // enable IDE interrupt
            usbdbg_set_irq_enabled(true);

            // run REPL
            if (pyexec_mode_kind == PYEXEC_MODE_RAW_REPL) {
                if (pyexec_raw_repl() != 0) {
                    break;
                }
            } else {
                if (pyexec_friendly_repl() != 0) {
                    break;
                }
            }

            nlr_pop();
        }
    }

    if (usbdbg_script_ready()) {
        nlr_buf_t nlr;
        if (nlr_push(&nlr) == 0) {
            // Enable IDE interrupt
            usbdbg_set_irq_enabled(true);
            // Execute the script.
            pyexec_str(usbdbg_get_script(), true);
            nlr_pop();
        } else {
            mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
        }
    }

    usbdbg_wait_for_command(1000);

    usbdbg_set_irq_enabled(false);

    mp_printf(MP_PYTHON_PRINTER, "MPY: soft reboot\n");
    #if MICROPY_PY_AUDIO
    py_audio_deinit();
    #endif
    #if MICROPY_PY_BLUETOOTH
    mp_bluetooth_deinit();
    #endif
    #if MICROPY_PY_NETWORK
    mod_network_deinit();
    #endif
    rp2_pio_deinit();
    machine_pin_deinit();
    gc_sweep_all();
    mp_deinit();
    first_soft_reset = false;
    goto soft_reset;
    return 0;
}

void gc_collect(void) {
    gc_collect_start();
    gc_helper_collect_regs_and_stack();
    #if MICROPY_PY_THREAD
    mp_thread_gc_others();
    #endif
    gc_collect_end();
}

void nlr_jump_fail(void *val) {
    printf("FATAL: uncaught exception %p\n", val);
    mp_obj_print_exception(&mp_plat_print, MP_OBJ_FROM_PTR(val));
    for (;;) {
        __breakpoint();
    }
}

#ifndef NDEBUG
void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr) {
    printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    panic("Assertion failed");
}
#endif

#define POLY (0xD5)

uint8_t rosc_random_u8(size_t cycles) {
    static uint8_t r;
    for (size_t i = 0; i < cycles; ++i) {
        r = ((r << 1) | rosc_hw->randombit) ^ (r & 0x80 ? POLY : 0);
        mp_hal_delay_us_fast(1);
    }
    return r;
}

uint32_t rosc_random_u32(void) {
    uint32_t value = 0;
    for (size_t i = 0; i < 4; ++i) {
        value = value << 8 | rosc_random_u8(32);
    }
    return value;
}

const char rp2_help_text[] =
    "Welcome to MicroPython!\n"
    "\n"
    "For online help please visit https://micropython.org/help/.\n"
    "\n"
    "For access to the hardware use the 'machine' module.  RP2 specific commands\n"
    "are in the 'rp2' module.\n"
    "\n"
    "Quick overview of some objects:\n"
    "  machine.Pin(pin) -- get a pin, eg machine.Pin(0)\n"
    "  machine.Pin(pin, m, [p]) -- get a pin and configure it for IO mode m, pull mode p\n"
    "    methods: init(..), value([v]), high(), low(), irq(handler)\n"
    "  machine.ADC(pin) -- make an analog object from a pin\n"
    "    methods: read_u16()\n"
    "  machine.PWM(pin) -- make a PWM object from a pin\n"
    "    methods: deinit(), freq([f]), duty_u16([d]), duty_ns([d])\n"
    "  machine.I2C(id) -- create an I2C object (id=0,1)\n"
    "    methods: readfrom(addr, buf, stop=True), writeto(addr, buf, stop=True)\n"
    "             readfrom_mem(addr, memaddr, arg), writeto_mem(addr, memaddr, arg)\n"
    "  machine.SPI(id, baudrate=1000000) -- create an SPI object (id=0,1)\n"
    "    methods: read(nbytes, write=0x00), write(buf), write_readinto(wr_buf, rd_buf)\n"
    "  machine.Timer(freq, callback) -- create a software timer object\n"
    "    eg: machine.Timer(freq=1, callback=lambda t:print(t))\n"
    "\n"
    "Pins are numbered 0-29, and 26-29 have ADC capabilities\n"
    "Pin IO modes are: Pin.IN, Pin.OUT, Pin.ALT\n"
    "Pin pull modes are: Pin.PULL_UP, Pin.PULL_DOWN\n"
    "\n"
    "Useful control commands:\n"
    "  CTRL-C -- interrupt a running program\n"
    "  CTRL-D -- on a blank line, do a soft reset of the board\n"
    "  CTRL-E -- on a blank line, enter paste mode\n"
    "\n"
    "For further help on a specific object, type help(obj)\n"
    "For a list of available modules, type help('modules')\n"
;

