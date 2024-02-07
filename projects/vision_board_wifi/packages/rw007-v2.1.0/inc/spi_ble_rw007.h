#ifndef __SPI_BLE_RW007__
#define __SPI_BLE_RW007__

#include "rtthread.h"
#include <string.h>
#include <stdint.h>

#include "spi_ble_nimble.h"

#define BLE_MAC_LEN                 6
#define BLE_MAX_DATA_LEN            1520

#define member_offset(type, member) ((unsigned long)(&((type *)0)->member))

typedef enum
{
    RW007_BLE_RESP_TYPE_CMD,
    RW007_BLE_RESP_TYPE_NOTIFY,
}rw007_ble_resp_type_t;

typedef enum
{
    RW007_BLE_REQ_TYPE_INIT = 1,
    RW007_BLE_REQ_TYPE_ADDR_GET,
    RW007_BLE_REQ_TYPE_CONN_INFO_GET,
    RW007_BLE_REQ_TYPE_CONN_UPD_PARAMS,
    RW007_BLE_REQ_TYPE_SCAN,
    RW007_BLE_REQ_TYPE_STOP_SCAN,
    RW007_BLE_REQ_TYPE_CONNECT,
    RW007_BLE_REQ_TYPE_DISCONN,
    RW007_BLE_REQ_TYPE_GATT_DIS_FULL,
    RW007_BLE_REQ_TYPE_MTU_EXCHANGE,
    RW007_BLE_REQ_TYPE_NOTIFYCHANGE,
    RW007_BLE_REQ_TYPE_WRITE,
    RW007_BLE_REQ_TYPE_READ,
    RW007_BLE_REQ_TYPE_NOTIFYCHANGE_BY_UUID,
    RW007_BLE_REQ_TYPE_WRITE_BY_UUID,
    RW007_BLE_REQ_TYPE_READ_BY_UUID,
} rw007_ble_req_type_t;

typedef enum
{
    RW007_BLE_RSP_TYPE_INIT = 0x50,
    RW007_BLE_RSP_TYPE_ADDR_GET,
    RW007_BLE_RSP_TYPE_CONN_INFO_GET,
    RW007_BLE_RSP_TYPE_CONN_UPD_PARAMS,
    RW007_BLE_RSP_TYPE_SCAN,
    RW007_BLE_RSP_TYPE_CONNECT,
    RW007_BLE_RSP_TYPE_DISCONN,
    RW007_BLE_RSP_TYPE_GATT_DIS_FULL,
    RW007_BLE_RSP_TYPE_GATTC_SRV,
    RW007_BLE_RSP_TYPE_GATTC_CHR,
    RW007_BLE_RSP_TYPE_GATTC_DSC,
    RW007_BLE_RSP_TYPE_MTU_EXCHANGE,
    RW007_BLE_RSP_TYPE_NOTIFY,
    RW007_BLE_RSP_TYPE_WRITE,
    RW007_BLE_RSP_TYPE_READ,
    RW007_BLE_RSP_TYPE_MAX,
} rw007_ble_rsp_type_t;

typedef enum
{
    RW007_BLE_NTF_TYPE_CONN_UPD = 0xA0,
    RW007_BLE_NTF_TYPE_CONN_UPD_PARAMS,
    RW007_BLE_NTF_TYPE_DISC,
    RW007_BLE_NTF_TYPE_CONNECT,
    RW007_BLE_NTF_TYPE_DISCONN,
    RW007_BLE_NTF_TYPE_MTU_EXCHANGE,
    RW007_BLE_NTF_TYPE_DISC_COMPLETE,
    RW007_BLE_NTF_TYPE_ADV_COMPLETE,
    RW007_BLE_NTF_TYPE_SUBSCRIBE,
    RW007_BLE_NTF_TYPE_NOTIFY_RX,
    RW007_BLE_NTF_TYPE_MAX
} rw007_ble_ntf_type_t;
#define RW007_BLE_INIT_ROLE_PERIPHERAL      1
#define RW007_BLE_INIT_ROLE_CENTRAL         2

/* Bluetooth Device address type */
#define RW007_BLE_ADDR_PUBLIC      (0x00)
#define RW007_BLE_ADDR_RANDOM      (0x01)
#define RW007_BLE_ADDR_PUBLIC_ID   (0x02)
#define RW007_BLE_ADDR_RANDOM_ID   (0x03)

/** Type of UUID */
enum {
    /** 16-bit UUID (BT SIG assigned) */
    BLE_UUID_TYPE_16 = 16,
    /** 32-bit UUID (BT SIG assigned) */
    BLE_UUID_TYPE_32 = 32,
    /** 128-bit UUID */
    BLE_UUID_TYPE_128 = 128,
};

typedef struct
{
    uint8_t type;
    uint8_t reserved;
    uint16_t reserved2;
    union {
        uint16_t uuid16;
        uint32_t uuid32;
        uint8_t uuid128[16];
    } uuid;
} rw007_ble_uuid_t;

typedef struct
{
    uint8_t type;
    uint8_t val[6];
} rw007_ble_addr_t;

typedef struct
{
    uint16_t start_handle;
    uint16_t end_handle;
    rw007_ble_uuid_t uuid;
} rw007_ble_svc_t;

typedef struct
{
    uint16_t def_handle;
    uint16_t val_handle;
    uint8_t properties;
    uint8_t reserved;
    uint16_t reserved2;
    rw007_ble_uuid_t uuid;
} rw007_ble_chr_t;

typedef struct
{
    uint16_t handle;
    uint16_t reserved;
    rw007_ble_uuid_t uuid;
} rw007_ble_dsc_t;

typedef struct
{
    uint16_t conn_handle;
    uint16_t reserved;
    rw007_ble_svc_t svc;
} rw007_ble_svc_rsp_t;

typedef struct
{
    uint16_t conn_handle;
    uint16_t attr_handle;
    rw007_ble_chr_t chr;
} rw007_ble_chr_rsp_t;

typedef struct
{
    uint16_t conn_handle;
    uint16_t attr_handle;
    rw007_ble_dsc_t dsc;
} rw007_ble_dsc_rsp_t;

typedef struct
{
    uint16_t conn_handle;
    uint16_t attr_handle;
    rw007_ble_uuid_t uuid;
    uint16_t status;
    uint16_t reserved;
} rw007_ble_write_rsp_t;

typedef struct
{
    uint16_t conn_handle;
    uint16_t attr_handle;
    rw007_ble_uuid_t uuid;
    uint16_t status;
    uint16_t length;
    // NOTE: packed below data
} rw007_ble_read_rsp_t;

typedef struct
{
    uint16_t conn_handle;
    uint16_t atrr_handle;
    rw007_ble_uuid_t uuid;
    uint16_t status;
    uint16_t reserved;
} rw007_ble_notifychange_rsp_t;

struct rw007_ble_write_by_uuid
{
    rt_uint16_t conn_handle;
    rt_uint16_t data_len;
    rw007_ble_uuid_t uuid;
    rt_uint8_t data[];
};

struct rw007_ble_read_by_uuid
{
    uint16_t conn_handle;
    uint16_t reserved;
    rw007_ble_uuid_t uuid;
};

struct rw007_ble_get_addr
{
    rt_uint8_t public_id_addr[6];
    rt_uint8_t random_id_addr[6];
};

struct rw007_ble_notify_change_by_uuid
{
    rt_uint16_t conn_handle;
    rt_uint16_t char_value;
    rw007_ble_uuid_t uuid;
};

struct rw007_ble_scan_params
{
    rt_uint8_t limited; // 1/0
    rt_uint8_t passive; // 1/0
    rt_uint8_t filter; // 0 disable /1 wl /2 inta /3 1+2
    rt_uint32_t duration;
    rt_uint16_t interval;
    rt_uint16_t window;
};

struct rw007_ble_req
{
    rt_uint16_t req_type;
    rt_uint16_t len;
    union
    {
        rt_uint32_t int_value;
        ble_addr_t addr;
        struct rw007_ble_get_addr get_addr;
        struct rw007_ble_notify_change_by_uuid notify_change;
        rt_uint8_t bytes[BLE_MAX_DATA_LEN];
    } value;
};

/**
 * below is rw007 upload(notify response) packet type
*/

struct rw007_ble_conn_desc
{
     /** If connection is encrypted */
    uint8_t encrypted:1;

    /** If connection is authenticated */
    uint8_t authenticated:1;

    /** If connection is bonded (security information is stored)  */
    uint8_t bonded:1;

    /** Size of a key used for encryption */
    uint8_t key_size:5;

    /** Connection Role
     * Possible values BLE_GAP_ROLE_SLAVE or BLE_GAP_ROLE_MASTER
     */
    uint8_t role;

    /** Connection handle */
    uint16_t conn_handle;

    /** Connection interval */
    uint16_t conn_itvl;

    /** Connection latency */
    uint16_t conn_latency;

    /** Connection supervision timeout */
    uint16_t supervision_timeout;

    uint8_t our_id_type;
    uint8_t our_ota_type;
    uint8_t peer_id_type;
    uint8_t peer_ota_type;
    uint8_t our_id_addr[6];
    uint8_t our_ota_addr[6];
    uint8_t peer_id_addr[6];
    uint8_t peer_ota_addr[6];
};

struct rw007_ble_gap_event_connect
{
    uint8_t event_type;
    uint8_t reserved;

    uint16_t conn_handle;
    int32_t status;
    // NOTE: below conn_disc
    // struct rw007_ble_conn_desc conn_disc;
};

struct rw007_ble_gap_event_disconnect
{
    uint8_t event_type;
    uint8_t reserved;
    uint16_t reserved2;

    int32_t reason;

    // NOTE: below conn_disc
    // struct rw007_ble_conn_desc conn_disc;
};

struct rw007_ble_gap_event_discov
{
    uint8_t event_type;
    uint8_t discov_type;
    uint8_t length_data;
    int8_t rssi;

    uint8_t addr_type;
    uint8_t direct_type;

    uint8_t addr[6];

    /***
     * LE direct advertising report fields; direct_addr is BLE_ADDR_ANY if
     * direct address fields are not present.
     */
    uint8_t direct_addr[6];

    uint16_t reserved;
    // NOTE: below fields are buffer data fields
};

struct rw007_ble_gap_event_conn_upd
{
    uint8_t event_type;
    uint8_t reserved;
    uint16_t conn_handle;

    int32_t status;

    // NOTE: below conn_disc
    // struct rw007_ble_conn_desc conn_disc;
};

struct rw007_ble_gap_event_discov_complete
{
    uint8_t event_type;
    uint8_t reserved;
    uint8_t reserved2;

    int32_t reason;
};

struct rw007_ble_gap_event_adv_complete
{
    uint8_t event_type;
    uint8_t reserved;
    uint8_t reserved2;

    int32_t reason;
};

struct rw007_ble_gap_event_notify_rx
{
    uint8_t event_type;
    /**
     * Whether the received command is a notification or an
     * indication;
     *     o 0: Notification;
     *     o 1: Indication.
     */
    uint8_t indication;

    /** The handle of the relevant ATT attribute. */
    uint16_t attr_handle;

    /** The handle of the relevant connection. */
    uint16_t conn_handle;

    uint16_t length_data;
    // NOTE: below is the buffer data
};

struct rw007_ble_gap_event_subscribe
{
    uint8_t event_type;
    uint8_t reserved;

    /** The handle of the relevant connection. */
    uint16_t conn_handle;

    /** The value handle of the relevant characteristic. */
    uint16_t attr_handle;

    /** One of the BLE_GAP_SUBSCRIBE_REASON codes. */
    uint8_t reason;

    /** Whether the peer was previously subscribed to notifications. */
    uint8_t prev_notify:1;

    /** Whether the peer is currently subscribed to notifications. */
    uint8_t cur_notify:1;

    /** Whether the peer was previously subscribed to indications. */
    uint8_t prev_indicate:1;

    /** Whether the peer is currently subscribed to indications. */
    uint8_t cur_indicate:1;
    uint8_t reserved4:4;
};

struct rw007_ble_gap_event_mtu_update
{
    uint8_t event_type;
    uint8_t reserved;

    /** The handle of the relevant connection. */
    uint16_t conn_handle;

    /**
     * Indicates the channel whose MTU has been updated; either
     * BLE_L2CAP_CID_ATT or the ID of a connection-oriented channel.
     */
    uint16_t channel_id;

    /* The channel's new MTU. */
    uint16_t value;
};

struct rw007_ble_resp
{
    rt_uint16_t resp_type;    // rw007_BLE_RESP_TYPE_CMD / RW007_BLE_RESP_TYPE_NOTIFY
    rt_uint16_t cmd;
    rt_uint16_t len;
    rt_uint16_t result;
    rt_uint8_t data[BLE_MAX_DATA_LEN];
};

typedef void (*rw007_ble_resp_handle_cb)(rt_uint16_t resp_type, void *data, rt_uint16_t size);
typedef void (*rw007_ble_ntf_handle_cb)(rt_uint16_t ntf_type, void *data, rt_uint16_t size);

void rw007_ble_resp_handle_cb_reg(rw007_ble_resp_handle_cb resp_cb);
void rw007_ble_ntf_handle_cb_reg(rw007_ble_ntf_handle_cb notify_cb);

int rw007_ble_init(rt_uint8_t roles);
int rw007_ble_addr_get(void);
int rw007_ble_conn_info_get(rt_uint16_t conn_handle);
int rw007_ble_conn_update_params(rt_uint16_t conn_handle, struct ble_gap_upd_params *params);
int rw007_ble_central_init(void);
int rw007_ble_scan(struct rw007_ble_scan_params *scan_param);
int rw007_ble_scan_stop(void);
int rw007_ble_connect(ble_addr_t *addr);
int rw007_ble_disconnect(rt_uint16_t conn_handle);
int rw007_ble_gattc_service_get(rt_uint16_t conn_handle);
int rw007_ble_gattc_characteristic_get(rt_uint16_t conn_handle, rt_uint16_t attr_handle);
int rw007_ble_gattc_discriptor_get(rt_uint16_t conn_handle, rt_uint16_t attr_handle);
int rw007_ble_gattc_service_discover(rt_uint16_t conn_handle);
int rw007_ble_mtu_exchange(rt_uint16_t conn_handle);
int rw007_ble_gatt_notify_change(rt_uint16_t attr_handle);
int rw007_ble_gattc_write(rt_uint16_t conn_handle, rt_uint16_t attr_handle, const rt_uint8_t *buf, rt_uint16_t len);
int rw007_ble_gattc_read(rt_uint16_t conn_handle, rt_uint16_t attr_handle);
int rw007_ble_gatt_notify_change_by_uuid(rt_uint16_t conn_handle, const rw007_ble_uuid_t *uuid, uint16_t value);
int rw007_ble_gattc_write_by_uuid(rt_uint16_t conn_handle, const rw007_ble_uuid_t *uuid, const rt_uint8_t *buf, rt_uint16_t len);
int rw007_ble_gattc_read_by_uuid(rt_uint16_t conn_handle, const rw007_ble_uuid_t *uuid);

static inline void _hexstrtoaddr(char* hexstr, rt_uint8_t addr[])
{
    int i, len, pos;
    rt_uint8_t hexbyte = 0;

    len = strlen(hexstr) + 1;
    pos = 5;

    for (i = 0; i < len; i++)
    {
        if (hexstr[i] >= '0' && hexstr[i] <= '9')
        {
            hexbyte <<= 4;
            hexbyte |= hexstr[i] - '0';
        }
        else if (hexstr[i] >= 'a' && hexstr[i] <= 'f')
        {
            hexbyte <<= 4;
            hexbyte |= hexstr[i] - 'a' + 10;
        }
        else if (hexstr[i] >= 'A' && hexstr[i] <= 'F')
        {
            hexbyte <<= 4;
            hexbyte |= hexstr[i] - 'A' + 10;
        }
        else
        {
            addr[pos--] = hexbyte;
            if (pos < 0) break;
            hexbyte = 0;
        }
    }
}

#define __is_print(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')
rt_inline void hex_dump(const rt_uint8_t *ptr, rt_size_t buflen)
{
    unsigned char *buf = (unsigned char *)ptr;
    int i, j;

    RT_ASSERT(ptr != RT_NULL);

    for (i = 0; i < buflen; i += 16)
    {
        rt_kprintf("%08X: ", i);

        for (j = 0; j < 16; j++)
            if (i + j < buflen)
                rt_kprintf("%02X ", buf[i + j]);
            else
                rt_kprintf("   ");
        rt_kprintf(" ");

        for (j = 0; j < 16; j++)
            if (i + j < buflen)
                rt_kprintf("%c", __is_print(buf[i + j]) ? buf[i + j] : '.');
        rt_kprintf("\n");
    }
}

#endif
