# vision_board_tinyusb_xpad 例程

## 简介

本例程主要功能是tinyusb host 工程展示。 这个例程也可以做为您的创作的基础usb工程，该实例默认需要一个XBoxOne 控制器

## 硬件说明

![](docs/picture/1.png)

硬件电路上需要对USB_CH_SEL引脚进行处理，默认拉低，只支持USB-FS，因为對接的XBoxOne 控制器本身是USB-FS

![](docs/picture/4.png)


一个XBoxOne 控制器 (VID:PID 0x045e:0x0b12)

![](docs/picture/xboxone.jpg)


## 软件说明

在tinyusb_port.c中完成了tinyusb的初始化：

![](docs/picture/2.png)

在drv_tinyusb.c中完成了fs-hs的切换以及中断的处理：

![](docs/picture/3.png)

## 运行

### 编译&下载

#### MDK 方式

1、双击 `mklinks.bat` 文件，执行脚本后会生成 `rt-thread`、`libraries` 两个文件夹：

![](docs/picture/mklinks.png)

2、编译固件

双击 **project.uvprojx** 文件打开MDK工程

![](docs/picture/uvprojx.png)

点击下图按钮进行项目全编译：

![](docs/picture/build.png)

3、烧录固件

将开发板的 Dap-Link USB 口与 PC 机连接，然后将固件下载至开发板。

![](docs/picture/download.png)

## 运行效果

正常运行后，插上 xbox one 控制器会显示
"XPAD device address = 1, instance = 0 is mounted"

然后按下 D-Pad 或是 X,Y,A, B, Start, Select 按钮也都会显示

![](docs/picture/xbox_operations.png)

## QA

Q：遇到在使用 Dap-Link 时找不到目标芯片的情况并且无法下载：

![](docs/picture/download1.png)

![](docs/picture/download2.png)

A1：方法一：**由于 JTAG/SWD 在芯片出厂时首次使用是加锁的**。请按住开发板的RST按键（正面右一）不松手。再次点击MDK下载按钮后，快速松开RST按键即可。后续无需重复以上操作。

A2：方法二：使用  [Renesas Flash Programmer](https://www.renesas.com/us/en/software-tool/renesas-flash-programmer-programming-gui#documents) 烧录工具进行第一次烧录，操作方法如下：

![](docs/picture/boot1.png)

![](docs/picture/boot2.png)
