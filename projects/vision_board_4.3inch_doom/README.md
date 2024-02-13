# 毁灭战士游戏Doom例程

## 简介

本例程主要功能为90年代毁灭战士游戏Doom(Doom在电子游戏界中被普遍视作第一人称射击游戏的开拓者之一)的移植。

<img src="docs/picture/1.png" style="zoom: 65%;" />

<img src="docs/picture/2.png" style="zoom: 67%;" />

## 硬件说明

* Vision Board 开发板
* 4.3寸正点原子RGB屏幕
* SD卡（*FAT32*格式）
* 需拷贝 `board/ports/doom/games` 下的游戏文件（任选其一）到SD卡的 doom 文件夹下（没有请先创建doom文件夹）

## 软件说明

启动doom游戏的源代码位于 `/projects/vision_board_4.3inch_doom/src/hal_entry.c` 中。

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

系统会等待SD卡挂载成功，成功后会自动启动运行游戏：

<img src="docs/picture/shell.png" style="zoom:50%;" />
