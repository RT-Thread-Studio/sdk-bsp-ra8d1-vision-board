#ifndef __OMV_LCD_H
#define __OMV_LCD_H

#include <stdint.h>

typedef struct
{
    void *dev;
    unsigned char *fb;
    unsigned short width, height;
} ovmhal_lcd_t;

int omvhal_lcd_init(ovmhal_lcd_t *lcd, int lcdtype, int *o_width, int *o_height);
void omvhal_lcd_deinit(ovmhal_lcd_t *lcd);
void omvhal_lcd_draw(ovmhal_lcd_t *lcd, int x, int y, int w, int h, unsigned short *p);
void omvhal_lcd_clear(ovmhal_lcd_t *lcd, int color);
void omvhal_set_backlight(ovmhal_lcd_t *lcd, int state);

#endif
