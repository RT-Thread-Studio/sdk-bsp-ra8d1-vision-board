#include <rtthread.h>

#if (defined(BSP_USING_LCD)) || (defined(SOC_SERIES_R7FA8M85))
#include <ra8/lcd_config.h>
#include "hal_data.h"

#define DRV_DEBUG
#define LOG_TAG             "mipi_cfg"
#include <drv_log.h>

#define MIPI_DSI_DISPLAY_CONFIG_DATA_DELAY_FLAG      ((mipi_dsi_cmd_id_t) 0xFE)
#define MIPI_DSI_DISPLAY_CONFIG_DATA_END_OF_TABLE    ((mipi_dsi_cmd_id_t) 0xFD)

typedef struct
{
    unsigned char        size;
    unsigned char        buffer[20];
    mipi_dsi_cmd_id_t    cmd_id;
    mipi_dsi_cmd_flag_t flags;
} lcd_table_setting_t;

volatile static bool g_message_sent = false;
volatile static mipi_dsi_phy_status_t g_phy_status;

const lcd_table_setting_t g_lcd_init_focuslcd[] =
{
    // {2,     {0x01, 0x00},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_0_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    // {120,    {0},                                   MIPI_DSI_DISPLAY_CONFIG_DATA_DELAY_FLAG, (mipi_dsi_cmd_flag_t)0},
    /* enable the BK function of Command2: BK3 */
    /************** BK3 Function start ***************/
    {6,     {0xFF, 0x77, 0x01, 0x00, 0x00, 0x13},   MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},

    {2,     {0xEF, 0x08},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /*------------- BK3 Function end ----------------*/

    /* enable the BK function of Command2: BK0 */
    /************** BK0 Function start ***************/
    {6,     {0xFF, 0x77, 0x01, 0x00, 0x00, 0x10},   MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* Display Line Setting: SCNL = (44Line+1)*8 = 360 */
    {3,     {0xC0, 0x2C, 0x00},                     MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},  // SCNL = (0x2c + 1) * 8 = 360
    /* Porch Control: VBP(Back-Porch Vertical line) = 13, VFP(Front-Porch Vertical line) = 2*/
    {3,     {0xC1, 0x0D, 0x02},                     MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},  // VFB=0x08 VBF=0x02
    /* Inversion selection & Frame Rate Control: Inversion Selection = 2 Dot, minimum number of pclk in each line = 5*/
    {3,     {0xC2, 0x31, 0x05},                     MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},  // PCLK= 512 + (0x05 * 16) = 592
    /* TODO 0xC3, default: RGB DE mode, VSP=[x], Low active*, HSP=[x], Low active */
    /* TODO 0xC3, default: The data is input on the positive edge of DOTCLK, The data DB23-0 is written when ENABLE = 1 */
    /* TODO 0xC3, default: RGB interface Vsync back porch setting for HV mode = 16 */
    /* TODO 0xC3, default: RGB interface Hsync back porch setting for HV mode = 8 */

    /* not on data sheet! */
    {2,     {0xCC, 0x10},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* Positive Voltage Gamma Control */
    {17,    {0xB0, 0x0A, 0x14, 0x1B, 0x0D, 0x10, 0x05, 0x07, 0x08, 0x06, 0x22, 0x03, 0x11, 0x10, 0xAD, 0x31, 0x1B}, MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},   // Negative Voltage Gamma Control
    /* Negative Voltage Gamma Control */
    {17,    {0xB1, 0x0A, 0x14, 0x1B, 0x0D, 0x10, 0x05, 0x07, 0x08, 0x06, 0x22, 0x03, 0x11, 0x10, 0xAD, 0x31, 0x1B}, MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /*------------- BK0 Function end ----------------*/

    /* enable the BK function of Command2: BK1 */
    /************** BK1 Function start ***************/
    {6,     {0xFF, 0x77, 0x01, 0x00, 0x00, 0x11},   MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* Vop Amplitude setting: VRH = 80 */
    {2,     {0xB0, 0x50},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* VCOM amplitude setting: VCOM = 94 (1.275V)*/
    {2,     {0xB1, 0x5E},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* VGH Voltage setting: VGHSS = 0x87 (15V)*/
    {2,     {0xB2, 0x87},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* TEST Command Setting: TESTCMD = 0x80 */
    {2,     {0xB3, 0x80},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* VGL Voltage setting: Gate Low Voltage setting = -9.51V*/
    {2,     {0xB5, 0x47},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* Power Control 1: Gamma OP bias current selection AP = 1(Min), APIS = 1(Min), APOS = 1(Min) */
    {2,     {0xB7, 0x85},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* Power Control 2: AVDD voltage setting = 6.6V, AVCL voltage setting = -4.6V */
    {2,     {0xB8, 0x21},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* Power Control 2: source pumping cell setting SVPO_PUM, SVNO_PUM */
    // {2,     {0xB9, 0x10},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* Source pre_drive timing set1: 8(1.6uS)*/
    {2,     {0xC1, 0x78},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* Source EQ2 Setting */
    {2,     {0xC2, 0x78},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* MIPI Setting 1: EOT_EN = 1, ERR_SEL = 0 (disable) */
    {2,     {0xD0, 0x88},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* TODO MIPI Setting 2, default: */
    /* TODO MIPI Setting 3, default: */
    /* TODO MIPI Setting 4, default: */
    {2,     {0xE0, 0x00},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    {2,     {0x1B, 0x02},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},

    {12,    {0xE1, 0x08, 0xA0, 0x00, 0x00, 0x07, 0xA0, 0x00, 0x00, 0x00, 0x44, 0x44},       MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    {13,    {0xE2, 0x11, 0x11, 0x44, 0x44, 0x75, 0xA0, 0x00, 0x00, 0x74, 0xA0, 0x00, 0x00}, MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    {5,     {0xE3, 0x00, 0x00, 0x11, 0x11},         MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    {3,     {0xE4, 0x44, 0x44},                     MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    {17,    {0xE5, 0x0A, 0x71, 0xD8, 0xA0, 0x0C, 0x73, 0xD8, 0xA0, 0x0E, 0x75, 0xD8, 0xA0, 0x10, 0x77, 0xD8, 0xA0}, MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    {5,     {0xE6, 0x00, 0x00, 0x11, 0x11},         MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    {3,     {0xE7, 0x44, 0x44},                     MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    {17,    {0xE8, 0x09, 0x70, 0xD8, 0xA0, 0x0B, 0x72, 0xD8, 0xA0, 0x0D, 0x74, 0xD8, 0xA0, 0x0F, 0x76, 0xD8, 0xA0}, MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    {8,     {0xEB, 0x02, 0x00, 0xE4, 0xE4, 0x88, 0x00, 0x40},                                                       MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    {3,     {0xEC, 0x3C, 0x00},                     MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    {17,    {0xED, 0xAB, 0x89, 0x76, 0x54, 0x02, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0x20, 0x45, 0x67, 0x98, 0XBA}, MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    {7,     {0xEF, 0x08, 0x08, 0x08, 0x45, 0x3F, 0x54},                                     MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /*------------- BK1 Function end -----------------/

    /* enable the BK function of Command2: BK3 */
    /************** BK3 Function start ***************/
    {6,     {0xFF, 0x77, 0x01, 0x00, 0x00, 0x13},   MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},

    {3,     {0xE8, 0x00, 0x0E},                     MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    {3,     {0xE8, 0x00, 0x0C},                     MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    {3,     {0xE8, 0x00, 0x00},                     MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /*------------- BK3 Function end -----------------/

    /* disable the BK function of Command2 */
    {6,     {0xFF, 0x77, 0x01, 0x00, 0x00, 0x00},   MIPI_DSI_CMD_ID_DCS_LONG_WRITE, MIPI_DSI_CMD_FLAG_LOW_POWER},

    /* Interface Pixel Format: 0x55(16-bit/pixel) 0x66(18-bit/pixel) 0x77(24-bit/pixel) */
    {2,     {0x3A, 0x55},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* Display data access control: Get normal scan , BGR mode */
    // {2,     {0x36, 0x48},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* Display data access control: Get normal scan , RGB mode */
    {2,     {0x36, 0x40},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* Sleep Out */
    {2,     {0x11, 0x00},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_0_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    {120,    {0},                                   MIPI_DSI_DISPLAY_CONFIG_DATA_DELAY_FLAG, (mipi_dsi_cmd_flag_t)0},
    /* Display On */
    {2,     {0x29, 0x00},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_0_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /* Tearing Effect Line ON: TE consists of V-Blanking information only */
    // {2,     {0x35, 0x00},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_1_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    // {60,    {0},                                    MIPI_DSI_DISPLAY_CONFIG_DATA_DELAY_FLAG, (mipi_dsi_cmd_flag_t)0},
    /*  All Pixel ON */
    // {2,     {0x23, 0x00},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_0_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},
    /*  Display Inversion On */
    // {2,     {0x21, 0x00},                           MIPI_DSI_CMD_ID_DCS_SHORT_WRITE_0_PARAM, MIPI_DSI_CMD_FLAG_LOW_POWER},

    {0x00,  {0},                                    MIPI_DSI_DISPLAY_CONFIG_DATA_END_OF_TABLE, (mipi_dsi_cmd_flag_t)0},
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
    LOG_D("inited mipi dsi configs\n");
}

#endif
