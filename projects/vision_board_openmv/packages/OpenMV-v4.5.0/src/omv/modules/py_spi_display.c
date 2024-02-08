/*
 * This file is part of the OpenMV project.
 *
 * Copyright (c) 2013-2021 Ibrahim Abdelkader <iabdalkader@openmv.io>
 * Copyright (c) 2013-2021 Kwabena W. Agyeman <kwagyeman@openmv.io>
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * SPI Display Python module.
 */
#include "omv_boardconfig.h"

#if MICROPY_PY_DISPLAY && defined(OMV_SPI_DISPLAY_CONTROLLER)

#include "py/obj.h"
#include "py/nlr.h"
#include "py/runtime.h"
#include "mphal.h"

#include "py_image.h"
#include "omv_gpio.h"
#include "omv_spi.h"
#include "py_display.h"

#define LCD_COMMAND_DISPOFF         (0x28)
#define LCD_COMMAND_DISPON          (0x29)
#define LCD_COMMAND_RAMWR           (0x2C)
#define LCD_COMMAND_SLPOUT          (0x11)
#define LCD_COMMAND_MADCTL          (0x36)
#define LCD_COMMAND_COLMOD          (0x3A)

#if OMV_SPI_DISPLAY_TRIPLE_BUFFER
#define LCD_TRIPLE_BUFFER_DEFAULT   (true)
#else
#define LCD_TRIPLE_BUFFER_DEFAULT   (false)
#endif

static void spi_transmit(py_display_obj_t *self, uint8_t *txdata, uint16_t size) {
    omv_spi_transfer_t spi_xfer = {
        .txbuf = txdata,
        .size = size,
        .timeout = OMV_SPI_MAX_TIMEOUT,
        .flags = OMV_SPI_XFER_BLOCKING
    };

    omv_gpio_write(OMV_SPI_DISPLAY_SSEL_PIN, 0);
    omv_spi_transfer_start(&self->spi_bus, &spi_xfer);
    omv_gpio_write(OMV_SPI_DISPLAY_SSEL_PIN, 1);
}

static void spi_transmit_16(py_display_obj_t *self, uint8_t *txdata, uint16_t size) {
    omv_spi_transfer_t spi_xfer = {
        .txbuf = txdata,
        .size = (!self->byte_swap) ? size : (size * 2),
        .timeout = OMV_SPI_MAX_TIMEOUT,
        .flags = OMV_SPI_XFER_BLOCKING,
    };

    omv_spi_transfer_start(&self->spi_bus, &spi_xfer);
}

static void spi_switch_mode(py_display_obj_t *self, int bits, bool dma) {
    omv_spi_deinit(&self->spi_bus);

    omv_spi_config_t spi_config;
    omv_spi_default_config(&spi_config, OMV_SPI_DISPLAY_CONTROLLER);

    spi_config.baudrate = self->spi_baudrate;
    spi_config.datasize = bits;
    spi_config.bus_mode = OMV_SPI_BUS_TX;
    spi_config.nss_enable = false;
    spi_config.dma_flags = dma ? OMV_SPI_DMA_NORMAL : 0;
    omv_spi_init(&self->spi_bus, &spi_config);
}

static void spi_display_command(py_display_obj_t *self, uint8_t cmd, uint8_t arg) {
    omv_gpio_write(OMV_SPI_DISPLAY_RS_PIN, 0);
    spi_transmit(self, (uint8_t []) { cmd }, 1);
    omv_gpio_write(OMV_SPI_DISPLAY_RS_PIN, 1);
    if (arg) {
        spi_transmit(self, (uint8_t []) { arg }, 1);
    }
}

static void spi_display_callback(omv_spi_t *spi, void *userdata, void *buf) {
    py_display_obj_t *self = (py_display_obj_t *) userdata;

    static uint8_t *spi_state_write_addr = NULL;
    static size_t spi_state_write_count = 0;

    // If userdata is not null then it means that we are being kicked off.
    if (buf == NULL) {
        spi_state_write_count = 0;
    }

    if (!spi_state_write_count) {
        spi_state_write_addr = (uint8_t *) self->framebuffers[self->framebuffer_tail];
        spi_state_write_count = self->width * self->height;

        if (self->byte_swap) {
            spi_state_write_count *= 2;
        }

        self->framebuffer_head = self->framebuffer_tail;
    }

    size_t spi_state_write_limit = (!self->byte_swap) ? OMV_SPI_MAX_16BIT_XFER : OMV_SPI_MAX_8BIT_XFER;
    uint8_t *addr = spi_state_write_addr;
    size_t count = IM_MIN(spi_state_write_count, spi_state_write_limit);

    spi_state_write_addr += (!self->byte_swap) ? (count * 2) : count;
    spi_state_write_count -= count;

    // When starting the interrupt chain the first transfer is not executed in interrupt context.
    // So, disable interrupts for the first transfer so that it completes first and unlocks the
    // SPI bus before allowing the interrupt it causes to trigger starting the interrupt chain.
    omv_spi_transfer_t spi_xfer = {
        .txbuf = addr,
        .size = count,
        .flags = OMV_SPI_XFER_DMA,
        .userdata = self,
        .callback = spi_display_callback,
    };

    if (buf == NULL) {
        uint32_t irq_state = disable_irq();
        omv_spi_transfer_start(&self->spi_bus, &spi_xfer);
        enable_irq(irq_state);
    } else {
        omv_spi_transfer_start(&self->spi_bus, &spi_xfer);
    }
}

static void spi_display_kick(py_display_obj_t *self) {
    if (!self->spi_tx_running) {
        spi_display_command(self, LCD_COMMAND_RAMWR, 0);
        spi_switch_mode(self, (!self->byte_swap) ? 16 : 8, true);
        omv_gpio_write(OMV_SPI_DISPLAY_SSEL_PIN, 0);

        // Limit the transfer size to single lines as you cannot send more
        // than 64KB per SPI transaction generally.
        for (int i = 0; i < self->height; i++) {
            uint8_t *buffer = (uint8_t *) (self->framebuffers[self->framebuffer_tail] + (self->width * i));
            spi_transmit_16(self, buffer, self->width);
        }

        spi_switch_mode(self, 8, false);
        omv_gpio_write(OMV_SPI_DISPLAY_SSEL_PIN, 1);
        spi_display_command(self, LCD_COMMAND_DISPON, 0);
        spi_display_command(self, LCD_COMMAND_RAMWR, 0);
        spi_switch_mode(self, (!self->byte_swap) ? 16 : 8, true);
        omv_gpio_write(OMV_SPI_DISPLAY_SSEL_PIN, 0);

        // Kickoff interrupt driven image update.
        self->spi_tx_running = true;
        spi_display_callback(&self->spi_bus, self, NULL);
    }
}

// TODO remove this when imlib_draw_image accepts a context.
static py_display_obj_t *lcd_self;

static void spi_display_draw_image_cb(int x_start, int x_end, int y_row, imlib_draw_row_data_t *data) {
    spi_transmit_16(lcd_self, data->dst_row_override, lcd_self->width);
}

static void spi_display_write(py_display_obj_t *self, image_t *src_img, int dst_x_start, int dst_y_start,
                              float x_scale, float y_scale, rectangle_t *roi, int rgb_channel, int alpha,
                              const uint16_t *color_palette, const uint8_t *alpha_palette, image_hint_t hint) {
    image_t dst_img;
    dst_img.w = self->width;
    dst_img.h = self->height;
    dst_img.pixfmt = PIXFORMAT_RGB565;

    int x0, x1, y0, y1;
    bool black = !imlib_draw_image_rectangle(&dst_img, src_img, dst_x_start, dst_y_start, x_scale,
                                             y_scale, roi, alpha, alpha_palette, hint, &x0, &x1, &y0, &y1);

    if (!self->triple_buffer) {
        dst_img.data = fb_alloc0(self->width * sizeof(uint16_t), FB_ALLOC_NO_HINT);

        spi_display_command(self, LCD_COMMAND_RAMWR, 0);
        spi_switch_mode(self, (!self->byte_swap) ? 16 : 8, true);
        omv_gpio_write(OMV_SPI_DISPLAY_SSEL_PIN, 0);

        if (black) {
            // zero the whole image
            for (int i = 0; i < self->height; i++) {
                spi_transmit_16(self, dst_img.data, self->width);
            }
        } else {
            // Zero the top rows
            for (int i = 0; i < y0; i++) {
                spi_transmit_16(self, dst_img.data, self->width);
            }

            // TODO remove this when imlib_draw_image accepts a context.
            lcd_self = self;

            // Transmits left/right parts already zeroed...
            imlib_draw_image(&dst_img, src_img, dst_x_start, dst_y_start,
                             x_scale, y_scale, roi, rgb_channel, alpha, color_palette, alpha_palette,
                             hint | IMAGE_HINT_BLACK_BACKGROUND, spi_display_draw_image_cb, dst_img.data);
            // Zero the bottom rows
            if (y1 < self->height) {
                memset(dst_img.data, 0, self->width * sizeof(uint16_t));
            }

            for (int i = y1; i < self->height; i++) {
                spi_transmit_16(self, dst_img.data, self->width);
            }
        }

        spi_switch_mode(self, 8, false);
        omv_gpio_write(OMV_SPI_DISPLAY_SSEL_PIN, 1);
        spi_display_command(self, LCD_COMMAND_DISPON, 0);
        fb_free();
    } else {
        // For triple buffering we are never drawing where tail or head
        // (which may instantly update to to be equal to tail) is.
        int new_framebuffer_tail = (self->framebuffer_tail + 1) % FRAMEBUFFER_COUNT;
        if (new_framebuffer_tail == self->framebuffer_head) {
            new_framebuffer_tail = (new_framebuffer_tail + 1) % FRAMEBUFFER_COUNT;
        }
        dst_img.data = (uint8_t *) self->framebuffers[new_framebuffer_tail];

        if (black) {
            // zero the whole image
            memset(dst_img.data, 0, self->width * self->height * sizeof(uint16_t));
        } else {
            // Zero the top rows
            if (y0) {
                memset(dst_img.data, 0, self->width * y0 * sizeof(uint16_t));
            }

            if (x0) {
                for (int i = y0; i < y1; i++) {
                    // Zero left
                    memset(IMAGE_COMPUTE_RGB565_PIXEL_ROW_PTR(&dst_img, i), 0, x0 * sizeof(uint16_t));
                }
            }

            imlib_draw_image(&dst_img, src_img, dst_x_start, dst_y_start,
                             x_scale, y_scale, roi, rgb_channel, alpha, color_palette,
                             alpha_palette, hint | IMAGE_HINT_BLACK_BACKGROUND, NULL, NULL);

            if (self->width - x1) {
                for (int i = y0; i < y1; i++) {
                    // Zero right
                    memset(IMAGE_COMPUTE_RGB565_PIXEL_ROW_PTR(&dst_img, i) + x1, 0, (self->width - x1) * sizeof(uint16_t));
                }
            }

            // Zero the bottom rows
            if (self->height - y1) {
                memset(IMAGE_COMPUTE_RGB565_PIXEL_ROW_PTR(&dst_img, y1),
                       0, self->width * (self->height - y1) * sizeof(uint16_t));
            }
        }

        #ifdef __DCACHE_PRESENT
        // Flush data for DMA
        SCB_CleanDCache_by_Addr((uint32_t *) dst_img.data, image_size(&dst_img));
        #endif

        // Update tail which means a new image is ready.
        self->framebuffer_tail = new_framebuffer_tail;

        // Kick off an update of the display.
        spi_display_kick(self);
    }
}

static void spi_display_clear(py_display_obj_t *self, bool display_off) {
    if (display_off) {
        // turns the display off (may not be black)
        if (self->spi_tx_running) {
            omv_spi_transfer_abort(&self->spi_bus);
            self->spi_tx_running = false;
            spi_switch_mode(self, 8, false);
            omv_gpio_write(OMV_SPI_DISPLAY_SSEL_PIN, 1);
        }
    } else {
        spi_display_command(self, LCD_COMMAND_DISPOFF, 0);
        fb_alloc_mark();
        spi_display_write(self, NULL, 0, 0, 1.f, 1.f, NULL, 0, 0, NULL, NULL, 0);
        fb_alloc_free_till_mark();
    }
}

#ifdef OMV_DISPLAY_BL_DAC
static DAC_HandleTypeDef lcd_dac_handle;
#endif

#ifdef OMV_SPI_DISPLAY_BL_PIN
static void spi_display_set_backlight(py_display_obj_t *self, uint32_t intensity) {
    #ifdef OMV_DISPLAY_BL_DAC
    if ((self->intensity < 255) && (255 <= intensity)) {
    #else
    if ((self->intensity < 1) && (1 <= intensity)) {
    #endif
        omv_gpio_write(OMV_SPI_DISPLAY_BL_PIN, 1);
        omv_gpio_deinit(OMV_SPI_DISPLAY_BL_PIN);
    } else if ((0 < self->intensity) && (intensity <= 0)) {
        omv_gpio_config(OMV_SPI_DISPLAY_BL_PIN, OMV_GPIO_MODE_OUTPUT, OMV_GPIO_PULL_NONE, OMV_GPIO_SPEED_LOW, -1);
        omv_gpio_write(OMV_SPI_DISPLAY_BL_PIN, 0);
    }

    #ifdef OMV_DISPLAY_BL_DAC
    if (((self->intensity <= 0) || (255 <= self->intensity)) && (0 < intensity) && (intensity < 255)) {
        omv_gpio_config(OMV_SPI_DISPLAY_BL_PIN, OMV_GPIO_MODE_ANALOG, OMV_GPIO_PULL_NONE, OMV_GPIO_SPEED_LOW, -1);

        DAC_ChannelConfTypeDef lcd_dac_channel_handle;
        lcd_dac_handle.Instance = OMV_DISPLAY_BL_DAC;
        lcd_dac_channel_handle.DAC_Trigger = DAC_TRIGGER_NONE;
        lcd_dac_channel_handle.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
        #if defined(MCU_SERIES_H7)
        lcd_dac_channel_handle.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
        lcd_dac_channel_handle.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
        lcd_dac_channel_handle.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
        #endif

        HAL_DAC_Init(&lcd_dac_handle);
        HAL_DAC_ConfigChannel(&lcd_dac_handle, &lcd_dac_channel_handle, OMV_DISPLAY_BL_DAC_CHANNEL);
        HAL_DAC_Start(&lcd_dac_handle, OMV_DISPLAY_BL_DAC_CHANNEL);
        HAL_DAC_SetValue(&lcd_dac_handle, OMV_DISPLAY_BL_DAC_CHANNEL, DAC_ALIGN_8B_R, intensity);
    } else if ((0 < self->intensity) && (self->intensity < 255) && ((intensity <= 0) || (255 <= intensity))) {
        HAL_DAC_Stop(&lcd_dac_handle, OMV_DISPLAY_BL_DAC_CHANNEL);
        HAL_DAC_DeInit(&lcd_dac_handle);
    } else if ((0 < self->intensity) && (self->intensity < 255) && (0 < intensity) && (intensity < 255)) {
        HAL_DAC_SetValue(&lcd_dac_handle, OMV_DISPLAY_BL_DAC_CHANNEL, DAC_ALIGN_8B_R, intensity);
    }
    #endif

    self->intensity = intensity;
}
#endif // OMV_SPI_DISPLAY_BL_PIN

static void spi_display_deinit(py_display_obj_t *self) {
    if (self->triple_buffer) {
        omv_spi_transfer_abort(&self->spi_bus);
        fb_alloc_free_till_mark_past_mark_permanent();
    }

    omv_spi_deinit(&self->spi_bus);
    omv_gpio_deinit(OMV_SPI_DISPLAY_RS_PIN);
    omv_gpio_deinit(OMV_SPI_DISPLAY_RST_PIN);

    #ifdef OMV_SPI_DISPLAY_BL_PIN
    spi_display_set_backlight(self, 255);
    #endif
}

mp_obj_t spi_display_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    enum { ARG_width, ARG_height, ARG_refresh, ARG_triple_buffer, ARG_bgr, ARG_byte_swap};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_width,         MP_ARG_INT,  {.u_int = 128  } },
        { MP_QSTR_height,        MP_ARG_INT,  {.u_int = 160  } },
        { MP_QSTR_refresh,       MP_ARG_INT,  {.u_int = 60   } },
        { MP_QSTR_bgr,           MP_ARG_BOOL, {.u_bool = false} },
        { MP_QSTR_byte_swap,     MP_ARG_BOOL, {.u_bool = false} },
        { MP_QSTR_triple_buffer, MP_ARG_BOOL, {.u_bool = LCD_TRIPLE_BUFFER_DEFAULT} },
    };

    // Parse args.
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    if ((args[ARG_width].u_int <= 0) || (args[ARG_width].u_int > 32767)) {
        mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Invalid Width!"));
    }
    if ((args[ARG_height].u_int <= 0) || (args[ARG_height].u_int > 32767)) {
        mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Invalid Height!"));
    }
    if ((args[ARG_refresh].u_int < 30) || (args[ARG_refresh].u_int > 120)) {
        mp_raise_msg(&mp_type_ValueError, MP_ERROR_TEXT("Invalid Refresh Rate!"));
    }

    py_display_obj_t *self = m_new_obj_with_finaliser(py_display_obj_t);
    self->base.type = &py_spi_display_type;
    self->framebuffer_tail = 0;
    self->framebuffer_head = 0;
    self->width = args[ARG_width].u_int;
    self->height = args[ARG_height].u_int;
    self->refresh = args[ARG_refresh].u_int;
    self->triple_buffer = args[ARG_triple_buffer].u_bool;
    self->bgr = args[ARG_bgr].u_bool;
    self->byte_swap = args[ARG_byte_swap].u_bool;

    omv_spi_config_t spi_config;
    omv_spi_default_config(&spi_config, OMV_SPI_DISPLAY_CONTROLLER);

    self->spi_baudrate = self->width * self->height * self->refresh * 16;
    spi_config.baudrate = self->spi_baudrate;
    spi_config.bus_mode = OMV_SPI_BUS_TX;
    spi_config.nss_enable = false;
    omv_spi_init(&self->spi_bus, &spi_config);
    omv_gpio_write(OMV_SPI_DISPLAY_SSEL_PIN, 1);

    omv_gpio_config(OMV_SPI_DISPLAY_RST_PIN, OMV_GPIO_MODE_OUTPUT, OMV_GPIO_PULL_NONE, OMV_GPIO_SPEED_LOW, -1);
    omv_gpio_write(OMV_SPI_DISPLAY_RST_PIN, 1);

    omv_gpio_config(OMV_SPI_DISPLAY_RS_PIN, OMV_GPIO_MODE_OUTPUT, OMV_GPIO_PULL_NONE, OMV_GPIO_SPEED_LOW, -1);
    omv_gpio_write(OMV_SPI_DISPLAY_RS_PIN, 1);

    // Reset LCD
    omv_gpio_write(OMV_SPI_DISPLAY_RST_PIN, 0);
    mp_hal_delay_ms(100);
    omv_gpio_write(OMV_SPI_DISPLAY_RST_PIN, 1);
    mp_hal_delay_ms(100);

    // Sleep out
    spi_display_command(self, LCD_COMMAND_SLPOUT, 0);
    mp_hal_delay_ms(120);
    // Memory data access control
    spi_display_command(self, LCD_COMMAND_MADCTL, self->bgr ? 0xC8 : 0xC0);
    // Interface pixel format
    spi_display_command(self, LCD_COMMAND_COLMOD, 0x05);

    if (self->triple_buffer) {
        fb_alloc_mark();
        uint32_t fb_size = self->width * self->height * sizeof(uint16_t);
        for (int i = 0; i < FRAMEBUFFER_COUNT; i++) {
            self->framebuffers[i] = (uint16_t *) fb_alloc0(fb_size, FB_ALLOC_CACHE_ALIGN);
        }
        fb_alloc_mark_permanent();
    }

    #ifdef OMV_SPI_DISPLAY_BL_PIN
    spi_display_set_backlight(self, 255);
    #endif

    return MP_OBJ_FROM_PTR(self);
}

STATIC const py_display_p_t py_display_p = {
    .deinit = spi_display_deinit,
    .clear = spi_display_clear,
    .write = spi_display_write,
    #ifdef OMV_SPI_DISPLAY_BL_PIN
    .set_backlight = spi_display_set_backlight,
    #endif
};

MP_DEFINE_CONST_OBJ_TYPE(
    py_spi_display_type,
    MP_QSTR_SPIDisplay,
    MP_TYPE_FLAG_NONE,
    make_new, spi_display_make_new,
    protocol, &py_display_p,
    locals_dict, &py_display_locals_dict
    );

#endif // MICROPY_PY_DISPLAY
