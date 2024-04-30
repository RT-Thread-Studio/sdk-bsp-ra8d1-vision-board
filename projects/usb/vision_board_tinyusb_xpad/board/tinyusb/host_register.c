#include <tusb_option.h>

#if (TUSB_OPT_HOST_ENABLED)

#include "host/usbh.h"
#include "host/usbh_pvt.h"
#include "host_xpad.h"

// register xpad host
usbh_class_driver_t const* usbh_app_driver_get_cb(uint8_t* driver_count)
{
  static usbh_class_driver_t host_xpad = {
  #if CFG_TUSB_DEBUG >= 2
    .name = "XPADH",
  #else
    .name = "",
  #endif
    .init=xpad_init,
    .open=xpad_open,
    .set_config=xpad_set_config,
    .xfer_cb = xpad_xfer_cb,
    .close = xpad_close
  };
  *driver_count = 1;
  return &host_xpad;
}
#endif
