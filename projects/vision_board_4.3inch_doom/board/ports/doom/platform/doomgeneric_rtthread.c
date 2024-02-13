#include <doomgeneric.h>
#include "doomkeys.h"

static uint32_t time_since_last_key;

void DG_Init()
{
	time_since_last_key = DG_GetTicksMs();
}

void DG_SleepMs(uint32_t ms)
{
	rt_thread_mdelay(ms);
}

uint32_t DG_GetTicksMs()
{
	return rt_tick_get_millisecond();
}

int DG_GetKey(int *pressed, unsigned char *key)
{

	return 0;
}

void DG_SetWindowTitle(const char *title)
{
}
