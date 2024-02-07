# rw007

[中文页](README_ZH.md) | English

## 1. Introduction

**RW007** is a SPI/UART high-speed wifi module based on Cortex-M4 WIFI SOC developed by Shanghai Ruiside Electronic Technology Co., Ltd. The warehouse is the SPI driver of **rw007**

**RW007** The hardware design is simple. In SPI mode, users only need to reserve 1 set of SPI signals, one interrupt input, and one IO output, including a total of 8 pins for power and ground.

### 1.1. File structure

| Folder | Description |
| ---- | ---- |
| src | Core driver source code, which mainly implements communication logic |
| inc | Header file directory |
| example | Platform porting example |

### 1.2 License

The at_device package complies with the Apache 2.0 license, see the `LICENSE` file for details.

### 1.3 Dependency

- RT-Thread 3.0+
- RT-Thread LWIP component
- RT-Thread SPI driver framework
- RT-Thread PIN driver framework (sample platform code dependency)

### 1.4 Configuration Macro Description

The relevant configuration macros will be configured in env during automatic configuration. The manual configuration is as follows

Type description

- bool: definition is valid, undefined, not valid
- string: string
- int: number

|Macro|Type|Function|
|--|--|--|
|PKG_USING_RW007|bool|Enable rw007 driver, use this software package, you need to define this macro|
|RW007_USING_STM32_DRIVERS|bool|Enable STM32 platform migration example|
|RW007_USING_IMXRT_DRIVERS|bool|Enable IMXRT platform migration example|
|RW007_USING_BLE|bool|Enable the BLE function (only effective in V2.1.0 or later)|
|RW007_USING_POWERSWITCH_EXAMPLE|bool|Enable the power-saving example of the switch under the STM32 platform(only effective in V2.1.0 or later)|
|RW007_USING_SPI_TEST|bool|Enable the SPI throughput test function(only effective in V2.1.0 or later)|
|RW007_SPI_BUS_NAME|string|The SPI bus device name used in example|
|RW007_CS_PIN|int|The serial number of the SPI chip select pin used in the example in the pin driver|
|RW007_BOOT0_PIN|int|The serial number of the BOOT0 pin used in the example in the pin driver (the same pin is multiplexed with the CLK of SPI)|
|RW007_BOOT1_PIN|int|The serial number of the BOOT1 pin used in the example in the pin driver (the same pin is multiplexed with the CS of SPI)|
|RW007_INT_BUSY_PIN|int|The serial number of the INT/BUSY pin used in the example in the pin driver|
|RW007_RST_PIN|int|The serial number of the RST pin used in the example in the pin driver|

### 1.5 BLE function description
The version above V2.1.0 supports enabling the Bluetooth BLE function. The usage method is as follows.
   Enter `menuconfig` in the `env` command line to configure
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
#### 1.5.1 BLE function test example command description
When the `ble` function in the `rw007` software package is turned on, the relevant functional test examples will be opened by default. You can enter `rw007_ble` to query through the `msh` command line.

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
#### 1.5.2 BLE function initialization
The `BLE` function initialization function must be called to use the `BLE` function.

* 1. The `RW007` BLE function is initialized as a host device.
```
rw007_ble init central
```
Example
```
msh />rw007_ble init central
122-ble_cmd_init
start ble central
msh />
```
* 2. The `RW007`BLE function is initialized as a slave device.
```
rw007_ble init peripheral
```
Special note: At present, the slave function of RW007 only supports the use of WeChat applet.

#### 1.5.3 Get the local BLE device address
The address of BLE device is divided into `public`  address and `random` address.

```
rw007_ble get_addr
```
Example
```
msh />rw007_ble get_addr
339 - ble_cmd_get_addr
msh />ble data input packet resp_type: 0, len: 12
public_id_addr = c8:47:8c:41:10:04
random_id_addr = 00:00:00:00:00:00
```
#### 1.5.4 SCAN
The `RW007` BLE function supports scanning the surrounding `BLE` devices through the `scan` command.

* 1. Scanning of surrounding `BLE` devices
```
rw007_ble scan
```
Example
```
msh >rw007_ble scan
365 - ble_cmd_scan
msh >
ble data input packet resp_type: 1, len: 37
RW007_BLE_NTF_TYPE_DISCOVER
received advertisement; event_type=0 rssi=-69 addr_type=1 addr=61:97:58:84:b3:fe
mfg_data = 02 01 1a 02 0a 0c 0a ff 4c 00 10 05 42 1c e0 56 bc
```
The content of the `addr` address here can be used by the connected device later. Since the name of the corresponding BLE device is not directly displayed at present, the address of the BLE device needs to be manually determined first.

* 2. Stop scanning surrounding `BLE` devices
```
rw007_ble stop_scan
```
#### 1.5.5 CONNECT
`RW007` BLE function device supports to connect to the device through the class `mac` address of the `BLE` device. This command only supports the master mode to connect to the class `mac` address of the slave device. After the connection is successful, there will be a corresponding `conn_handle `Value, this `conn_handle` value is used in other functions.

Test command 
```
rw007_ble connect xx:xx:xx:xx:xx:xx
```
Example
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
#### 1.5.6 DISCONNECT
`RW007`BLE function device supports active disconnection of the slave device.
```
rw007_ble disconnect [conn_handle]
```
The requirements of `conn_handle` and `connect` are the same.

Example
```
msh />rw007_ble disconnect 1
341 - ble_cmd_disconnect
msh />resp_type: 1, len: 52
RW007_BLE_NTF_TYPE_DISCONN
```
#### 1.5.7 WRITE

`RW007`BLE function to write data, supports writing corresponding data by specifying `UUID`.
```
rw007_ble write_uuid [conn_handle] [UUID] [data]
```
Instruction description: `[conn_handle]` is the corresponding value generated when `RW007` is connected to the device, `[UUID]` is the characteristic parameter of the corresponding `BLE` device, and `[data]` is the data to be sent.

Example
```
msh />rw007_ble write_uuid 1 ebe0ccbe7a0a4b0c8a1a6ff2997da3a6 00
data: 00
write conn_hanle:1 uuid:ebe0ccbe7a0a4b0c8a1a6ff2997da3a6
write rc:0
```
Note: In the test example, `[conn_handle]` is 1, `[UUID]` is `ebe0ccbe7a0a4b0c8a1a6ff2997da3a6`, and `[data]` is `0x00` (Hex value is used here)

#### 1.5.8 Update the NOTIFY parameter

`RW007` BLE function supports modifying the receiving `notify` parameters.
```
rw007_ble notify_change [conn_handle] [char_value] [UUID]
```
Parameter Description:

`[conn_handle]` is the corresponding value generated when `RW007` is connected to the device

`[char_value]` is the configuration parameter, as follows.
```
0：disable indication&notification
1：enable notification，disable indication
2：enable indication， disable notification
3：enable indication &notification
```
`[UUID]` is the characteristic parameter value of the corresponding `BLE` device.
#### 1.5.9 Description of broadcast packet types
The type of broadcast packet `event` in the `RW007` BLE function can be divided into the following types.
```
/* Advertising report */
#define BLE_HCI_ADV_RPT_EVTYPE_ADV_IND      (0)
#define BLE_HCI_ADV_RPT_EVTYPE_DIR_IND      (1)
#define BLE_HCI_ADV_RPT_EVTYPE_SCAN_IND     (2)
#define BLE_HCI_ADV_RPT_EVTYPE_NONCONN_IND  (3)
#define BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP     (4)
```
The broadcast packet structure is as follows:
```
Broadcast package structure description:
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

Different broadcast packet types are distinguished by event_type. All types are divided into the following types:
/* Advertising report */
0: normal broadcast packet
1: Direct broadcast package
2: Scan request package
3: Not connect to broadcast instructions
4: Scan response packet
```
Therefore, in the `scan` scan response of the device, the corresponding `event_type` can be used to determine the type of broadcast packet.

#### 1.6.0 READ
`RW007` supports the function of reading data through `UUID`.
```
rw007_ble read_uuid [conn_handle] [UUID]
```
Parameter Description:

`[conn_handle]` is the corresponding value generated when `RW007` is connected to the device

`[UUID]` is the characteristic parameter of the corresponding `BLE` device.

Example
```
msh />rw007_ble read_uuid 1 2a24
read conn_hanle:1 uuid:2a24
read rc:0
msh />ble data input packet resp_type: 0, len: 18
RW007_BLE_RSP_TYPE_READ
connect:1 attr_handle:14 uuid: 24 2a  read data:
00000000: 4C 59 57 53 44 30 33 4D 4D 43 00                 LYWSD03MMC.
```
Note: Here, `[conn_handle]` is 1, `[UUID]` is `2a24`, and the data read is the name of the BLE device.

The RW007 module already supports the function of reading 128-bit UUID.

Example

```
msh />rw007_ble read_uuid 1 ebe0ccbe7a0a4b0c8a1a6ff2997da3a6
read conn_hanle:1 uuid:ebe0ccbe7a0a4b0c8a1a6ff2997da3a6
read rc:0
msh />ble data input packet resp_type: 0, len: 22
RW007_BLE_RSP_TYPE_READ
connect:1 attr_handle:51 uuid: a6 a3 7d 99 f2 6f 1a 8a 0c 4b 0a 7a be cc e0 eb  read data:
00000000: 00                     
```
Note: Here `[conn_handle]` is 1, `[UUID]` is 128-bit `ebe0ccbe7a0a4b0c8a1a6ff2997da3a6`, and the read data value is `00`

#### 1.6.1 BLE network configuration function description

`RW007` supports the WeChat applet `BLE` network configuration function.

```
rw007_ble cfgwifi [duration_ms]    
```
Parameter Description:

[duration_ms] is the duration for the module to broadcast to the outside world as a slave, in milliseconds (ms). The name of the `RW007` module to broadcast to the outside world is `RW007-XXXX`, where `XXXX` is the last 4 digits of the module's `mac` address .

For the host-driven code, the host side can refer to the following implementation code to enter the `BLE` network distribution mode.
```c
rt_err_t rw007_ble_cfgwifi(uint32_t duration_ms)
{
    struct rw007_ble_cfgwifi cfgwifi;

    cfgwifi.duration_ms = duration_ms;

    return spi_set_data(wifi_sta.wlan, RW00x_CMD_BLE_CFGWIFI, &cfgwifi, sizeof(struct rw007_ble_cfgwifi));
}
```
Example
```
msh />rw007_ble cfgwifi 30000
msh />[420332] I/WLAN.mgnt: wifi connect success ssid:
[421341] I/WLAN.lwip: Got IP address : 192.168.1.212
```
Note: `30000` is the duration of external broadcast of the `RW007` module as a slave. Once the duration exceeds the duration, the external broadcast will stop.

***Some restrictions on the simultaneous use of BLE network configuration and BLE host functions***
(1) The SCAN function of the host and the BLE network distribution cannot be used at the same time. When the `RW007` module is in the `Advertise` broadcast state (BLE slave function opens broadcast), and the host (stm32) executes the scan operation again, the scan operation will return an error Code, it means that the scan operation is invalid. The scan operation can not be performed normally until the broadcast status of `Advertise` ends, but this process does not affect the status of the connected device.

(2) The SCAN function of the host and the BLE network configuration cannot be used at the same time. When the host (stm32) is performing the scan operation of the BLE host, and the `RW007` module is configured for `Advertise` broadcast at the same time, the scan operation will be interrupted. The scan operation can only be performed normally after the broadcast status of `Advertise` ends, but this process does not affect the status of connected devices.

## 2. Matters needing attention

Because of the pin multiplexing situation, the driver of the bsp SPI needs to be configured during Config.

## 4. Contact

- Maintenance: RT-Thread development team
- Homepage: https://github.com/RT-Thread-packages/rw007
