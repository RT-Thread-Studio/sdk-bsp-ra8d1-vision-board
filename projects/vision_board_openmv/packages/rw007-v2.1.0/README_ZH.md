# rw007

中文页 | [English](README.md)

## 1. 简介

**RW007**是由上海睿赛德电子科技有限公司开发基于 WIFI SOC的SPI/UART 高速wifi模块。该仓库为**rw007**的SPI驱动

**RW007**硬件设计简单，SPI模式下用户只需要预留1组SPI信号，一个中断输入，一个IO输出即可，包含电源和地总共8个引脚。

### 1.1. 文件结构

| 文件夹 | 说明 |
| ---- | ---- |
| src  | 核心驱动源码，主要实现通信逻辑 |
| inc  | 头文件目录 |
| example | 平台移植示例 |

### 1.2 许可证

rw007 package 遵循 Apache 2.0 许可，详见 `LICENSE` 文件。

### 1.3 依赖

- RT-Thread 3.0+
- RT-Thread LWIP 组件
- RT-Thread SPI 驱动框架
- RT-Thread PIN 驱动框架(示例平台代码依赖)

### 1.4 配置宏说明

自动配置时相关配置宏将在env中被配置 手动配置参考如下

类型说明

- bool: 定义生效 未定义 不生效
- string: 字符串
- int: 数值

|宏|类型|功能|
|--|--|--|
|PKG_USING_RW007|bool|开启rw007驱动，使用该软件包则需要定义该宏|
|RW007_USING_STM32_DRIVERS|bool|使能STM32平台移植示例|
|RW007_USING_IMXRT_DRIVERS|bool|使能IMXRT平台移植示例|
|RW007_USING_BLE|bool|使能开启BLE功能(仅在V2.1.0以上版本生效)|
|RW007_USING_POWERSWITCH_EXAMPLE|bool|使能开启STM32平台下的开关省电示例(仅在V2.1.0以上版本生效)|
|RW007_USING_SPI_TEST|bool|使能开启SPI 吞吐量测试功能(仅在V2.1.0以上版本生效)|
|RW007_SPI_BUS_NAME|string|example中使用的SPI总线设备名称|
|RW007_CS_PIN|int|example中使用的SPI 片选引脚在pin驱动中的序号|
|RW007_BOOT0_PIN|int|example中使用的BOOT0引脚在pin驱动中的序号(与SPI的CLK是同一引脚复用)|
|RW007_BOOT1_PIN|int|example中使用的BOOT1引脚在pin驱动中的序号(与SPI的CS是同一引脚复用)|
|RW007_INT_BUSY_PIN|int|example中使用的INT/BUSY引脚在pin驱动中的序号|
|RW007_RST_PIN|int|example中使用的RST引脚在pin驱动中的序号|

### 1.5 BLE 功能说明
  在 V2.1.0 以上的版本支持开启蓝牙 BLE 功能，使用方法如下。
  在`env`命令行输入`menuconfig`进行配置
```
-> RT-Thread online packages
  -> IoT
    ->internet of things
      -> WiFi
        --- rw007: SPI WIFI rw007 driver
              version(v2.1.0)
          [*] use ble function
          (300000000) SPI Max Hz
          (spi1) RW007 BUS NAME
          (22) CS pin index
          (39) INT/BUSY pin index
          (9) RESET pin index
          [*] example driver: rw007 for stm32
          [*] test: spi throughput capability and packet loss testing
          [*] example: save power by switching
```

#### 1.5.1 BLE 功能测试示例命令说明
  在开启`rw007`软件包中的`ble`功能，会默认打开相关的功能测试示例，可以通过`msh`命令行输入`rw007_ble`查询。

```
msh />rw007_ble
rw007_ble help
rw007_ble init central/peripheral                           Note: init ble mode
rw007_ble get_addr                                          Note: get ble address
rw007_ble update_params                                     Note: update connect parameters(no support)
rw007_ble scan                                              Note: scan ble slave
rw007_ble stop_scan                                         Note: stop scan
rw007_ble connect xx:xx:xx:xx:xx:xx                         Note: use slave addr to connect
rw007_ble disconnect [conn_handle]                          Note: disconnect slave
rw007_ble get_server                                        Note: discover all server(no support)
rw007_ble get_char                                          Note: discover all description(no support)
rw007_ble get_disc                                          Note: discover all characteristic(no support)
rw007_ble mtu_exch                                          Note: ble mtu exchange(no support)
rw007_ble notify                                            Note: enable ble notify(no support)
rw007_ble write                                             Note: ble write data(no support)
rw007_ble read                                              Note: ble read data(no support)
rw007_ble notify_change [conn_handle] [UUID] [char_value]   Note: ble notify configure by uuid
rw007_ble write_uuid [conn_handle] [UUID] [data]            Note: ble write data by uuid
rw007_ble read_uuid [conn_handle] [UUID]                    Note: ble read data by uuid
rw007_ble cfgwifi [duration_ms]                             Note: enable ble configuration wifi

```
#### 1.5.2 BLE 功能初始化

`BLE`功能初始化函数，使用`BLE`功能必须调用。
* 1.`RW007` BLE 功能初始化为主机设备。

```
rw007_ble init central
```
测试示例
```
msh />rw007_ble init central
122 - ble_cmd_init
start ble central
msh />
```
* 2.`RW007`BLE 功能初始化为从机设备。

```
rw007_ble init peripheral
```
*特别说明：目前 RW007 的从机功能仅支持微信小程序配网使用。*

#### 1.5.3 获取本机的BLE设备地址
由于BLE设备的地址分为`public`公共地址和`random`随机地址。

```
rw007_ble get_addr
```
测试示例
```
msh />rw007_ble get_addr
339 - ble_cmd_get_addr
msh />ble data input packet resp_type: 0, len: 12
public_id_addr = c8:47:8c:41:10:04
random_id_addr = 00:00:00:00:00:00
```

#### 1.5.4 扫描设备
目前 `RW007` BLE 功能支持通过 `scan`命令，扫描周围的`BLE`设备。
* 1.执行扫描周围`BLE`设备
```
rw007_ble scan
```
测试示例
```
msh >rw007_ble scan
365 - ble_cmd_scan
msh >
ble data input packet resp_type: 1, len: 37
RW007_BLE_NTF_TYPE_DISCOVER
received advertisement; event_type=0 rssi=-69 addr_type=1 addr=61:97:58:84:b3:fe
mfg_data = 02 01 1a 02 0a 0c 0a ff 4c 00 10 05 42 1c e0 56 bc
```
这里的 `addr`地址内容可以为后面连接设备使用，由于目前没有直接显示对应 BLE 设备的名称，需要先人工确定 BLE 设备的地址。

* 2.停止扫描周围`BLE`设备
该指令用于 `RW007`模块正在执行`scan`扫描操作过程中，马上停止扫描的动作，可以执行此命令。
```
rw007_ble stop_scan
```

#### 1.5.5 连接设备

目前 `RW007` BLE 功能设备支持通过 `BLE`设备的类`mac`地址来连接设备,该命令仅支持主机模式去连接从机设备的类`mac`地址,连接成功后会有对应的`conn_handle`值，这个`conn_handle`值在其他功能沿用。

测试命令
```
rw007_ble connect xx:xx:xx:xx:xx:xx
```

测试示例
```
msh />rw007_ble connect 4c:ed:fb:00:04:b1
198 - ble_cmd_connect
str_addr: 4c:ed:fb:00:04:b1
mac addr: b1: 4: 0:fb:ed:4c
msh />resp_type: 1, len: 52
RW007_BLE_NTF_TYPE_CONNECT
type: 0, status: 0, conn_handle: 1

resp_type: 1, len: 44
RW007_BLE_NTF_TYPE_CONNECT_DESC
our_id_addr: c9:47:8c:42:00:48
peer_id_addr: b1:04:00:fb:ed:4c
```

#### 1.5.6 设备断开连接
目前`RW007`BLE 功能设备支持主动断开从设备的连接。
```
rw007_ble disconnect [conn_handle]
```
其中 `conn_handle`与 `connect`连接时的需要一致。

测试示例
```
msh />rw007_ble disconnect 1
341 - ble_cmd_disconnect
msh />resp_type: 1, len: 52
RW007_BLE_NTF_TYPE_DISCONN
```

#### 1.5.7 设备写数据
目前`RW007`BLE 功能写数据，支持通过指定`UUID`来写对应的数据。
```
rw007_ble write_uuid [conn_handle] [UUID] [data]
```
指令说明：`[conn_handle]`为 `RW007`连接设备时生成的对应值，`[UUID]`为对应 `BLE`设备的特征参数，`[data]`为需要发送的数据。

测试示例
```
msh />rw007_ble write_uuid 1 ebe0ccbe7a0a4b0c8a1a6ff2997da3a6 00
data: 00
write conn_hanle:1 uuid:ebe0ccbe7a0a4b0c8a1a6ff2997da3a6
write rc:0
```
说明：测试示例中的 `[conn_handle]`为 1，`[UUID]`为`ebe0ccbe7a0a4b0c8a1a6ff2997da3a6`,`[data]`为 `0x00`(这里是使用 hex 值)

#### 1.5.8 更新 NOTIFY 参数
目前 `RW007` BLE 功能支持修改接收 `notify`参数。
```
rw007_ble notify_change [conn_handle] [char_value] [UUID]
```
参数说明：

`[conn_handle]`为 `RW007`连接设备时生成的对应值

`[char_value]` 为配置参数，具体如下

```
0：disable indication&notification
1：enable notification，disable indication
2：enable indication， disable notification
3：enable indication &notification
```

`[UUID]`为对应 `BLE`设备的特征参数值。

#### 1.5.9 广播包类型的说明
目前 `RW007`  BLE 功能中可以关于广播包 `event`的类型可以分为下面几种。
```
/* Advertising report */
#define BLE_HCI_ADV_RPT_EVTYPE_ADV_IND      (0)
#define BLE_HCI_ADV_RPT_EVTYPE_DIR_IND      (1)
#define BLE_HCI_ADV_RPT_EVTYPE_SCAN_IND     (2)
#define BLE_HCI_ADV_RPT_EVTYPE_NONCONN_IND  (3)
#define BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP     (4)
```
广播包结构如下：
```
广播包结构描述：
struct ble_gap_disc_desc {
    uint8_t event_type;
    uint8_t length_data;
    ble_addr_t addr;
    int8_t rssi;
    uint8_t *data;

    /***
     * LE direct advertising report fields; direct_addr is BLE_ADDR_ANY if
     * direct address fields are not present.
     */
    ble_addr_t direct_addr;
};

通过event_type区分出不同的广播包类型，所有类型分为如下几种：
/* Advertising report */
0 : 普通广播数据包
1 : 直接广播包 
2 : 扫描请求包 
3 : 不可连接广播指示
4 : 扫描响应数据包
```
因此，在设备的`scan`扫描回应中可以通过对应的`event_type`来判定是哪种广播包的类型。

#### 1.6.0 设备读取数据

目前 `RW007`支持通过 `UUID` 读取数据的功能。
```
rw007_ble read_uuid [conn_handle] [UUID]
```
参数说明：

`[conn_handle]`为 `RW007`连接设备时生成的对应值

`[UUID]`为对应 `BLE`设备的特征参数。

测试示例
```
msh />rw007_ble read_uuid 1 2a24
read conn_hanle:1 uuid:2a24
read rc:0
msh />ble data input packet resp_type: 0, len: 18
RW007_BLE_RSP_TYPE_READ
connect:1 attr_handle:14 uuid: 24 2a  read data:
00000000: 4C 59 57 53 44 30 33 4D 4D 43 00                 LYWSD03MMC.
```
说明：这里的 `[conn_handle]`为 1，`[UUID]` 为 `2a24 `，读取到的数据为 BLE 设备的名称。

目前 RW007 模块已经支持读取 128 位的 UUID 的功能。
测试示例
```
msh />rw007_ble read_uuid 1 ebe0ccbe7a0a4b0c8a1a6ff2997da3a6
read conn_hanle:1 uuid:ebe0ccbe7a0a4b0c8a1a6ff2997da3a6
read rc:0
msh />ble data input packet resp_type: 0, len: 22
RW007_BLE_RSP_TYPE_READ
connect:1 attr_handle:51 uuid: a6 a3 7d 99 f2 6f 1a 8a 0c 4b 0a 7a be cc e0 eb  read data:
00000000: 00                     
```
说明：这里的 `[conn_handle]`为 1，`[UUID]` 为128位的 `ebe0ccbe7a0a4b0c8a1a6ff2997da3a6 `，读取到的数据值为 `00`

#### 1.6.1 微信小程序 BLE 配网功能说明

目前`RW007`支持微信小程序 `BLE`配网功能。

```
rw007_ble cfgwifi [duration_ms]    
```
参数说明：

[duration_ms] 为模块作为从机对外进行广播的时长，以毫秒（ms）为单位，`RW007`模块对外广播的名称为`RW007-XXXX`其中`XXXX`为模块`mac`地址的后4位。

主机驱动的代码,主机端可以参考下面的实现代码，进入 `BLE` 配网模式。

```c
rt_err_t rw007_ble_cfgwifi(uint32_t duration_ms)
{
    struct rw007_ble_cfgwifi cfgwifi;

    cfgwifi.duration_ms = duration_ms;

    return spi_set_data(wifi_sta.wlan, RW00x_CMD_BLE_CFGWIFI, &cfgwifi, sizeof(struct rw007_ble_cfgwifi));
}
```
测试示例
```
msh />rw007_ble cfgwifi 30000
msh />[420332] I/WLAN.mgnt: wifi connect success ssid:
[421341] I/WLAN.lwip: Got IP address : 192.168.1.212
```
说明：`30000`为`RW007`模块作为从机对外广播的时长，一旦超过时长后，就会停止对外广播。

***关于 BLE 配网和 BLE 主机功能同时使用的一些限制事项***
（1）主机的 SCAN 功能和 BLE 配网不能同时使用，当`RW007` 模块处于`Advertise`广播状态（BLE 从机功能开启广播），主机（stm32)再执行 scan 操作时，scan操作会返回错误码，代表本次 scan 操作无效，直到 `Advertise`广播状态结束后，才能正常执行 scan 操作，但是此过程不影响已连接设备的状态。

（2）主机的 SCAN 功能和 BLE 配网不能同时使用，当主机（stm32)正在在执行BLE 主机的 scan 操作时，同时配置`RW007` 模块进行`Advertise`广播，此时 scan 操作会被打断，直到`Advertise`广播状态结束后，才能正常执行 scan 操作,但是此过程不影响已连接设备的状态。

## 2. 注意事项

由于存在引脚复用情况，所以bsp的SPI的驱动需要引脚配置在Config时进行。

## 4. 联系方式

- 维护：RT-Thread 开发团队
- 主页：https://github.com/RT-Thread-packages/rw007
