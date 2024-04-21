#include "omv_boardconfig.h"
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6100100) && (OMV_ENABLE_SCC8660 == 1)
#include "scc8660.h"
#include <stdio.h>
#include "sensor.h"
#include "omv_i2c.h"

void scc8660_iic_read(unsigned char addr, unsigned char reg, unsigned char* data)
{
    omv_i2c_t bus;
    bus.inst = (struct rt_i2c_bus_device*)rt_device_find(OMV_CAM_BUS_NAME);
    uint8_t slv_addr = 0x66;
    uint8_t reg_addr = reg;

    int result = omv_i2c_readb(&bus, slv_addr, reg_addr, data);
    if(result != 0)
    {
        rt_kprintf("scc8660 read error\n");
    }
}

void scc8660_iic_write(unsigned char addr, unsigned char reg, unsigned char data)
{
    omv_i2c_t bus;
    bus.inst = (struct rt_i2c_bus_device*)rt_device_find(OMV_CAM_BUS_NAME);
    uint8_t slv_addr = 0x66;
    uint8_t reg_addr = reg;
    int result = omv_i2c_writeb(&bus, slv_addr, reg_addr, data);
    if(result != 0)
    {
        rt_kprintf("scc8660 write error\n");
    }
}

int read_reg(sensor_t* sensor, uint16_t reg_addr)
{
    uint8_t reg_data;
    if(omv_i2c_readb(&sensor->i2c_bus, sensor->slv_addr, reg_addr, &reg_data) != 0)
    {
        return -1;
    }
    return reg_data;
}
int write_reg(sensor_t* sensor, uint16_t reg_addr, uint16_t reg_data)
{
    return omv_i2c_writeb(&sensor->i2c_bus, sensor->slv_addr, reg_addr, reg_data);
}
static int set_framesize(sensor_t* sensor, framesize_t framesize)
{
    int ret = 0;
    switch(framesize)
    {
    case FRAMESIZE_VGA:
		ret =scc8660_set_framesize(1);
        break;
    case FRAMESIZE_QVGA:
        ret =scc8660_set_framesize(2);
        break;
    case FRAMESIZE_HQVGA:
        ret =scc8660_set_framesize(3);
        break;
    case FRAMESIZE_QQVGA:
        ret =scc8660_set_framesize(4);
        break;
    default:
        ret=-1;
		break;
    }
    return ret;
}
static int set_brightness(sensor_t* sensor, int level)
{
    int ret = 0;
    scc8660_set_brightness(level);
    return ret;
}
static int set_auto_exposure(sensor_t* sensor, int enable, int exposure_value)
{
    int ret = 0;
    scc8660_set_auto_exposure(enable, exposure_value);
    return ret;
}
static int set_pixformat(sensor_t* sensor, pixformat_t pixformat)
{
    int ret = 0;

    return ret;
}
static int set_hmirror(sensor_t* sensor, int enable)
{
    scc8660_set_hmirror(enable);
    return 0;
}
static int set_vflip(sensor_t* sensor, int enable)
{
    scc8660_set_vflip(enable);
    return 0;
}
static int set_auto_whitebal(sensor_t* sensor, int enable, float r_gain_db, float g_gain_db, float b_gain_db)
{
    int ret = 0;
    scc8660_set_auto_whitebal(enable, r_gain_db, g_gain_db, b_gain_db);
    return ret;
}
int SCC8660_init(sensor_t* sensor)
{
    //Initialize sensor structure.
    scc8660_init();


    sensor->set_framesize       = set_framesize;
    sensor->set_brightness      = set_brightness;
    sensor->set_auto_exposure   = set_auto_exposure;
    sensor->set_pixformat       = set_pixformat;
    sensor->set_hmirror         = set_hmirror;
    sensor->set_vflip           = set_vflip;
    sensor->set_auto_whitebal   = set_auto_whitebal;

    // Set sensor flags
    sensor->hw_flags.vsync = 0;
    sensor->hw_flags.hsync = 0;
    sensor->hw_flags.pixck = 1;
    sensor->hw_flags.fsync = 0;
    sensor->hw_flags.jpege = 0;
    sensor->hw_flags.gs_bpp = 2;
    sensor->hw_flags.rgb_swap = 1;
    sensor->hw_flags.yuv_order = SENSOR_HW_FLAGS_YVU422;

    return 0;
}
#endif // (OMV_ENABLE_SCC8660 == 1)
