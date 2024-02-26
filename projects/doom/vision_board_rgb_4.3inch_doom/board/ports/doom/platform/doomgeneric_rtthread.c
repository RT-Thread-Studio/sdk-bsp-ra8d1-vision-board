#include "doomtype.h"
#include <doomgeneric.h>
#include "doomkeys.h"

#include <lcd_port.h>
#include "hal_data.h"
#include "gt9147.h"

#define GT9147_RST_PIN BSP_IO_PORT_00_PIN_00
#define GT9147_IRQ_PIN BSP_IO_PORT_00_PIN_10

#define IRQ_BTN_PIN "p907"

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
	VJ_KEY_NONE,		   // 0
	VJ_PUSH_BUTTON,		   // 1
	VJ_SCREEN_TOP_LEFT,	   // 2
	VJ_SCREEN_TOP_RIGHT,   // 3
	VJ_SCREEN_BOTTOM_LEFT, // 4
	VJ_SCREEN_BOTOM_RIGHT, // 5
	VJ_SCREEN_UP,		   // 6
	VJ_SCREEN_DOWN,		   // 7
	VJ_SCREEN_LEFT,		   // 8
	VJ_SCREEN_RIGHT,	   // 9
	VJ_SCREEN_CENTER,	   // 10

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

static rt_device_t touch_dev;
struct rt_touch_data *read_data;

static uint32_t time_since_last_key;
static uint8_t joystick_event;
static uint8_t touch_state = 0;
static uint8_t fire_state = 0;

static rt_err_t rt_hw_gt9147_register(void);
static void key_init(void);

void DG_Init()
{
	time_since_last_key = DG_GetTicksMs();

	rt_hw_gt9147_register();
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

static void key_irq_callback(void *args)
{
	joystick_event = VJ_PUSH_BUTTON;
	fire_state = VJ_PRESSED;
}

static void key_init(void)
{
	/* init */
	rt_uint32_t pin = rt_pin_get(IRQ_BTN_PIN);
	rt_err_t err = rt_pin_attach_irq(pin, PIN_IRQ_MODE_FALLING, key_irq_callback, RT_NULL);
	if (RT_EOK != err)
	{
		rt_kprintf("\n attach irq failed. \n");
	}
	err = rt_pin_irq_enable(pin, PIN_IRQ_ENABLE);
	if (RT_EOK != err)
	{
		rt_kprintf("\n enable irq failed. \n");
	}
}

static inline bool does_fit_constraint(uint16_t x, uint16_t y, const constraint *cstr)
{
	if (x == 1) // Workaround for touching the borders of the screen
	{
		return false;
	}

	return (x >= cstr->left && x <= cstr->right) && (y >= cstr->top && y <= cstr->bottom);
}

int DG_GetKey(int *pressed, unsigned char *key)
{
	extern boolean menuactive;
	static uint8_t touch_passed = 0;

	if (RT_NULL != rt_device_read(touch_dev, 0, read_data, 1))
	{
		if (read_data->event == RT_TOUCH_EVENT_NONE)
			return 0;

		if (read_data->event == RT_TOUCH_EVENT_DOWN || read_data->event == RT_TOUCH_EVENT_MOVE)
		{
			touch_state = VJ_PRESSED;
			touch_passed = 1;
		}
		if (read_data->event == RT_TOUCH_EVENT_UP)
		{
			touch_state = VJ_RELEASED;
			touch_passed = 0;
		}

		for (int i = 0; i < sizeof(constraints) / sizeof(constraints[0]); i++)
		{
			if (i >= VJ_SCREEN_TOP_LEFT && i <= VJ_SCREEN_CENTER && touch_passed && does_fit_constraint(read_data->x_coordinate, read_data->y_coordinate, &constraints[i]))
			{
				joystick_event = i;
			}
		}

		switch (joystick_event)
		{
		case VJ_SCREEN_TOP_LEFT:
		{
			*key = KEY_ESCAPE;
			break;
		}
		case VJ_SCREEN_TOP_RIGHT:
		{
			*key = KEY_TAB;
			break;
		}
		case VJ_SCREEN_BOTTOM_LEFT:
		{
			*key = KEYP_MINUS;
			break;
		}
		case VJ_SCREEN_BOTOM_RIGHT:
		{
			*key = KEYP_PLUS;
			break;
		}
		case VJ_PUSH_BUTTON:
		{
			if (menuactive)
			{
				*key = KEY_ENTER;
			}
			else
			{
				*key = KEY_FIRE;
			}

			break;
		}
		case VJ_SCREEN_UP:
		{
			*key = KEY_UPARROW;
			break;
		}
		case VJ_SCREEN_DOWN:
		{
			*key = KEY_DOWNARROW;
			break;
		}
		case VJ_SCREEN_LEFT:
		{
			*key = KEY_LEFTARROW;
			break;
		}
		case VJ_SCREEN_RIGHT:
		{
			*key = KEY_RIGHTARROW;
			break;
		}
		case VJ_SCREEN_CENTER:
		{
			if (menuactive)
			{
				*key = KEY_ENTER;
			}
			else
			{
				*key = KEY_USE;
			}
			break;
		}
		default:
		{
			return 0;
		}
		}
		// rt_kprintf("joystick_event:%d touch_state:%d\n", joystick_event, touch_state);

		if (fire_state == VJ_PRESSED)
		{
			*pressed = fire_state == VJ_PRESSED;
			fire_state = VJ_RELEASED;
			return 1;
		}
		*pressed = touch_state == VJ_PRESSED;
		return 1;
	}
	return 0;
}

void DG_SetWindowTitle(const char *title)
{
}

static rt_err_t gt9147_probe(rt_uint16_t x, rt_uint16_t y)
{
	void *id;

	touch_dev = rt_device_find("gt9147");
	if (touch_dev == RT_NULL)
	{
		rt_kprintf("can't find device gt911\n");
		return -1;
	}

	if (rt_device_open(touch_dev, RT_DEVICE_FLAG_INT_RX) != RT_EOK)
	{
		rt_kprintf("open device failed!");
		return -1;
	}

	id = rt_malloc(sizeof(struct rt_touch_info));
	rt_device_control(touch_dev, RT_TOUCH_CTRL_GET_ID, id);
	rt_uint8_t *read_id = (rt_uint8_t *)id;
	rt_kprintf("id = GT%d%d%d \n", read_id[0] - '0', read_id[1] - '0', read_id[2] - '0');

	rt_device_control(touch_dev, RT_TOUCH_CTRL_SET_X_RANGE, &x); /* if possible you can set your x y coordinate*/
	rt_device_control(touch_dev, RT_TOUCH_CTRL_SET_Y_RANGE, &y);
	rt_device_control(touch_dev, RT_TOUCH_CTRL_GET_INFO, id);
	rt_kprintf("range_x = %d \n", (*(struct rt_touch_info *)id).range_x);
	rt_kprintf("range_y = %d \n", (*(struct rt_touch_info *)id).range_y);
	rt_kprintf("point_num = %d \n", (*(struct rt_touch_info *)id).point_num);
	rt_free(id);

	read_data = (struct rt_touch_data *)rt_calloc(1, sizeof(struct rt_touch_data));
	if (!read_data)
	{
		return -RT_ENOMEM;
	}

	return RT_EOK;
}

static rt_err_t rt_hw_gt9147_register(void)
{
	struct rt_touch_config config;
	rt_uint8_t rst;
	rst = GT9147_RST_PIN;
	config.dev_name = "sci3i";
	config.irq_pin.pin = GT9147_IRQ_PIN;
	config.irq_pin.mode = PIN_MODE_INPUT_PULLDOWN;
	config.user_data = &rst;

	if (rt_hw_gt9147_init("gt9147", &config) != RT_EOK)
	{
		rt_kprintf("touch device gt9147 init failed.\n");
		return -RT_ERROR;
	}

	if (gt9147_probe(LCD_WIDTH, LCD_HEIGHT) != RT_EOK)
	{
		rt_kprintf("probe gt9147 failed.\n");
		return -RT_ERROR;
	}
	return RT_EOK;
}
