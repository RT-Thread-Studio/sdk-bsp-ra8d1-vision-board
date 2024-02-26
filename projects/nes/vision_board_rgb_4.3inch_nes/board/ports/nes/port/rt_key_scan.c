#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <rt_key_scan.h>


/* 配置 KEY 输入引脚  */
#define PIN_KEY0        GET_PIN(C, 0)     // PC0:  KEY0         --> KEY
#define PIN_KEY1        GET_PIN(C, 1)      // PC1 :  KEY1         --> KEY
#define PIN_KEY2        GET_PIN(C, 4)      // PC4 :  KEY2         --> KEY
#define PIN_WK_UP       GET_PIN(C, 5)     // PC5:  WK_UP        --> KEY

rt_event_t key_event;

static void key_scan()
{

}
void key_scan_init()
{

}
