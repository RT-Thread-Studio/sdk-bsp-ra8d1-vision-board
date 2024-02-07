#include <rtthread.h>

#ifdef RW007_USING_STM32_DRIVERS
#include <rtdevice.h>
#include <drv_spi.h>
#include <board.h>
#include <spi_wifi_rw007.h>

extern void spi_wifi_isr(int vector);

#ifdef RW007_USING_POWERSWITCH_EXAMPLE
#define RW007_PWR_PIN   GET_PIN(D, 13)

static void _wifi_pwr_on(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // step 1: resume SPI pin
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);

    // step 2: open power
    rt_pin_write(RW007_PWR_PIN, PIN_HIGH);

    // step 3: rest timing
    rt_pin_write(RW007_RST_PIN, PIN_LOW);
    rt_thread_delay(rt_tick_from_millisecond(100));
    rt_pin_write(RW007_RST_PIN, PIN_HIGH);

    /* Wait rw007 ready(exit busy stat) */
    while(!rt_pin_read(RW007_INT_BUSY_PIN))
    {
        rt_thread_delay(5);
    }
    // step 4: wait reset random pin state pass, enable interrupt
    rt_thread_delay(rt_tick_from_millisecond(200));
    rt_pin_mode(RW007_INT_BUSY_PIN, PIN_MODE_INPUT_PULLUP);
    __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_4); // clean PD4 INT pin flag
    rt_pin_irq_enable(RW007_INT_BUSY_PIN, PIN_IRQ_ENABLE);
}

static void _wifi_pwr_down(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // step 1: disable interrupt, close power
    rt_pin_irq_enable(RW007_INT_BUSY_PIN, PIN_IRQ_DISABLE);
    rt_pin_write(RW007_RST_PIN, PIN_LOW);
    rt_pin_write(RW007_PWR_PIN, PIN_LOW);

    rt_pin_mode(RW007_INT_BUSY_PIN, PIN_MODE_INPUT_PULLDOWN);

    /*
    * step 2: set spi: mosi miso clk csn pins avoid leak
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);
}

void wifi_powerswitch(void)
{
    static int onoff = 0;

    rt_kprintf("wifi %s\n", onoff ? "power on" : "power down");

    if (onoff)
    {
        rw007_powerswitch_request(rw007_power_switch_on);
        // rt_thread_delay(rt_tick_from_millisecond(650));
        rw007_wifi_state_reset();
    }
    else
    {
        rw007_powerswitch_request(rw007_power_switch_off);
    }

    onoff = !onoff;
}
MSH_CMD_EXPORT(wifi_powerswitch, "wifi power switch");
#endif /*RW007_USING_POWERSWITCH_EXAMPLE*/

static void rw007_gpio_init(void)
{
    // /* Configure IO */
    rt_pin_mode(RW007_RST_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(RW007_INT_BUSY_PIN, PIN_MODE_INPUT_PULLDOWN);

    /* Reset rw007 and config mode */
    rt_pin_write(RW007_RST_PIN, PIN_LOW);

#ifdef RW007_USING_POWERSWITCH_EXAMPLE
    rt_pin_mode(RW007_PWR_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(RW007_PWR_PIN, PIN_HIGH);
#endif // RW007_USING_POWERSWITCH_EXAMPLE

    rt_thread_delay(rt_tick_from_millisecond(100));
    rt_pin_write(RW007_RST_PIN, PIN_HIGH);

    /* Wait rw007 ready(exit busy stat) */
    while(!rt_pin_read(RW007_INT_BUSY_PIN))
    {
        rt_thread_delay(5);
    }

    rt_thread_delay(rt_tick_from_millisecond(200));
    rt_pin_mode(RW007_INT_BUSY_PIN, PIN_MODE_INPUT_PULLUP);

#ifdef RW007_USING_POWERSWITCH_EXAMPLE
    rw007_register_powerswitch_cb(_wifi_pwr_down, _wifi_pwr_on);
#endif // RW007_USING_POWERSWITCH_EXAMPLE
    
}

int wifi_spi_device_init(void)
{
    char sn_version[32];

    rw007_gpio_init();

#if (RT_VER_NUM >= 0x50000)
    rt_hw_spi_device_attach(RW007_SPI_BUS_NAME, "wspi", RW007_CS_PIN);
#else
    GPIO_TypeDef *cs_gpiox;
    uint16_t cs_pin;
    cs_gpiox = (GPIO_TypeDef *)((rt_base_t)GPIOA + (rt_base_t)(RW007_CS_PIN / 16) * 0x0400UL);
    cs_pin = (uint16_t)(1 << RW007_CS_PIN % 16);
    rt_hw_spi_device_attach(RW007_SPI_BUS_NAME, "wspi", cs_gpiox, cs_pin);
#endif

    rt_hw_wifi_init("wspi");

    rt_wlan_set_mode(RT_WLAN_DEVICE_STA_NAME, RT_WLAN_STATION);
    rt_wlan_set_mode(RT_WLAN_DEVICE_AP_NAME, RT_WLAN_AP);

    rw007_sn_get(sn_version);
    rt_kprintf("\nrw007  sn: [%s]\n", sn_version);
    rw007_version_get(sn_version);
    rt_kprintf("rw007 ver: [%s]\n\n", sn_version);

    return 0;
}
INIT_APP_EXPORT(wifi_spi_device_init);


static void int_wifi_irq(void * p)
{
    ((void)p);
    spi_wifi_isr(0);
}

void spi_wifi_hw_init(void)
{
    rt_pin_attach_irq(RW007_INT_BUSY_PIN, PIN_IRQ_MODE_FALLING, int_wifi_irq, 0);
    rt_pin_irq_enable(RW007_INT_BUSY_PIN, RT_TRUE);
}

static void wifi_module_ota_enable(int argc, char *args[])
{
    enum rw007_ota_enable_mode enable;
    enum rw007_ota_upgrade_mode upgrade_mode;

    if (argc < 2)
    {
        rt_kprintf("ota_enable 1/0 1/0\n");
        return;
    }

    if (argc >= 2)
    {
        int enable_i = atoi(args[1]);

        enable = ((enable_i != rw007_ota_enable) ? rw007_ota_disable : rw007_ota_enable);
    }
    if (argc >= 3)
    {
        int upgrade_mode_i = atoi(args[2]);

        upgrade_mode = ((upgrade_mode_i != rw007_ota_upgrade_immediate) ? rw007_ota_upgrade_manual : rw007_ota_upgrade_immediate);
    }

    rw007_cfg_ota(enable, upgrade_mode);
}
MSH_CMD_EXPORT_ALIAS(wifi_module_ota_enable, ota_enable, enable WIFI module ota);

#endif /* RW007_USING_STM32_DRIVERS */
