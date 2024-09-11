# vision_board_mipi_7.0inch 示例说明

## 简介

本例程主要功能是测试 7.0 寸 mipi 屏幕，通过测试命令可以在 LCD 屏幕上显示不同颜色。

## 硬件说明

* Vision-Board 开发板
* [野火7.0寸](https://doc.embedfire.com/products/link/zh/latest/module/screen/ebf_lcd_mipi_5.5_7_10.1.html#mipi-7)mipi屏幕

![](docs/picture/shc.png)

mipi接口引脚定义如上图所示，需要将7寸屏幕通过FPC反接排线插入 Vision-Board 的正面mipi接口中，接线方式见下图：

![](docs/picture/1.jpg)

## 软件说明

触摸部分的代码位于 `/projects/vision_board_mipi_7.0inch/src/hal_entry.c` 中。在 hal_entry 中主要对野火的7寸屏幕进行了初始化，并单独开辟了一个线程用于测试读取触摸坐标点。

mipi 屏幕初始化配置代码位于： `vision_board_mipi_7.0inch\board\ports\mipi_lcd\mipi_config.c` 中。其中包括了对mipi屏幕参数的初始化。

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







#### RT-Thread Studio方式

双击mklinks.bat，会产生两个文件夹libraries和rt-rthread

![](docs/picture/rt-thread_studio1.png)



在当前项目路径下打开env，在env中输入scons --target=eclipse

![](docs/picture/rt-thread_studio2.png)



打开RT-Thread Studio，左上角菜单栏，文件-->导入

![](docs/picture/rt-thread_studio3.png)

![](docs/picture/rt-thread_studio4.png)



![](docs/picture/rt-thread_studio5.png)



![](docs/picture/rt-thread_studio6.png)











## 运行效果

* 打开 Dap-Link 虚拟出的串口终端，波特率为115200，当用手触摸显示屏时，坐标点位会在串口终端中显示。
* 串口终端中输入 `lcd_test` 指令后，LCD会以红、绿、蓝三个颜色分别刷新显示。

## QA

Q：遇到在使用 Dap-Link 时找不到目标芯片的情况并且无法下载：

![](docs/picture/download1.png)

![](docs/picture/download2.png)

A1：方法一：**由于 JTAG/SWD 在芯片出厂时首次使用是加锁的**。请按住开发板的RST按键（正面右一）不松手。再次点击MDK下载按钮后，快速松开RST按键即可。后续无需重复以上操作。

A2：方法二：使用  [Renesas Flash Programmer](https://www.renesas.com/us/en/software-tool/renesas-flash-programmer-programming-gui#documents) 烧录工具进行第一次烧录，操作方法如下：

![](docs/picture/boot1.png)

![](docs/picture/boot2.png)









**Q:  遇到编译报错（MDK）：**

![](docs/picture/MDK1.png)

问题：找不到hal_entry函数

**A:**  解决方法：hal_entry函数定义在src文件夹下，在mdk中从新把这个文件添加进去后编译即可解决

![](docs/picture/MDK2.png)

![](docs/picture/MDK3.png)

![](docs/picture/MDK4.png)



**Q:  下载报错（MDK）：**

![](docs/picture/MDK5.png)

![](docs/picture/MDK6.png)



**A:**  解决方法：

![](docs/picture/MDK7.png)

![](docs/picture/MDK8.png)

![](docs/picture/MDK9.png)

![](docs/picture/MDK10.png)

![](docs/picture/MDK11.png)

![](docs/picture/MDK12.png)





