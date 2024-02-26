#include <rtthread.h>

#if (defined(BSP_USING_LCD)) || (defined(SOC_SERIES_R7FA8M85))
#include <lcd_port.h>
#include "hal_data.h"

#define DRV_DEBUG
#define LOG_TAG             "mipi_cfg"
#include <drv_log.h>

#define MIPI_DSI_DISPLAY_CONFIG_DATA_DELAY_FLAG      ((mipi_dsi_cmd_id_t) 0xFE)
#define MIPI_DSI_DISPLAY_CONFIG_DATA_END_OF_TABLE    ((mipi_dsi_cmd_id_t) 0xFD)

typedef struct
{
    unsigned char        size;
    unsigned char        buffer[10];
    mipi_dsi_cmd_id_t    cmd_id;
    mipi_dsi_cmd_flag_t flags;
} lcd_table_setting_t;

volatile static bool g_message_sent;
volatile static mipi_dsi_phy_status_t g_phy_status;
        
const lcd_table_setting_t g_lcd_init_focuslcd[] =
{
    {2,  {0xB2, 0x10},                         MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM,    MIPI_DSI_CMD_FLAG_LOW_POWER},

    {2,  {0x80, 0xAC},                         MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM,    MIPI_DSI_CMD_FLAG_LOW_POWER},
    {2,  {0x81, 0xB8},                         MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM,    MIPI_DSI_CMD_FLAG_LOW_POWER},
    {2,  {0x82, 0x09},                         MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM,    MIPI_DSI_CMD_FLAG_LOW_POWER},
    {2,  {0x83, 0x78},                         MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM,    MIPI_DSI_CMD_FLAG_LOW_POWER},
    {2,  {0x84, 0x7F},                         MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM,    MIPI_DSI_CMD_FLAG_LOW_POWER},
    {2,  {0x85, 0xBB},                         MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM,    MIPI_DSI_CMD_FLAG_LOW_POWER},
    {2,  {0x86, 0x70},                         MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM,    MIPI_DSI_CMD_FLAG_LOW_POWER},

    {0x00, {0},                                MIPI_DSI_DISPLAY_CONFIG_DATA_END_OF_TABLE, (mipi_dsi_cmd_flag_t)0},
};

void mipi_dsi0_callback(mipi_dsi_callback_args_t *p_args)
{
    switch (p_args->event)
    {
    case MIPI_DSI_EVENT_SEQUENCE_0:
    {
        if (MIPI_DSI_SEQUENCE_STATUS_DESCRIPTORS_FINISHED == p_args->tx_status)
        {
            g_message_sent = true;
        }
        break;
    }
    case MIPI_DSI_EVENT_PHY:
    {
        g_phy_status |= p_args->phy_status;
        break;
    }
    default:
    {
        break;
    }
    }
}

static void mipi_dsi_push_table(const lcd_table_setting_t *table)
{
    fsp_err_t err = FSP_SUCCESS;
    const lcd_table_setting_t *p_entry = table;

    while (MIPI_DSI_DISPLAY_CONFIG_DATA_END_OF_TABLE != p_entry->cmd_id)
    {
        mipi_dsi_cmd_t msg =
        {
            .channel = 0,
            .cmd_id = p_entry->cmd_id,
            .flags = p_entry->flags,
            .tx_len = p_entry->size,
            .p_tx_buffer = p_entry->buffer,
        };

        if (MIPI_DSI_DISPLAY_CONFIG_DATA_DELAY_FLAG == msg.cmd_id)
        {
            R_BSP_SoftwareDelay(table->size, BSP_DELAY_UNITS_MILLISECONDS);
        }
        else
        {
            g_message_sent = false;
            /* Send a command to the peripheral device */
            err = R_MIPI_DSI_Command(&g_mipi_dsi0_ctrl, &msg);
            if (err != FSP_SUCCESS)
            {
                LOG_E("R_MIPI_DSI_Command error\n");
            }
            /* Wait */
            while (!g_message_sent);
        }
        p_entry++;
    }
}

void ra8_mipi_lcd_init(void)
{
    mipi_dsi_push_table(g_lcd_init_focuslcd);
}

#endif
