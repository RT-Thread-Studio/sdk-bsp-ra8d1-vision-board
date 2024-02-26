# GT911
GT911 是专为 7”~8”设计的新一代 5 点电容触控方案，拥有 26 个驱动通道和 14 个感应 通道，以满足更高的 touch 精度要求，本软件包已经对接到了Touch框架，通过 Touch 框架，开发者可以快速的将此触摸芯片驱动起来。

## 支持情况

| 包含设备 | 触摸芯片 |
|----------|----------|
| **通讯接口** |  |
| IIC | √ |
| **工作模式** |  |
| 中断 | √ |
| 轮询 | √ |

## 使用说明

### 依赖
- RT-Thread 4.0.0+
- Touch 框架
- I2C 驱动：gt911设备使用 I2C 进行数据通讯

### 获取软件包

使用 gt911软件包需要在 RT-Thread 的包管理中选中它，具体路径如下：

```
RT-Thread online packages  --->
  peripheral libraries and drivers  --->
    touch drivers  --->
      GT911 touch driver package  --->
              Version (latest)  --->
```

### 使用软件包

gt911软件包初始化函数如下所示：

```
int rt_hw_gt911_init(const char *name, struct rt_touch_config *cfg)
```

该函数需要由用户调用，函数主要完成的功能有，

- 设备配置和初始化（根据传入的配置信息，配置接口设备和中断引脚）；
- 注册相应的传感器设备，完成 gt911设备的注册；

#### 初始化示例

```.c
int rt_hw_gt911_port(void)
{
    struct rt_touch_config cfg;
    rt_uint8_t rst_pin;

    rst_pin = GT911_RST_PIN;
    cfg.dev_name = "i2c2";
    cfg.irq_pin.pin = GT911_IRQ_PIN;
    cfg.irq_pin.mode = PIN_MODE_INPUT_PULLDOWN;
    cfg.user_data = &rst_pin;

    rt_hw_gt911_init("gt911", &cfg);

    return 0;
}
INIT_ENV_EXPORT(rt_hw_gt911_port);
```

## 联系人信息

维护人:

- [RiceChen](https://github.com/RiceChen) 

- 邮箱：980307037@qq.com
