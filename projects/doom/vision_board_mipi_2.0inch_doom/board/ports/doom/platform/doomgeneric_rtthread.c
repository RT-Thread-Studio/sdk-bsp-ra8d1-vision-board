#include "doomtype.h"
#include <doomgeneric.h>
#include "doomkeys.h"

#include <ra8/lcd_config.h>
#include "hal_data.h"

typedef struct
{
    uint16_t top;
    uint16_t bottom;
    uint16_t left;
    uint16_t right;
} constraint;

typedef enum
{
    VJ_EVENT_NONE,
    VJ_PRESSED,
    VJ_RELEASED,
} joystick_event_type;

typedef enum
{
    VJ_KEY_NONE,           // 0
    VJ_PUSH_BUTTON,        // 1
    VJ_SCREEN_TOP_LEFT,    // 2
    VJ_SCREEN_TOP_RIGHT,   // 3
    VJ_SCREEN_BOTTOM_LEFT, // 4
    VJ_SCREEN_BOTOM_RIGHT, // 5
    VJ_SCREEN_UP,          // 6
    VJ_SCREEN_DOWN,        // 7
    VJ_SCREEN_LEFT,        // 8
    VJ_SCREEN_RIGHT,       // 9
    VJ_SCREEN_CENTER,      // 10

    VJ_KEY_COUNT
} virtual_joystick_key;

const constraint constraints[] = {

    [VJ_SCREEN_TOP_LEFT] = {
        .left = 550,
        .top = 340,
        .right = 800,
        .bottom = 480,
    },

    [VJ_SCREEN_TOP_RIGHT] = {
        .left = 0,
        .top = 340,
        .right = 250,
        .bottom = 480,
    },

    [VJ_SCREEN_BOTTOM_LEFT] = {
        .left = 550,
        .top = 0,
        .right = 800,
        .bottom = 140,
    },

    [VJ_SCREEN_BOTOM_RIGHT] = {
        .left = 0,
        .top = 0,
        .right = 250,
        .bottom = 140,
    },

    [VJ_SCREEN_LEFT] = {.top = 140, .bottom = 340, .left = 550, .right = 800},

    [VJ_SCREEN_RIGHT] = {.top = 140, .bottom = 340, .left = 0, .right = 250},

    [VJ_SCREEN_UP] = {.top = 340, .bottom = 480, .left = 250, .right = 550},

    [VJ_SCREEN_DOWN] = {.top = 0, .left = 250, .bottom = 140, .right = 550},

    [VJ_SCREEN_CENTER] = {.top = 140, .bottom = 340, .left = 250, .right = 550},
};

static uint32_t time_since_last_key;

static void key_init(void);

void DG_Init()
{
    time_since_last_key = DG_GetTicksMs();

    key_init();
}

void DG_SleepMs(uint32_t ms)
{
    rt_thread_mdelay(ms);
}

uint32_t DG_GetTicksMs()
{
    return rt_tick_get_millisecond();
}

static void key_init(void)
{
}

int DG_GetKey(int *pressed, unsigned char *key)
{
    return 0;
}

void DG_SetWindowTitle(const char *title)
{
}

