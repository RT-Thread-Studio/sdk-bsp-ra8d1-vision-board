/*
 * This file is part of the OpenMV project.
 *
 * Copyright (c) 2013-2021 Ibrahim Abdelkader <iabdalkader@openmv.io>
 * Copyright (c) 2013-2021 Kwabena W. Agyeman <kwagyeman@openmv.io>
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * Sensor abstraction layer.
 */
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "py/mphal.h"
#include "irq.h"
#include "cambus.h"
#include "sensor.h"
#include "framebuffer.h"
#include "omv_boardconfig.h"
#include "unaligned_memcpy.h"

#define MDMA_BUFFER_SIZE        (64)
#define DMA_MAX_XFER_SIZE       (0xFFFF*4)
#define DMA_MAX_XFER_SIZE_DBL   ((DMA_MAX_XFER_SIZE)*2)
#define DMA_LENGTH_ALIGNMENT    (16)
#define SENSOR_TIMEOUT_MS       (3000)
#define ARRAY_SIZE(a)           (sizeof(a) / sizeof((a)[0]))

sensor_t sensor = {};
static TIM_HandleTypeDef  TIMHandle  = {.Instance = DCMI_TIM};
static DMA_HandleTypeDef  DMAHandle  = {.Instance = DMA2_Stream1};
static DCMI_HandleTypeDef DCMIHandle = {.Instance = DCMI};
#if (OMV_ENABLE_SENSOR_MDMA == 1)
static MDMA_HandleTypeDef DCMI_MDMA_Handle0 = {.Instance = MDMA_Channel0};
static MDMA_HandleTypeDef DCMI_MDMA_Handle1 = {.Instance = MDMA_Channel1};
#endif
// SPI on image sensor connector.
#ifdef ISC_SPI
SPI_HandleTypeDef ISC_SPIHandle = {.Instance = ISC_SPI};
#endif // ISC_SPI

static bool first_line = false;
static bool drop_frame = false;

extern uint8_t _line_buf;

void DCMI_IRQHandler(void) {
    HAL_DCMI_IRQHandler(&DCMIHandle);
}

void DMA2_Stream1_IRQHandler(void) {
    HAL_DMA_IRQHandler(DCMIHandle.DMA_Handle);
}

#ifdef ISC_SPI
void ISC_SPI_IRQHandler(void)
{
    HAL_SPI_IRQHandler(&ISC_SPIHandle);
}

void ISC_SPI_DMA_IRQHandler(void)
{
    HAL_DMA_IRQHandler(ISC_SPIHandle.hdmarx);
}
#endif // ISC_SPI

static int sensor_dma_config()
{
    // DMA Stream configuration
    #if defined(MCU_SERIES_H7)
    DMAHandle.Init.Request              = DMA_REQUEST_DCMI;         /* DMA Channel                      */
    #else
    DMAHandle.Init.Channel              = DMA_CHANNEL_1;            /* DMA Channel                      */
    #endif
    DMAHandle.Init.Direction            = DMA_PERIPH_TO_MEMORY;     /* Peripheral to memory transfer    */
    DMAHandle.Init.MemInc               = DMA_MINC_ENABLE;          /* Memory increment mode Enable     */
    DMAHandle.Init.PeriphInc            = DMA_PINC_DISABLE;         /* Peripheral increment mode Enable */
    DMAHandle.Init.PeriphDataAlignment  = DMA_PDATAALIGN_WORD;      /* Peripheral data alignment : Word */
    DMAHandle.Init.MemDataAlignment     = DMA_MDATAALIGN_WORD;      /* Memory data alignment : Word     */
    DMAHandle.Init.Mode                 = DMA_NORMAL;               /* Normal DMA mode                  */
    DMAHandle.Init.Priority             = DMA_PRIORITY_HIGH;        /* Priority level : high            */
    DMAHandle.Init.FIFOMode             = DMA_FIFOMODE_ENABLE;      /* FIFO mode enabled                */
    DMAHandle.Init.FIFOThreshold        = DMA_FIFO_THRESHOLD_FULL;  /* FIFO threshold full              */
    DMAHandle.Init.MemBurst             = DMA_MBURST_INC4;          /* Memory burst                     */
    DMAHandle.Init.PeriphBurst          = DMA_PBURST_SINGLE;        /* Peripheral burst                 */

    // Initialize the DMA stream
    HAL_DMA_DeInit(&DMAHandle);
    if (HAL_DMA_Init(&DMAHandle) != HAL_OK) {
        // Initialization Error
        return -1;
    }

    // Configure and enable DMA IRQ Channel
    NVIC_SetPriority(DMA2_Stream1_IRQn, IRQ_PRI_DMA21);
    HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
    return 0;
}

void sensor_init0()
{
    sensor_abort();

    // Re-init cambus to reset the bus state after soft reset, which
    // could have interrupted the bus in the middle of a transfer.
    if (sensor.bus.initialized) {
        // Reinitialize the bus using the last used id and speed.
        cambus_init(&sensor.bus, sensor.bus.id, sensor.bus.speed);
    }

    // Disable VSYNC IRQ and callback
    sensor_set_vsync_callback(NULL);

    // Disable Frame callback.
    sensor_set_frame_callback(NULL);
}

int sensor_init()
{
    int init_ret = 0;

    // List of cambus I2C buses to scan.
    uint32_t buses[][2] = {
        {ISC_I2C_ID, ISC_I2C_SPEED},
        #if defined(ISC_I2C_ALT)
        {ISC_I2C_ALT_ID, ISC_I2C_ALT_SPEED},
        #endif
    };

    // Reset the sesnor state
    memset(&sensor, 0, sizeof(sensor_t));

    // Set default snapshot function.
    // Some sensors need to call snapshot from init.
    sensor.snapshot = sensor_snapshot;

    // Configure the sensor external clock (XCLK).
    if (sensor_set_xclk_frequency(OMV_XCLK_FREQUENCY) != 0) {
        // Failed to initialize the sensor clock.
        return SENSOR_ERROR_TIM_INIT_FAILED;
    }

    // Detect and initialize the image sensor.
    for (uint32_t i=0, n_buses = ARRAY_SIZE(buses); i < n_buses; i++) {
        uint32_t id = buses[i][0], speed = buses[i][1];
        if ((init_ret = sensor_probe_init(id, speed)) == 0) {
            // Sensor was detected on the current bus.
            break;
        }
        cambus_deinit(&sensor.bus);
        // Scan the next bus or fail if this is the last one.
        if ((i+1) == n_buses) {
            // Sensor probe/init failed.
            return init_ret;
        }
    }

    // Configure the DCMI DMA Stream
    if (sensor_dma_config() != 0) {
        // DMA problem
        return SENSOR_ERROR_DMA_INIT_FAILED;
    }

    // Configure the DCMI interface.
    if (sensor_dcmi_config(PIXFORMAT_INVALID) != 0){
        // DCMI config failed
        return SENSOR_ERROR_DCMI_INIT_FAILED;
    }

    // Clear fb_enabled flag
    // This is executed only once to initialize the FB enabled flag.
    JPEG_FB()->enabled = 0;

    // Set default color palette.
    sensor.color_palette = rainbow_table;

    sensor.detected = true;

    /* All good! */
    return 0;
}

int sensor_dcmi_config(uint32_t pixformat)
{
    // VSYNC clock polarity
    DCMIHandle.Init.VSPolarity  = sensor.hw_flags.vsync ? DCMI_VSPOLARITY_HIGH : DCMI_VSPOLARITY_LOW;
    // HSYNC clock polarity
    DCMIHandle.Init.HSPolarity  = sensor.hw_flags.hsync ? DCMI_HSPOLARITY_HIGH : DCMI_HSPOLARITY_LOW;
    // PXCLK clock polarity
    DCMIHandle.Init.PCKPolarity = sensor.hw_flags.pixck ? DCMI_PCKPOLARITY_RISING : DCMI_PCKPOLARITY_FALLING;

    // Setup capture parameters.
    DCMIHandle.Init.SynchroMode = DCMI_SYNCHRO_HARDWARE;    // Enable Hardware synchronization
    DCMIHandle.Init.CaptureRate = DCMI_CR_ALL_FRAME;        // Capture rate all frames
    DCMIHandle.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B; // Capture 8 bits on every pixel clock
    // Set JPEG Mode
    DCMIHandle.Init.JPEGMode = (pixformat == PIXFORMAT_JPEG) ?
                                    DCMI_JPEG_ENABLE : DCMI_JPEG_DISABLE;
    #if defined(MCU_SERIES_F7) || defined(MCU_SERIES_H7)
    DCMIHandle.Init.ByteSelectMode  = DCMI_BSM_ALL;         // Capture all received bytes
    DCMIHandle.Init.ByteSelectStart = DCMI_OEBS_ODD;        // Ignored
    DCMIHandle.Init.LineSelectMode  = DCMI_LSM_ALL;         // Capture all received lines
    DCMIHandle.Init.LineSelectStart = DCMI_OELS_ODD;        // Ignored
    #endif

    // Associate the DMA handle to the DCMI handle
    __HAL_LINKDMA(&DCMIHandle, DMA_Handle, DMAHandle);

    // Initialize the DCMI
    HAL_DCMI_DeInit(&DCMIHandle);
    if (HAL_DCMI_Init(&DCMIHandle) != HAL_OK) {
        // Initialization Error
        return -1;
    }

    // Configure and enable DCMI IRQ Channel
    NVIC_SetPriority(DCMI_IRQn, IRQ_PRI_DCMI);
    HAL_NVIC_EnableIRQ(DCMI_IRQn);
    return 0;
}

int sensor_abort()
{
    // This stops the DCMI hardware from generating DMA requests immediately and then stops the DMA
    // hardware. Note that HAL_DMA_Abort is a blocking operation. Do not use this in an interrupt.
    if (DCMI->CR & DCMI_CR_ENABLE) {
        DCMI->CR &= ~DCMI_CR_ENABLE;
        HAL_DMA_Abort(&DMAHandle);
        HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
        #if (OMV_ENABLE_SENSOR_MDMA == 1)
        HAL_MDMA_Abort(&DCMI_MDMA_Handle0);
        HAL_MDMA_Abort(&DCMI_MDMA_Handle1);
        HAL_MDMA_DeInit(&DCMI_MDMA_Handle0);
        HAL_MDMA_DeInit(&DCMI_MDMA_Handle1);
        #endif
        __HAL_DCMI_DISABLE_IT(&DCMIHandle, DCMI_IT_FRAME);
        __HAL_DCMI_CLEAR_FLAG(&DCMIHandle, DCMI_FLAG_FRAMERI);
        first_line = false;
        drop_frame = false;
        sensor.last_frame_ms = 0;
        sensor.last_frame_ms_valid = false;
    }

    framebuffer_reset_buffers();

    return 0;
}

uint32_t sensor_get_xclk_frequency()
{
    return (DCMI_TIM_PCLK_FREQ() * 2) / (TIMHandle.Init.Period + 1);
}

int sensor_set_xclk_frequency(uint32_t frequency)
{
    #if (OMV_XCLK_SOURCE == OMV_XCLK_TIM)
    /* TCLK (PCLK * 2) */
    int tclk = DCMI_TIM_PCLK_FREQ() * 2;

    /* Period should be even */
    int period = (tclk / frequency) - 1;
    int pulse = period / 2;

    if (TIMHandle.Init.Period && (TIMHandle.Init.Period != period)) {
        // __HAL_TIM_SET_AUTORELOAD sets TIMHandle.Init.Period...
        __HAL_TIM_SET_AUTORELOAD(&TIMHandle, period);
        __HAL_TIM_SET_COMPARE(&TIMHandle, DCMI_TIM_CHANNEL, pulse);
        return 0;
    }

    /* Timer base configuration */
    TIMHandle.Init.Period            = period;
    TIMHandle.Init.Prescaler         = 0;
    TIMHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    TIMHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    TIMHandle.Init.RepetitionCounter = 0;
    TIMHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    /* Timer channel configuration */
    TIM_OC_InitTypeDef TIMOCHandle;
    TIMOCHandle.Pulse           = pulse;
    TIMOCHandle.OCMode          = TIM_OCMODE_PWM1;
    TIMOCHandle.OCPolarity      = TIM_OCPOLARITY_HIGH;
    TIMOCHandle.OCNPolarity     = TIM_OCNPOLARITY_HIGH;
    TIMOCHandle.OCFastMode      = TIM_OCFAST_DISABLE;
    TIMOCHandle.OCIdleState     = TIM_OCIDLESTATE_RESET;
    TIMOCHandle.OCNIdleState    = TIM_OCNIDLESTATE_RESET;

    if ((HAL_TIM_PWM_Init(&TIMHandle) != HAL_OK)
    || (HAL_TIM_PWM_ConfigChannel(&TIMHandle, &TIMOCHandle, DCMI_TIM_CHANNEL) != HAL_OK)
    || (HAL_TIM_PWM_Start(&TIMHandle, DCMI_TIM_CHANNEL) != HAL_OK)) {
        return -1;
    }
    #elif (OMV_XCLK_SOURCE == OMV_XCLK_MCO)
    // Pass through the MCO1 clock with source input set to HSE (12MHz).
    // Note MCO1 is multiplexed on OPENMV2/TIM1 only.
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
    #elif (OMV_XCLK_SOURCE == OMV_XCLK_OSC)
    // An external oscillator is used for the sensor clock.
    // Configure and enable external oscillator if needed.
    #else
    #error "OMV_XCLK_SOURCE is not set!"
    #endif // (OMV_XCLK_SOURCE == OMV_XCLK_TIM)
    return 0;
}

int sensor_shutdown(int enable)
{
    int ret = 0;
    sensor_abort();

    if (enable) {
        if (sensor.pwdn_pol == ACTIVE_HIGH) {
            DCMI_PWDN_HIGH();
        } else {
            DCMI_PWDN_LOW();
        }
        HAL_NVIC_DisableIRQ(DCMI_IRQn);
        HAL_DCMI_DeInit(&DCMIHandle);
    } else {
        if (sensor.pwdn_pol == ACTIVE_HIGH) {
            DCMI_PWDN_LOW();
        } else {
            DCMI_PWDN_HIGH();
        }
        ret = sensor_dcmi_config(sensor.pixformat);
    }

    mp_hal_delay_ms(10);
    return ret;
}

int sensor_set_vsync_callback(vsync_cb_t vsync_cb)
{
    sensor.vsync_callback = vsync_cb;
    if (sensor.vsync_callback == NULL) {
        // Disable VSYNC EXTI IRQ
        HAL_NVIC_DisableIRQ(DCMI_VSYNC_IRQN);
    } else {
        // Enable VSYNC EXTI IRQ
        NVIC_SetPriority(DCMI_VSYNC_IRQN, IRQ_PRI_EXTINT);
        HAL_NVIC_EnableIRQ(DCMI_VSYNC_IRQN);
    }
    return 0;
}

void DCMI_VsyncExtiCallback()
{
    __HAL_GPIO_EXTI_CLEAR_FLAG(1 << DCMI_VSYNC_IRQ_LINE);
    if (sensor.vsync_callback != NULL) {
        sensor.vsync_callback(HAL_GPIO_ReadPin(DCMI_VSYNC_PORT, DCMI_VSYNC_PIN));
    }
}

// If we are cropping the image by more than 1 word in width we can align the line start to
// a word address to improve copy performance. Do not crop by more than 1 word as this will
// result in less time between DMA transfers complete interrupts on 16-byte boundaries.
static uint32_t get_dcmi_hw_crop(uint32_t bytes_per_pixel)
{
    uint32_t byte_x_offset = (MAIN_FB()->x * bytes_per_pixel) % sizeof(uint32_t);
    uint32_t width_remainder = (resolution[sensor.framesize][0] - (MAIN_FB()->x + MAIN_FB()->u)) * bytes_per_pixel;
    uint32_t x_crop = 0;

    if (byte_x_offset && (width_remainder >= (sizeof(uint32_t) - byte_x_offset))) {
        x_crop = byte_x_offset;
    }

    return x_crop;
}

// Stop allowing new data in on the end of the frame and let snapshot know that the frame has been
// received. Note that DCMI_DMAConvCpltUser() is called before DCMI_IT_FRAME is enabled by
// DCMI_DMAXferCplt() so this means that the last line of data is *always* transferred before
// moving the tail to the next buffer.
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
    // This can be executed at any time since this interrupt has a higher priority than DMA2_Stream1_IRQn.
    #if (OMV_ENABLE_SENSOR_MDMA == 1)
    // Clear out any stale flags.
    DMA2->LIFCR = DMA_FLAG_TCIF1_5 | DMA_FLAG_HTIF1_5;
    // Re-enable the DMA IRQ to catch the next start line.
    HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
    #endif

    // Reset DCMI_DMAConvCpltUser frame drop state.
    first_line = false;
    if (drop_frame) {
        drop_frame = false;
        return;
    }

    framebuffer_get_tail(FB_NO_FLAGS);

    if (sensor.frame_callback) {
        sensor.frame_callback();
    }
}

#if (OMV_ENABLE_SENSOR_MDMA == 1)
static void mdma_memcpy(vbuffer_t *buffer, void *dst, void *src, int bpp, bool transposed)
{
    // We're using two handles to give each channel the maximum amount of time possible to do the line
    // transfer. In most situations only one channel will be running at a time. However, if SDRAM is
    // backedup we don't have to disable the channel if it is flushing trailing data to SDRAM.
    MDMA_HandleTypeDef *handle = (buffer->offset % 2) ? &DCMI_MDMA_Handle1 : &DCMI_MDMA_Handle0;

    // If MDMA is still running from a previous transfer HAL_MDMA_Start() will disable that transfer
    // and start a new transfer.
    __HAL_UNLOCK(handle);
    handle->State = HAL_MDMA_STATE_READY;
    HAL_MDMA_Start(handle,
                   (uint32_t) src,
                   (uint32_t) dst,
                   transposed ? bpp : (MAIN_FB()->u * bpp),
                   transposed ? MAIN_FB()->u : 1);
}
#endif

// This function is called back after each line transfer is complete,
// with a pointer to the line buffer that was used. At this point the
// DMA transfers the next line to the other half of the line buffer.
void DCMI_DMAConvCpltUser(uint32_t addr)
{
    if (!first_line) {
        first_line = true;
        uint32_t tick = HAL_GetTick();
        uint32_t framerate_ms = IM_DIV(1000, sensor.framerate);

        // Drops frames to match the frame rate requested by the user. The frame is NOT copied to
        // SRAM/SDRAM when dropping to save CPU cycles/energy that would be wasted.
        // If framerate is zero then this does nothing...
        if (sensor.last_frame_ms_valid && ((tick - sensor.last_frame_ms) < framerate_ms)) {
            drop_frame = true;
        } else if (sensor.last_frame_ms_valid) {
            sensor.last_frame_ms += framerate_ms;
        } else {
            sensor.last_frame_ms = tick;
            sensor.last_frame_ms_valid = true;
        }
    }

    if (drop_frame) {
        // If we're dropping a frame in full offload mode it's safe to disable this interrupt saving
        // ourselves from having to service the DMA complete callback.
        #if (OMV_ENABLE_SENSOR_MDMA == 1)
        if (!sensor.transpose) {
            HAL_NVIC_DisableIRQ(DMA2_Stream1_IRQn);
        }
        #endif
        return;
    }

    vbuffer_t *buffer = framebuffer_get_tail(FB_PEEK);

    // If snapshot was not already waiting to receive data then we have missed this frame and have
    // to drop it. So, abort this and future transfers. Snapshot will restart the process.
    if (!buffer) {
        DCMI->CR &= ~DCMI_CR_ENABLE;
        HAL_DMA_Abort_IT(&DMAHandle); // Note: Use HAL_DMA_Abort_IT and not HAL_DMA_Abort inside an interrupt.
        #if (OMV_ENABLE_SENSOR_MDMA == 1)
        HAL_MDMA_DeInit(&DCMI_MDMA_Handle0);
        HAL_MDMA_DeInit(&DCMI_MDMA_Handle1);
        #endif
        __HAL_DCMI_DISABLE_IT(&DCMIHandle, DCMI_IT_FRAME);
        __HAL_DCMI_CLEAR_FLAG(&DCMIHandle, DCMI_FLAG_FRAMERI);
        first_line = false;
        drop_frame = false;
        sensor.last_frame_ms = 0;
        sensor.last_frame_ms_valid = false;
        // Reset the queue of frames when we start dropping frames.
        if (!sensor.disable_full_flush) {
            framebuffer_flush_buffers();
        }
        return;
    }

    // We are transferring the image from the DCMI hardware to line buffers so that we have more
    // control to post process the image data before writing it to the frame buffer. This requires
    // more CPU, but, allows us to crop and rotate the image as the data is received.

    // Additionally, the line buffers act as very large fifos which hide SDRAM memory access times
    // on the OpenMV Cam H7 Plus. When SDRAM refreshes the row you are trying to write to the fifo
    // depth on the DCMI hardware and DMA hardware is not enough to prevent data loss.

    if (sensor.pixformat == PIXFORMAT_JPEG) {
        if (sensor.hw_flags.jpeg_mode == 4) {
            // JPEG MODE 4:
            //
            // The width and height are fixed in each frame. The first two bytes are valid data
            // length in every line, followed by valid image data. Dummy data (0xFF) may be used as
            // padding at each line end if the current valid image data is less than the line width.
            //
            // In this mode `offset` holds the size of all jpeg data transferred.
            //
            // Note: We are using this mode for the OV5640 because it allows us to use the line
            // buffers to fifo the JPEG image data input so we can handle SDRAM refresh hiccups
            // that will cause data loss if we make the DMA hardware write directly to the FB.
            //
            uint16_t size = __REV16(*((uint16_t *) addr));
            // Prevent a buffer overflow when writing the jpeg data.
            if (buffer->offset + size > framebuffer_get_buffer_size()) {
                buffer->jpeg_buffer_overflow = true;
                return;
            }
            unaligned_memcpy(buffer->data + buffer->offset, ((uint16_t *) addr) + 1, size);
            buffer->offset += size;
       } else if (sensor.hw_flags.jpeg_mode == 3) {
            // JPEG MODE 3:
            //
            // Compression data is transmitted with programmable width. The last line width maybe
            // different from the other line (there is no dummy data). In each frame, the line
            // number may be different.
            //
            // In this mode `offset` will be incremented by one after 262,140 Bytes have been
            // transferred. If 524,280 Bytes have been transferred line will be incremented again.
            // The DMA counter must be used to get the amount of data transferred between.
            //
            // Note: In this mode the JPEG image data is written directly to the frame buffer. This
            // is not optimal. However, it works okay for the OV2640 since the PCLK is much lower
            // than the OV5640 PCLK. The OV5640 drops data in this mode. Hence using mode 4 above.
            //
            buffer->offset += 1;
        }
        return;
    }

    // DCMI_DMAXferCplt in the HAL DCMI driver always calls DCMI_DMAConvCpltUser with the other
    // MAR register. So, we have to fix the address in full MDMA offload mode...
    #if (OMV_ENABLE_SENSOR_MDMA == 1)
    if (!sensor.transpose) {
        addr = (uint32_t) &_line_buf;
    }
    #endif

    uint32_t bytes_per_pixel = sensor_get_src_bpp();
    uint8_t *src = ((uint8_t *) addr) + (MAIN_FB()->x * bytes_per_pixel) - get_dcmi_hw_crop(bytes_per_pixel);
    uint8_t *dst = buffer->data;

    if (sensor.pixformat == PIXFORMAT_GRAYSCALE) {
        bytes_per_pixel = sizeof(uint8_t);
    }

    // For all non-JPEG and non-transposed modes we can completely offload image catpure to MDMA
    // and we do not need to receive any line interrupts for the rest of the frame until it ends.
    #if (OMV_ENABLE_SENSOR_MDMA == 1)
    if (!sensor.transpose) {
        // NOTE: We're starting MDMA here because it gives the maximum amount of time before we
        // have to drop the frame if there's no space. If you use the FRAME/VSYNC callbacks then
        // you will have to drop the frame earlier than necessary if there's no space resulting
        // in the apparent unloaded FPS being lower than this method gives you.
        uint32_t line_width_bytes = MAIN_FB()->u * bytes_per_pixel;
        // DMA0 will copy this line of the image to the final destination.
        __HAL_UNLOCK(&DCMI_MDMA_Handle0);
        DCMI_MDMA_Handle0.State = HAL_MDMA_STATE_READY;
        HAL_MDMA_Start(&DCMI_MDMA_Handle0, (uint32_t) src, (uint32_t) dst,
                       line_width_bytes, 1);
        // DMA1 will copy all remaining lines of the image to the final destination.
        __HAL_UNLOCK(&DCMI_MDMA_Handle1);
        DCMI_MDMA_Handle1.State = HAL_MDMA_STATE_READY;
        HAL_MDMA_Start(&DCMI_MDMA_Handle1, (uint32_t) src, (uint32_t) (dst + line_width_bytes),
                       line_width_bytes, MAIN_FB()->v - 1);
        HAL_NVIC_DisableIRQ(DMA2_Stream1_IRQn);
        return;
    }
    #endif

    if (!sensor.transpose) {
        dst += MAIN_FB()->u * bytes_per_pixel * buffer->offset++;
    } else {
        dst += bytes_per_pixel * buffer->offset++;
    }

    // Implement per line, per pixel cropping, and image transposing (for image rotation) in
    // in software using the CPU to transfer the image from the line buffers to the frame buffer.
    uint16_t *src16 = (uint16_t *) src;
    uint16_t *dst16 = (uint16_t *) dst;

    switch (sensor.pixformat) {
        case PIXFORMAT_BAYER:
            #if (OMV_ENABLE_SENSOR_MDMA == 1)
            mdma_memcpy(buffer, dst, src, sizeof(uint8_t), sensor.transpose);
            #else
            if (!sensor.transpose) {
                unaligned_memcpy(dst, src, MAIN_FB()->u);
            } else {
                for (int i = MAIN_FB()->u, h = MAIN_FB()->v; i; i--) {
                    *dst = *src++;
                    dst += h;
                }
            }
            #endif
            break;
        case PIXFORMAT_GRAYSCALE:
            #if (OMV_ENABLE_SENSOR_MDMA == 1)
            mdma_memcpy(buffer, dst, src, sizeof(uint8_t), sensor.transpose);
            #else
            if (sensor.hw_flags.gs_bpp == 1) {
                // 1BPP GRAYSCALE.
                if (!sensor.transpose) {
                    unaligned_memcpy(dst, src, MAIN_FB()->u);
                } else {
                    for (int i = MAIN_FB()->u, h = MAIN_FB()->v; i; i--) {
                        *dst = *src++;
                        dst += h;
                    }
                }
            } else {
                // Extract Y channel from YUV.
                if (!sensor.transpose) {
                    unaligned_2_to_1_memcpy(dst, src16, MAIN_FB()->u);
                } else {
                    for (int i = MAIN_FB()->u, h = MAIN_FB()->v; i; i--) {
                        *dst = *src16++;
                        dst += h;
                    }
                }
            }
            #endif
            break;
        case PIXFORMAT_RGB565:
        case PIXFORMAT_YUV422:
            #if (OMV_ENABLE_SENSOR_MDMA == 1)
            mdma_memcpy(buffer, dst16, src16, sizeof(uint16_t), sensor.transpose);
            #else
            if ((sensor.pixformat == PIXFORMAT_RGB565 && sensor.hw_flags.rgb_swap)
            ||  (sensor.pixformat == PIXFORMAT_YUV422 && sensor.hw_flags.yuv_swap)) {
                if (!sensor.transpose) {
                    unaligned_memcpy_rev16(dst16, src16, MAIN_FB()->u);
                } else {
                    for (int i = MAIN_FB()->u, h = MAIN_FB()->v; i; i--) {
                        *dst16 = __REV16(*src16++);
                        dst16 += h;
                    }
                }
            } else {
                if (!sensor.transpose) {
                    unaligned_memcpy(dst16, src16, MAIN_FB()->u * sizeof(uint16_t));
                } else {
                    for (int i = MAIN_FB()->u, h = MAIN_FB()->v; i; i--) {
                        *dst16 = *src16++;
                        dst16 += h;
                    }
                }
            }
            #endif
            break;
        default:
            break;
    }
}

#if (OMV_ENABLE_SENSOR_MDMA == 1)
// Configures an MDMA channel to completely offload the CPU in copying one line of pixels.
static void mdma_config(MDMA_InitTypeDef *init, sensor_t *sensor, uint32_t bytes_per_pixel)
{
    init->Request                   = MDMA_REQUEST_SW;
    init->TransferTriggerMode       = MDMA_REPEAT_BLOCK_TRANSFER;
    init->Priority                  = MDMA_PRIORITY_VERY_HIGH;
    init->DataAlignment             = MDMA_DATAALIGN_PACKENABLE;
    init->BufferTransferLength      = MDMA_BUFFER_SIZE;
    // The source address is 1KB aligned. So, a burst size of 16 beats (AHB Max) should not break.
    // Destination lines may not be aligned however so the burst size must be computed.
    init->SourceBurst               = MDMA_SOURCE_BURST_16BEATS;
    init->SourceBlockAddressOffset  = 0;
    init->DestBlockAddressOffset    = 0;

    if ((sensor->pixformat == PIXFORMAT_RGB565 && sensor->hw_flags.rgb_swap)
    ||  (sensor->pixformat == PIXFORMAT_YUV422 && sensor->hw_flags.yuv_swap)) {
        init->Endianness = MDMA_LITTLE_BYTE_ENDIANNESS_EXCHANGE;
    } else {
        init->Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
    }

    uint32_t line_offset_bytes = (MAIN_FB()->x * bytes_per_pixel) - get_dcmi_hw_crop(bytes_per_pixel);
    uint32_t line_width_bytes = MAIN_FB()->u * bytes_per_pixel;

    if (sensor->transpose) {
        line_width_bytes = bytes_per_pixel;
        init->DestBlockAddressOffset = (MAIN_FB()->v - 1) * bytes_per_pixel;
    }

    // YUV422 Source -> Y Destination
    if ((sensor->pixformat == PIXFORMAT_GRAYSCALE) && (sensor->hw_flags.gs_bpp == 2)) {
        line_width_bytes /= 2;
        if (sensor->transpose) {
            init->DestBlockAddressOffset /= 2;
        }
    }

    // Destination will be 32-byte aligned. So, we just need to breakup the line width into the largest
    // power of 2. Source may have an offset which further limits this to a sub power of 2.
    for (int i = 3; i >= 0; i--) {
        if (!(line_width_bytes % (1 << i))) {
            for (int j = IM_MIN(i, 2); j >= 0; j--) {
                if (!(line_offset_bytes % (1 << j))) {
                    init->SourceInc = MDMA_CTCR_SINC_1 | (j << MDMA_CTCR_SINCOS_Pos);
                    init->SourceDataSize = j << MDMA_CTCR_SSIZE_Pos;
                    break;
                }
            }

            init->DestinationInc = MDMA_CTCR_DINC_1 | (i << MDMA_CTCR_DINCOS_Pos);
            init->DestDataSize = i << MDMA_CTCR_DSIZE_Pos;

            // Find the burst size we can break the destination transfer up into.
            uint32_t count = MDMA_BUFFER_SIZE >> i;

            for (int i = 7; i >= 0; i--) {
                if (!(count % (1 << i))) {
                    init->DestBurst = i << MDMA_CTCR_DBURST_Pos;
                    break;
                }
            }

            break;
        }
    }

    // YUV422 Source -> Y Destination
    if ((sensor->pixformat == PIXFORMAT_GRAYSCALE) && (sensor->hw_flags.gs_bpp == 2)) {
        init->SourceInc         = MDMA_SRC_INC_HALFWORD;
        init->SourceDataSize    = MDMA_SRC_DATASIZE_BYTE;
    }
}
#endif

// This is the default snapshot function, which can be replaced in sensor_init functions. This function
// uses the DCMI and DMA to capture frames and each line is processed in the DCMI_DMAConvCpltUser function.
int sensor_snapshot(sensor_t *sensor, image_t *image, uint32_t flags)
{
    uint32_t length = 0;

    // Compress the framebuffer for the IDE preview, only if it's not the first frame,
    // the framebuffer is enabled and the image sensor does not support JPEG encoding.
    // Note: This doesn't run unless the IDE is connected and the framebuffer is enabled.
    framebuffer_update_jpeg_buffer();

    // Make sure the raw frame fits into the FB. It will be switched from RGB565 to BAYER
    // first to save space before being cropped until it fits.
    sensor_auto_crop_framebuffer();

    // The user may have changed the MAIN_FB width or height on the last image so we need
    // to restore that here. We don't have to restore bpp because that's taken care of
    // already in the code below. Note that we do the JPEG compression above first to save
    // the FB of whatever the user set it to and now we restore.
    uint32_t w = MAIN_FB()->u;
    uint32_t h = MAIN_FB()->v;

    // If DCMI_DMAConvCpltUser() happens before framebuffer_free_current_buffer(); below then the
    // transfer is stopped and it will be re-enabled again right afterwards in the single vbuffer
    // case. We know the transfer was stopped by checking DCMI_CR_ENABLE.
    framebuffer_free_current_buffer();

    // We will be in one of the following states now:
    // 1. No transfer is currently running right now and DCMI_CR_ENABLE is not set.
    // 2. A transfer is running and we are waiting for the data to be received.

    // We are not using DCMI_CR_CAPTURE because when this bit is cleared to stop the continuous transfer it does not actually go
    // low until the end of the frame (yes, you read that right). DCMI_CR_ENABLE stops the capture when cleared and stays low.
    //
    // When DCMI_CR_ENABLE is cleared during a DCMI transfer the hardware will automatically
    // wait for the start of the next frame when it's re-enabled again below. So, we do not
    // need to wait till there's no frame happening before enabling.
    if (!(DCMI->CR & DCMI_CR_ENABLE)) {
        // Setup the size and address of the transfer
        uint32_t bytes_per_pixel = sensor_get_src_bpp();

        // Error out if the pixformat is not set.
        if (!bytes_per_pixel) {
            return SENSOR_ERROR_INVALID_PIXFORMAT;
        }

        uint32_t x_crop = get_dcmi_hw_crop(bytes_per_pixel);
        uint32_t dma_line_width_bytes = resolution[sensor->framesize][0] * bytes_per_pixel;

        // Shrink the captured pixel count by one word to allow cropping to fix alignment.
        if (x_crop) {
            dma_line_width_bytes -= sizeof(uint32_t);
        }

        length = dma_line_width_bytes * h;

        // Error out if the transfer size is not compatible with DMA transfer restrictions.
        if ((!dma_line_width_bytes)
        || (dma_line_width_bytes % sizeof(uint32_t))
        || (dma_line_width_bytes > (OMV_LINE_BUF_SIZE / 2))
        || (!length)
        || (length % DMA_LENGTH_ALIGNMENT)) {
            return SENSOR_ERROR_INVALID_FRAMESIZE;
        }

        // Get the destination buffer address.
        vbuffer_t *buffer = framebuffer_get_tail(FB_PEEK);

        if ((sensor->pixformat == PIXFORMAT_JPEG) && (sensor->chip_id == OV2640_ID) && (!buffer)) {
            return SENSOR_ERROR_FRAMEBUFFER_ERROR;
        }

        #if (OMV_ENABLE_SENSOR_MDMA == 1)
        // The code below will enable MDMA data transfer from the DCMI line buffer for non-JPEG modes.
        if (sensor->pixformat != PIXFORMAT_JPEG) {
            mdma_config(&DCMI_MDMA_Handle0.Init, sensor, bytes_per_pixel);
            memcpy(&DCMI_MDMA_Handle1.Init, &DCMI_MDMA_Handle0.Init, sizeof(MDMA_InitTypeDef));
            HAL_MDMA_Init(&DCMI_MDMA_Handle0);

            // If we are not transposing the image we can fully offload image capture from the CPU.
            if (!sensor->transpose) {
                // MDMA will trigger on each TC from DMA and transfer one line to the frame buffer.
                DCMI_MDMA_Handle1.Init.Request = MDMA_REQUEST_DMA2_Stream1_TC;
                DCMI_MDMA_Handle1.Init.TransferTriggerMode = MDMA_BLOCK_TRANSFER;
                // We setup MDMA to repeatedly reset itself to transfer the same line buffer.
                DCMI_MDMA_Handle1.Init.SourceBlockAddressOffset = -(MAIN_FB()->u * bytes_per_pixel);

                HAL_MDMA_Init(&DCMI_MDMA_Handle1);
                HAL_MDMA_ConfigPostRequestMask(&DCMI_MDMA_Handle1, (uint32_t) &DMA2->LIFCR, DMA_FLAG_TCIF1_5);
            } else {
                HAL_MDMA_Init(&DCMI_MDMA_Handle1);
            }
        }
        #endif

        HAL_DCMI_DisableCrop(&DCMIHandle);
        if (sensor->pixformat != PIXFORMAT_JPEG) {
            // Vertically crop the image. Horizontal cropping is done in software.
            HAL_DCMI_ConfigCrop(&DCMIHandle, x_crop, MAIN_FB()->y, dma_line_width_bytes - 1, h - 1);
            HAL_DCMI_EnableCrop(&DCMIHandle);
        }

        // Reset the circular, current target, and double buffer mode flags which get set by the below calls.
        ((DMA_Stream_TypeDef *) DMAHandle.Instance)->CR &= ~(DMA_SxCR_CIRC | DMA_SxCR_CT | DMA_SxCR_DBM);

        // Note that HAL_DCMI_Start_DMA and HAL_DCMI_Start_DMA_MB are effectively the same
        // method. The only difference between them is how large the DMA transfer size gets
        // set at. For both of them DMA doesn't actually care how much data the DCMI hardware
        // generates. It's just trying to move fixed size DMA transfers from the DCMI hardware
        // to one memory address or another memory address. After transferring X bytes to one
        // address it will switch to the next address and transfer X bytes again. Both of these
        // methods set the addresses right after each other. So, effectively DMA is just writing
        // data to a circular buffer with an interrupt every time 1/2 of it is written.
        if ((sensor->pixformat == PIXFORMAT_JPEG) && (sensor->chip_id == OV2640_ID)) {
            // The JPEG image will be directly transferred to the frame buffer.
            // The DCMI hardware can transfer up to 524,280 bytes.
            length = DMA_MAX_XFER_SIZE_DBL;
            uint32_t size = framebuffer_get_buffer_size();
            length = IM_MIN(length, size);

            // Start a transfer where the whole frame buffer is located where the DMA is writing
            // data to. We only use this for JPEG mode for the OV2640. Since we don't know the
            // line size of data being transferred we just examine how much data was transferred
            // once DMA hardware stalls waiting for data. Note that because we are writing
            // directly to the frame buffer we do not have the option of aborting the transfer
            // if we are not ready to move data from a line buffer to the frame buffer.

            // In this mode the DMA hardware is just treating the frame buffer as two large
            // DMA buffers. At the end of the frame less data may be transferred than requested.
            HAL_DCMI_Start_DMA(&DCMIHandle, DCMI_MODE_SNAPSHOT,
                               (uint32_t) buffer->data, length / sizeof(uint32_t));

            // If length is greater than DMA_MAX_XFER_SIZE then HAL_DCMI_Start_DMA splits length
            // into two transfers less than DMA_MAX_XFER_SIZE.
            if (length > DMA_MAX_XFER_SIZE) {
                length /= 2;
            }
        #if (OMV_ENABLE_SENSOR_MDMA == 1)
        // Special transfer mode with MDMA that completely offloads the line capture load.
        } else if ((sensor->pixformat != PIXFORMAT_JPEG) && (!sensor->transpose)) {
            // DMA to circular mode writing the same line over and over again.
            ((DMA_Stream_TypeDef *) DMAHandle.Instance)->CR |= DMA_SxCR_CIRC;
            // DCMI will transfer to same line and MDMA will move to final location.
            HAL_DCMI_Start_DMA(&DCMIHandle, DCMI_MODE_CONTINUOUS,
                               (uint32_t) &_line_buf, dma_line_width_bytes / sizeof(uint32_t));
        #endif
        } else {
            // Start a multibuffer transfer (line by line). The DMA hardware will ping-pong
            // transferring data between the uncached line buffers. Since data is continuously
            // being captured the ping-ponging will stop at the end of the frame and then
            // continue when the next frame starts.
            HAL_DCMI_Start_DMA_MB(&DCMIHandle, DCMI_MODE_CONTINUOUS,
                                  (uint32_t) &_line_buf, length / sizeof(uint32_t), h);
        }
    }

    // Let the camera know we want to trigger it now.
    #if defined(DCMI_FSYNC_PIN)
    if (sensor->hw_flags.fsync) {
        DCMI_FSYNC_HIGH();
    }
    #endif

    // In camera sensor JPEG mode 4 we will not necessarily see every line in the frame and
    // in camera sensor JPEG mode 3 we will definitely not see every line in the frame. Given
    // this, we need to enable the end of frame interrupt before we have necessarily
    // finished transferring all JEPG data. This works as long as the end of the frame comes
    // much later after all JPEG data has been transferred. If this is violated the JPEG image
    // will be corrupted.
    if (DCMI->CR & DCMI_JPEG_ENABLE) {
        __HAL_DCMI_ENABLE_IT(&DCMIHandle, DCMI_IT_FRAME);
    }

    vbuffer_t *buffer = NULL;
    // Wait for the frame data. __WFI() below will exit right on time because of DCMI_IT_FRAME.
    // While waiting SysTick will trigger allowing us to timeout.
    for (uint32_t tick_start = HAL_GetTick(); !(buffer = framebuffer_get_head(FB_NO_FLAGS)); ) {
        __WFI();

        // If we haven't exited this loop before the timeout then we need to abort the transfer.
        if ((HAL_GetTick() - tick_start) > SENSOR_TIMEOUT_MS) {
            sensor_abort();

            #if defined(DCMI_FSYNC_PIN)
            if (sensor->hw_flags.fsync) {
                DCMI_FSYNC_LOW();
            }
            #endif

            return SENSOR_ERROR_CAPTURE_TIMEOUT;
        }
    }

    // We have to abort the JPEG data transfer since it will be stuck waiting for data.
    // line will contain how many transfers we completed.
    // The DMA counter must be used to get the number of remaining words to be transferred.
    if ((sensor->pixformat == PIXFORMAT_JPEG) && (sensor->chip_id == OV2640_ID)) {
        sensor_abort();
    }

    // We're done receiving data.
    #if defined(DCMI_FSYNC_PIN)
    if (sensor->hw_flags.fsync) {
        DCMI_FSYNC_LOW();
    }
    #endif

    // The JPEG in the frame buffer is actually invalid.
    if (buffer->jpeg_buffer_overflow) {
        return SENSOR_ERROR_JPEG_OVERFLOW;
    }

    // Prepare the frame buffer w/h/bpp values given the image type.

    if (!sensor->transpose) {
        MAIN_FB()->w = w;
        MAIN_FB()->h = h;
    } else {
        MAIN_FB()->w = h;
        MAIN_FB()->h = w;
    }

    // Fix the BPP.
    switch (sensor->pixformat) {
        case PIXFORMAT_GRAYSCALE:
            MAIN_FB()->pixfmt = PIXFORMAT_GRAYSCALE;
            break;
        case PIXFORMAT_RGB565:
            MAIN_FB()->pixfmt = PIXFORMAT_RGB565;
            break;
        case PIXFORMAT_BAYER:
            MAIN_FB()->pixfmt    = PIXFORMAT_BAYER;
            MAIN_FB()->subfmt_id = sensor->hw_flags.bayer;
            break;
        case PIXFORMAT_YUV422: {
            bool yuv_order = sensor->hw_flags.yuv_order == SENSOR_HW_FLAGS_YUV422;
            int even = yuv_order ? PIXFORMAT_YUV422 : PIXFORMAT_YVU422;
            int odd = yuv_order ? PIXFORMAT_YVU422 : PIXFORMAT_YUV422;
            MAIN_FB()->pixfmt = (MAIN_FB()->x % 2) ? odd : even;
            break;
        }
        case PIXFORMAT_JPEG: {
            int32_t size = 0;
            if (sensor->chip_id == OV5640_ID) {
                // Offset contains the sum of all the bytes transferred from the offset buffers
                // while in DCMI_DMAConvCpltUser().
                size = buffer->offset;
            } else {
                // Offset contains the number of length transfers completed. To get the number of bytes transferred
                // within a transfer we have to look at the DMA counter and see how much data was moved.
                size = buffer->offset * length;

                if (__HAL_DMA_GET_COUNTER(&DMAHandle)) { // Add in the uncompleted transfer length.
                    size += ((length / sizeof(uint32_t)) - __HAL_DMA_GET_COUNTER(&DMAHandle)) * sizeof(uint32_t);
                }
            }
            // Clean trailing data after 0xFFD9 at the end of the jpeg byte stream.
            MAIN_FB()->pixfmt = PIXFORMAT_JPEG;
            MAIN_FB()->size = jpeg_clean_trailing_bytes(size, buffer->data);
            break;
        }
        default:
            break;
    }

    // Set the user image.
    framebuffer_init_image(image);
    return 0;
}
