#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

/* Automatically generated file; DO NOT EDIT. */
/* RT-Thread Configuration */

/* RT-Thread Kernel */

#define RT_NAME_MAX 12
#define RT_ALIGN_SIZE 8
#define RT_THREAD_PRIORITY_32
#define RT_THREAD_PRIORITY_MAX 32
#define RT_TICK_PER_SECOND 1000
#define RT_USING_OVERFLOW_CHECK
#define RT_USING_HOOK
#define RT_HOOK_USING_FUNC_PTR
#define RT_USING_IDLE_HOOK
#define RT_IDLE_HOOK_LIST_SIZE 4
#define IDLE_THREAD_STACK_SIZE 256
#define RT_USING_TIMER_SOFT
#define RT_TIMER_THREAD_PRIO 4
#define RT_TIMER_THREAD_STACK_SIZE 512

/* kservice optimization */

#define RT_USING_DEBUG
#define RT_DEBUGING_COLOR
#define RT_DEBUGING_CONTEXT

/* Inter-Thread communication */

#define RT_USING_SEMAPHORE
#define RT_USING_MUTEX
#define RT_USING_EVENT
#define RT_USING_MAILBOX
#define RT_USING_MESSAGEQUEUE

/* Memory Management */

#define RT_USING_MEMPOOL
#define RT_USING_MEMHEAP
#define RT_MEMHEAP_FAST_MODE
#define RT_USING_MEMHEAP_AS_HEAP
#define RT_USING_MEMHEAP_AUTO_BINDING
#define RT_USING_HEAP

/* Kernel Device Object */

#define RT_USING_DEVICE
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 128
#define RT_CONSOLE_DEVICE_NAME "uart9"
#define RT_VER_NUM 0x50002
#define RT_USING_HW_ATOMIC
#define RT_USING_CPU_FFS
#define ARCH_ARM
#define ARCH_ARM_CORTEX_M
#define ARCH_ARM_CORTEX_M85

/* RT-Thread Components */

#define RT_USING_COMPONENTS_INIT
#define RT_USING_USER_MAIN
#define RT_MAIN_THREAD_STACK_SIZE 2048
#define RT_MAIN_THREAD_PRIORITY 10
#define RT_USING_MSH
#define RT_USING_FINSH
#define FINSH_USING_MSH
#define FINSH_THREAD_NAME "tshell"
#define FINSH_THREAD_PRIORITY 20
#define FINSH_THREAD_STACK_SIZE 4096
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES 5
#define FINSH_USING_SYMTAB
#define FINSH_CMD_SIZE 80
#define MSH_USING_BUILT_IN_COMMANDS
#define FINSH_USING_DESCRIPTION
#define FINSH_ARG_MAX 10
#define FINSH_USING_OPTION_COMPLETION

/* DFS: device virtual file system */


/* Device Drivers */

#define RT_USING_DEVICE_IPC
#define RT_UNAMED_PIPE_NUMBER 64
#define RT_USING_SERIAL
#define RT_USING_SERIAL_V2
#define RT_SERIAL_USING_DMA
#define RT_USING_PIN

/* Using USB */

#define RT_USING_USB

/* C/C++ and POSIX layer */

/* ISO-ANSI C layer */

/* Timezone and Daylight Saving Time */

#define RT_LIBC_USING_LIGHT_TZ_DST
#define RT_LIBC_TZ_DEFAULT_HOUR 8
#define RT_LIBC_TZ_DEFAULT_MIN 0
#define RT_LIBC_TZ_DEFAULT_SEC 0

/* POSIX (Portable Operating System Interface) layer */


/* Interprocess Communication (IPC) */


/* Socket is in the 'Network' category */


/* Network */


/* Utilities */


/* RT-Thread Utestcases */


/* RT-Thread online packages */

/* IoT - internet of things */


/* Wi-Fi */

/* Marvell WiFi */


/* Wiced WiFi */


/* CYW43012 WiFi */


/* BL808 WiFi */


/* CYW43439 WiFi */


/* IoT Cloud */


/* security packages */


/* language packages */

/* JSON: JavaScript Object Notation, a lightweight data-interchange format */


/* XML: Extensible Markup Language */


/* multimedia packages */

/* LVGL: powerful and easy-to-use embedded GUI library */


/* u8g2: a monochrome graphic library */


/* tools packages */


/* system packages */

/* enhanced kernel services */


/* acceleration: Assembly language or algorithmic acceleration packages */


/* CMSIS: ARM Cortex-M Microcontroller Software Interface Standard */


/* Micrium: Micrium software products porting for RT-Thread */

#define PKG_USING_TINYUSB
#define PKG_TINYUSB_STACK_SIZE 2048
#define PKG_TINYUSB_THREAD_PRIORITY 8
#define PKG_TINYUSB_MEM_SECTION ".data"
#define PKG_TINYUSB_MEM_ALIGN 4
#define PKG_TINYUSB_RHPORT_NUM_1
#define PKG_TINYUSB_RHPORT_NUM 0
#define PKG_TINYUSB_FULL_SPEED
#define PKG_TINYUSB_DEVICE_PORT_SPEED 0x00
#define PKG_TINYUSB_DEVICE_ENABLE
#define PKG_TINYUSB_DEVICE_VID 0xCAFE
#define PKG_TINYUSB_DEVICE_PID 0x4000
#define PKG_TINYUSB_DEVICE_MANUFACTURER "TinyUSB"
#define PKG_TINYUSB_DEVICE_PRODUCT "TinyUSB Device"
#define PKG_TINYUSB_EDPT0_SIZE 64
#define PKG_TINYUSB_DEVICE_CURRENT 250
#define PKG_TINYUSB_DEVICE_CDC
#define CFG_TUD_CDC 1
#define PKG_TINYUSB_DEVICE_CDC_STRING "TinyUSB CDC"
#define PKG_TINYUSB_DEVICE_CDC_EPNUM_NOTIF 1
#define PKG_TINYUSB_DEVICE_CDC_EPNUM 2
#define PKG_TINYUSB_DEVICE_CDC_RX_BUFSIZE 64
#define PKG_TINYUSB_DEVICE_CDC_TX_BUFSIZE 64
#define PKG_TINYUSB_DEVICE_EXAMPLE
#define PKG_TINYUSB_DEVICE_EXAMPLE_CDC
#define PKG_TINYUSB_DEBUG_ERROR
#define CFG_TUSB_DEBUG 1
#define PKG_USING_TINYUSB_V01401

/* peripheral libraries and drivers */

/* HAL & SDK Drivers */

/* STM32 HAL & SDK Drivers */


/* Kendryte SDK */


/* sensors drivers */


/* touch drivers */


/* AI packages */


/* Signal Processing and Control Algorithm Packages */


/* miscellaneous packages */

/* project laboratory */

/* samples: kernel and components samples */


/* entertainment: terminal games and other interesting software packages */


/* Arduino libraries */


/* Projects and Demos */


/* Sensors */


/* Display */


/* Timing */


/* Data Processing */


/* Data Storage */

/* Communication */


/* Device Control */


/* Other */


/* Signal IO */


/* Uncategorized */

#define SOC_FAMILY_RENESAS
#define SOC_SERIES_R7FA8M85

/* Hardware Drivers Config */

#define SOC_R7FA8D1AH

/* On-chip Peripheral Drivers */

#define BSP_USING_GPIO
#define BSP_USING_UART
#define BSP_USING_UART9
#define BSP_UART9_RX_BUFSIZE 256
#define BSP_UART9_TX_BUFSIZE 0
#define BSP_USING_SDRAM
#define BSP_USING_SDRAM_SIZE 0x2000000

#endif
