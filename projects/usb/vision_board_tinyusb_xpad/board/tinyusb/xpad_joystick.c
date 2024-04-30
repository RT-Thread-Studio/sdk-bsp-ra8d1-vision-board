#include <stdint.h>
#include <rtthread.h>
#include <tusb.h>
#include "host_xpad.h"

#define LOG_TAG       "xpad"
#define DBG_LVL       DBG_LOG
#include <rtdbg.h>

static bool xpad_mounted = false;
static uint8_t xpad_dev_addr = 0;
static uint8_t xpad_instance = 0;
static uint8_t motor_left = 0;
static uint8_t motor_right = 0;
#define PACKET_HEADER_INPUT    (0x20)
#ifndef BIT
#define BIT(x)  (1U<<(x))
#endif

static inline bool is_xpad(uint8_t dev_addr)
{
  uint16_t vid, pid;
  tuh_vid_pid_get(dev_addr, &vid, &pid);

  return ( (vid == 0x045e && pid == 0x0b12) );
}

// Invoked when device with hid interface is mounted
// Report descriptor is also available for use. tuh_hid_parse_report_descriptor()
// can be used to parse common/simple enough descriptor.
// Note: if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE, it will be skipped
// therefore report_desc = NULL, desc_len = 0
void tuh_xpad_mount_cb(uint8_t dev_addr, uint8_t idx)
{
  uint16_t vid, pid;
  tuh_vid_pid_get(dev_addr, &vid, &pid);

  LOG_D("XPAD device address = %d, instance = %d is mounted", dev_addr, idx);
  LOG_D("VID = %04x, PID = %04x", vid, pid);
  //
  if ( is_xpad(dev_addr) )
  {
    if (!xpad_mounted)
    {
      xpad_dev_addr = dev_addr;
      xpad_instance = idx;
      motor_left = 0;
      motor_right = 0;
      xpad_mounted = true;
    }
    // request to receive report
    // tuh_hid_report_received_cb() will be invoked when report is available
    if ( !tuh_xpad_receive_report(dev_addr, idx) )
    {
      LOG_E("cannot request to receive report");
    }
  }
}

// Invoked when device with hid interface is un-mounted
void tuh_xpad_umount_cb(uint8_t dev_addr, uint8_t idx)
{
  LOG_D("XPad device address = %d, interface = %d is unmounted", dev_addr, idx);
  if (xpad_mounted && xpad_dev_addr == dev_addr)
  {
    xpad_mounted = false;
  }
}


void process_xpad(uint8_t const* report, uint16_t len)
{
  if (report[0] == PACKET_HEADER_INPUT) {
     // BTN_START
     if (report[4] & BIT(2)) {
       LOG_D("BTN_START press");
     }
     if (report[4] & BIT(3)) {
       LOG_D("BTN_SELECT press");
     }
     if (report[4] & BIT(4)) {
       LOG_D("BTN_A press");
     }
     if (report[4] & BIT(5)) {
       LOG_D("BTN_B press");
     }
     if (report[4] & BIT(6)) {
       LOG_D("BTN_X press");
     }
     if (report[4] & BIT(7)) {
       LOG_D("BTN_Y press");
     }
     if (report[5] & BIT(0)) {
       LOG_D("DPAD_UP press");
     }
     if (report[5] & BIT(1)) {
       LOG_D("DPAD_DOWN press");
     }
     if (report[5] & BIT(2)) {
       LOG_D("DPAD_LEFT press");
     }
     if (report[5] & BIT(3)) {
       LOG_D("DPAD_RIGHT press");
     }
     if (report[5] & BIT(4)) {
       LOG_D("BTN_TL press");
     }
     if (report[5] & BIT(5)) {
       LOG_D("BTN_TR press");
     }

     const uint16_t tl = report[6] + report[7] * 256;
     if (tl > 0) {
       LOG_D("BTN_TH_L: %d press", tl);
     }
     const uint16_t tr = report[8] + report[9] * 256;
     if (tr > 0) {
       LOG_D("BTN_TH_R: %d press", tr);
     }
  }
}

void tuh_xpad_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
  if ( is_xpad(dev_addr) )
  {
    process_xpad(report, len);
  }

  // continue to request to receive report
  if ( !tuh_xpad_receive_report(dev_addr, instance) )
  {
    LOG_E("Error: cannot request to receive report\r\n");
  }
}
