/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */

#include "tusb_option.h"

#if (CFG_TUH_ENABLED && CFG_TUH_VENDOR)

//--------------------------------------------------------------------+
// INCLUDE
//--------------------------------------------------------------------+
#include "host/usbh.h"
#include "host/usbh_pvt.h"
#include "vendor_host.h"

#ifndef CFG_TUH_CUSTOM_LOG_LEVEL
  #define CFG_TUH_CUSTOM_LOG_LEVEL   CFG_TUH_LOG_LEVEL
#endif

#define TU_LOG_DRV(...)   TU_LOG(CFG_TUH_CUSTOM_LOG_LEVEL, __VA_ARGS__)

#ifndef BIT
#define BIT(x)            (1u<<x)
#endif

typedef struct {
  uint8_t daddr;

  uint8_t itf_num;
  uint8_t ep_in;
  uint8_t ep_out;

  bool mounted;           // Enumeration is complete

  uint16_t epin_size;
  uint16_t epout_size;

  CFG_TUH_MEM_ALIGN uint8_t epin_buf[64];
  CFG_TUH_MEM_ALIGN uint8_t epout_buf[64];
} xpad_interface_info_t;

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF
//--------------------------------------------------------------------+
#define GIP_PL_LEN(N)     (N)
#define GIP_SEQ0          (0x00)
#define GIP_PWR_ON        (0x00)

enum gip_command_core {
	GIP_CMD_ACKNOWLEDGE = 0x01,
	GIP_CMD_ANNOUNCE = 0x02,
	GIP_CMD_STATUS = 0x03,
	GIP_CMD_IDENTIFY = 0x04,
	GIP_CMD_POWER = 0x05,
	GIP_CMD_AUTHENTICATE = 0x06,
	GIP_CMD_VIRTUAL_KEY = 0x07,
	GIP_CMD_AUDIO_CONTROL = 0x08,
	GIP_CMD_LED = 0x0a,
	GIP_CMD_HID_REPORT = 0x0b,
	GIP_CMD_FIRMWARE = 0x0c,
	GIP_CMD_SERIAL_NUMBER = 0x1e,
	GIP_CMD_AUDIO_SAMPLES = 0x60,
};

enum gip_command_client {
	GIP_CMD_RUMBLE = 0x09,
	GIP_CMD_INPUT = 0x20,
};

enum gip_option {
	GIP_OPT_ACKNOWLEDGE = BIT(4),
	GIP_OPT_INTERNAL = BIT(5),
	GIP_OPT_CHUNK_START = BIT(6),
	GIP_OPT_CHUNK = BIT(7),
};

enum gip_audio_control {
	GIP_AUD_CTRL_VOLUME_CHAT = 0x00,
	GIP_AUD_CTRL_FORMAT_CHAT = 0x01,
	GIP_AUD_CTRL_FORMAT = 0x02,
	GIP_AUD_CTRL_VOLUME = 0x03,
};

enum gip_audio_volume_mute {
	GIP_AUD_VOLUME_UNMUTED = 0x04,
	GIP_AUD_VOLUME_MIC_MUTED = 0x05,
};

//--------------------------------------------------------------------+
// INTERNAL OBJECT & FUNCTION DECLARATION
//--------------------------------------------------------------------+

/*
 * This packet is required for all Xbox One pads with 2015
 * or later firmware installed (or present from the factory).
 */
static const uint8_t xboxone_power_on[] = {
	GIP_CMD_POWER, GIP_OPT_INTERNAL, GIP_SEQ0, GIP_PL_LEN(1), GIP_PWR_ON
};

CFG_TUH_MEM_SECTION
tu_static xpad_interface_info_t xpad_itf[CFG_TUH_VENDOR];

//--------------------------------------------------------------------+
// Helper
//--------------------------------------------------------------------+
TU_ATTR_ALWAYS_INLINE static inline xpad_interface_info_t* get_xpad_itf(uint8_t daddr, uint8_t idx) {
  TU_ASSERT(daddr > 0 && idx < CFG_TUH_VENDOR, NULL);
  xpad_interface_info_t* p_xpad = &xpad_itf[idx];
  return (p_xpad->daddr == daddr) ? p_xpad : NULL;
}

// Get instance ID by endpoint address
static uint8_t get_idx_by_epaddr(uint8_t daddr, uint8_t ep_addr) {
  for (uint8_t idx = 0; idx < CFG_TUH_VENDOR; idx++) {
    xpad_interface_info_t const* p_xpad = &xpad_itf[idx];
    if (p_xpad->daddr == daddr &&
        (p_xpad->ep_in == ep_addr || p_xpad->ep_out == ep_addr)) {
      return idx;
    }
  }
  return TUSB_INDEX_INVALID_8;
}

static xpad_interface_info_t* find_new_itf(void) {
  for (uint8_t i = 0; i < CFG_TUH_VENDOR; i++) {
    if (xpad_itf[i].daddr == 0) return &xpad_itf[i];
  }
  return NULL;
}

//--------------------------------------------------------------------+
// Interface API
//--------------------------------------------------------------------+
uint8_t tuh_xpad_itf_get_count(uint8_t daddr) {
  uint8_t count = 0;
  for (uint8_t i = 0; i < CFG_TUH_VENDOR; i++) {
    if (xpad_itf[i].daddr == daddr) count++;
  }
  return count;
}

uint8_t tuh_xpad_itf_get_total_count(void) {
  uint8_t count = 0;
  for (uint8_t i = 0; i < CFG_TUH_VENDOR; i++) {
    if (xpad_itf[i].daddr != 0) count++;
  }
  return count;
}

bool tuh_xpad_mounted(uint8_t daddr, uint8_t idx) {
  xpad_interface_info_t* p_xpad = get_xpad_itf(daddr, idx);
  TU_VERIFY(p_xpad);
  return p_xpad->mounted;
}


//--------------------------------------------------------------------+
// APPLICATION API (need to check parameters)
//--------------------------------------------------------------------+

//--------------------------------------------------------------------+
// Interrupt Endpoint API
//--------------------------------------------------------------------+

// Check if HID interface is ready to receive report
bool tuh_xpad_receive_ready(uint8_t dev_addr, uint8_t idx) {
  xpad_interface_info_t* p_xpad = get_xpad_itf(dev_addr, idx);
  TU_VERIFY(p_xpad);
  return !usbh_edpt_busy(dev_addr, p_xpad->ep_in);
}

bool tuh_xpad_receive_report(uint8_t daddr, uint8_t idx) {
  xpad_interface_info_t* p_xpad = get_xpad_itf(daddr, idx);
  TU_VERIFY(p_xpad);

  // claim endpoint
  TU_VERIFY(usbh_edpt_claim(daddr, p_xpad->ep_in));

  if (!usbh_edpt_xfer(daddr, p_xpad->ep_in, p_xpad->epin_buf, p_xpad->epin_size)) {
    usbh_edpt_release(daddr, p_xpad->ep_in);
    return false;
  }

  return true;
}

bool tuh_xpad_receive_abort(uint8_t dev_addr, uint8_t idx) {
  xpad_interface_info_t* p_xpad = get_xpad_itf(dev_addr, idx);
  TU_VERIFY(p_xpad);
  return tuh_edpt_abort_xfer(dev_addr, p_xpad->ep_in);
}

//--------------------------------------------------------------------+
// USBH-CLASS API
//--------------------------------------------------------------------+
bool cush_init(void)
{
  tu_memclr(xpad_itf, sizeof(xpad_itf));
  return true;
}

bool cush_deinit(void)
{
  return true;
}

/*
09 02 77 00 03 01 00 a0 fa
desc_if
09 04 00 00 02 ff 47 d0 00
07 05 02 03 40 00 04
07 05 82 03 40 00 04
09 04 00 01 02 ff 47 d0 00
07 05 02 03 40 00 04
07 05 82 03 40 00 02
09 04 01 00 00 ff 47 d0 00
09 04 01 01 02 ff 47 d0 00
07 05 03 01 e4 00 01
07 05 83 01 40 00 01
09 04 02 00 00 ff 47 d0 00
09 04 02 01 02 ff 47 d0 00
07 05 04 02 40 00 00
07 05 84 02 40 00 00
*/
bool cush_open(uint8_t rhport, uint8_t daddr, tusb_desc_interface_t const* desc_itf, uint16_t max_len) {
  (void) rhport;
  (void) max_len;

  TU_VERIFY(255 == desc_itf->bInterfaceClass &&
            71 == desc_itf->bInterfaceSubClass &&
            208 == desc_itf->bInterfaceProtocol);
  TU_LOG_DRV("[%u] XBox opening Interface %u\r\n", daddr, desc_itf->bInterfaceNumber);
  uint16_t const drv_len = (uint16_t) (sizeof(tusb_desc_interface_t) + sizeof(tusb_desc_configuration_t) +
                                       desc_itf->bNumEndpoints * sizeof(tusb_desc_endpoint_t));
  TU_ASSERT(max_len >= drv_len);
  uint8_t const* p_desc = (uint8_t const*) desc_itf;

  xpad_interface_info_t* p_xpad = find_new_itf();
  TU_ASSERT(p_xpad); // not enough interface, try to increase CFG_TUH_VENDOR
  p_xpad->daddr = daddr;

  //------------- Endpoint Descriptors -------------//
  p_desc = tu_desc_next(p_desc);
  tusb_desc_endpoint_t const* desc_ep = (tusb_desc_endpoint_t const*) p_desc;

  for (int i = 0; i < desc_itf->bNumEndpoints; i++) {
    TU_ASSERT(TUSB_DESC_ENDPOINT == desc_ep->bDescriptorType);
    TU_ASSERT(tuh_edpt_open(daddr, desc_ep));

    if (tu_edpt_dir(desc_ep->bEndpointAddress) == TUSB_DIR_IN) {
      p_xpad->ep_in = desc_ep->bEndpointAddress;
      p_xpad->epin_size = tu_edpt_packet_size(desc_ep);
    } else {
      p_xpad->ep_out = desc_ep->bEndpointAddress;
      p_xpad->epout_size = tu_edpt_packet_size(desc_ep);
    }

    p_desc = tu_desc_next(p_desc);
    desc_ep = (tusb_desc_endpoint_t const*) p_desc;
  }

  p_xpad->itf_num = desc_itf->bInterfaceNumber;

  return true;
}

static bool send_xpad_power_on(xpad_interface_info_t* const xpad_itf)
{
  // claim endpoint
  TU_VERIFY(usbh_edpt_claim(xpad_itf->daddr, xpad_itf->ep_out));
  memcpy(xpad_itf->epout_buf, xboxone_power_on, sizeof(xboxone_power_on));

  if (!usbh_edpt_xfer(xpad_itf->daddr, xpad_itf->ep_out, &xpad_itf->epout_buf[0], sizeof(xboxone_power_on))) {
    usbh_edpt_release(xpad_itf->daddr, xpad_itf->ep_out);
    return false;
  }
  return true;
}

static void xpad_setconfig_comp(tuh_xfer_t* xfer) {
  uint8_t const dev_addr = xfer->daddr;
  uint8_t const itf_num = (uint8_t)tu_le16toh(tu_le16toh(xfer->setup->wIndex));
  xpad_interface_info_t* const xpad_itf = get_xpad_itf(dev_addr, itf_num);
  xpad_itf->mounted = true;
  // enumeration is complete
  send_xpad_power_on(xpad_itf);

  if (tuh_xpad_mount_cb) tuh_xpad_mount_cb(dev_addr, itf_num);
  usbh_driver_set_config_complete(dev_addr, itf_num);
}

bool cush_set_config(uint8_t dev_addr, uint8_t itf_num)
{
  if (itf_num != 0) {
    return true;
  }

  xpad_interface_info_t const* xpad_itf = get_xpad_itf(dev_addr, itf_num);
  tusb_control_request_t const request = {
      .bmRequestType_bit = {
          .recipient = TUSB_REQ_RCPT_DEVICE,
          .type      = TUSB_REQ_TYPE_STANDARD,
          .direction = TUSB_DIR_OUT
      },
      .bRequest = TUSB_REQ_SET_CONFIGURATION,
      .wValue   = 1,
      .wIndex   = tu_htole16((uint16_t) xpad_itf->itf_num),
      .wLength  = 0,
  };

  tuh_xfer_t xfer = {
      .daddr       = dev_addr,
      .ep_addr     = 0,
      .setup       = &request,
      .buffer      = NULL,
      .complete_cb = xpad_setconfig_comp,
      .user_data   = 0
  };

  tuh_control_xfer(&xfer);
  return true;
}

bool cush_xfer_cb(uint8_t dev_addr, uint8_t pipe_hdl, xfer_result_t result, uint32_t xferred_bytes)
{
  (void) result;
  uint8_t const dir = tu_edpt_dir(pipe_hdl);
  uint8_t const idx = get_idx_by_epaddr(dev_addr, pipe_hdl);
  xpad_interface_info_t* const xpad_itf = get_xpad_itf(dev_addr, idx);
  TU_VERIFY(xpad_itf);
  if (dir == TUSB_DIR_IN) {
    tuh_xpad_report_received_cb(dev_addr, idx, xpad_itf->epin_buf, (uint16_t) xferred_bytes);
  }
  return true;
}

void cush_close(uint8_t dev_addr)
{
  for (uint8_t i = 0; i < CFG_TUH_VENDOR; i++) {
    xpad_interface_info_t* p_xpad = &xpad_itf[i];
    if (p_xpad->daddr == dev_addr) {
      TU_LOG_DRV("  xpadh close addr = %u index = %u\r\n", dev_addr, i);
      if (tuh_xpad_umount_cb) tuh_xpad_umount_cb(dev_addr, i);
      tu_memclr(p_xpad, sizeof(xpad_interface_info_t));
    }
  }
}

#endif
