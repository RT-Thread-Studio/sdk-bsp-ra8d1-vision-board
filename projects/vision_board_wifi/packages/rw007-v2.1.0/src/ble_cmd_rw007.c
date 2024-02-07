#include "spi_ble_rw007.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct ble_cmd_des
{
    const char *cmd;
    int (*fun)(int argc, char *argv[]);
};

static int ble_cmd_help(int argc, char *argv[]);

static int ble_cmd_init(int argc, char *argv[]);
static int ble_cmd_get_addr(int argc, char *argv[]);
static int ble_cmd_get_connect_info(int argc, char *argv[]);
static int ble_cmd_update_params(int argc, char *argv[]);
static int ble_cmd_scan(int argc, char *argv[]);
static int ble_cmd_stop_scan(int argc, char *argv[]);
static int ble_cmd_connect(int argc, char *argv[]);
static int ble_cmd_disconnect(int argc, char *argv[]);
static int ble_cmd_gattc_service_get(int argc, char *argv[]);
static int ble_cmd_gattc_characteristic_get(int argc, char *argv[]);
static int ble_cmd_gattc_discriptor_get(int argc, char *argv[]);
static int ble_cmd_mtu_exchange(int argc, char *argv[]);
static int ble_cmd_gatt_notify_change(int argc, char *argv[]);
static int ble_cmd_gattc_write(int argc, char *argv[]);
static int ble_cmd_gattc_read(int argc, char *argv[]);
static int ble_cmd_gatt_notify_change_by_uuid(int argc, char *argv[]);
static int ble_cmd_gattc_write_by_uuid(int argc, char *argv[]);
static int ble_cmd_gattc_read_by_uuid(int argc, char *argv[]);
static int ble_cmd_cfgwifi(int argc, char *argv[]);

static const struct ble_cmd_des cmd_tab[] =
{
    {"help",            ble_cmd_help},
    {"init",            ble_cmd_init},
    {"get_addr",        ble_cmd_get_addr},
    {"get_con_info",    ble_cmd_get_connect_info},
    {"update_params",   ble_cmd_update_params},
    {"scan",            ble_cmd_scan},
    {"stop_scan",       ble_cmd_stop_scan},
    {"connect",         ble_cmd_connect},
    {"disconnect",      ble_cmd_disconnect},
    {"get_server",      ble_cmd_gattc_service_get},
    {"get_char",        ble_cmd_gattc_characteristic_get},
    {"get_disc",        ble_cmd_gattc_discriptor_get},
    {"mtu_exch",        ble_cmd_mtu_exchange},
    {"notify",          ble_cmd_gatt_notify_change},
    {"write",           ble_cmd_gattc_write},
    {"read",            ble_cmd_gattc_read},
    {"notify_change",     ble_cmd_gatt_notify_change_by_uuid},
    {"write_uuid",      ble_cmd_gattc_write_by_uuid},
    {"read_uuid",       ble_cmd_gattc_read_by_uuid},
    {"cfgwifi",         ble_cmd_cfgwifi},
};

static int ble_cmd_help(int argc, char *argv[])
{
    rt_kprintf("[rw007_ble command]\n\n");
    rt_kprintf("rw007_ble help\n");
    rt_kprintf("rw007_ble init central/peripheral                           Note: init ble mode\n");
    rt_kprintf("rw007_ble get_addr                                          Note: get ble address\n");
    rt_kprintf("rw007_ble update_params                                     Note: update connect parameters(no support)\n");
    rt_kprintf("rw007_ble scan                                              Note: scan ble slave\n");
    rt_kprintf("rw007_ble stop_scan                                         Note: stop scan\n");
    rt_kprintf("rw007_ble connect xx:xx:xx:xx:xx:xx                         Note: use slave addr to connect\n");
    rt_kprintf("rw007_ble disconnect [conn_handle]                          Note: disconnect slave\n");
    rt_kprintf("rw007_ble get_server                                        Note: discover all server(no support)\n");
    rt_kprintf("rw007_ble get_char                                          Note: discover all description(no support)\n");
    rt_kprintf("rw007_ble get_disc                                          Note: discover all characteristic(no support)\n");
    rt_kprintf("rw007_ble mtu_exch                                          Note: ble mtu exchange(no support)\n");
    rt_kprintf("rw007_ble notify                                            Note: enable ble notify(no support)\n");
    rt_kprintf("rw007_ble write                                             Note: ble write data(no support)\n");
    rt_kprintf("rw007_ble read                                              Note: ble read data(no support)\n");
    rt_kprintf("rw007_ble notify_change [conn_handle] [UUID] [char_value]   Note: ble notify configure by uuid\n");
    rt_kprintf("rw007_ble write_uuid [conn_handle] [UUID] [data]            Note: ble write data by uuid\n");
    rt_kprintf("rw007_ble read_uuid [conn_handle] [UUID]                    Note: ble read data by uuid\n");
    rt_kprintf("rw007_ble cfgwifi duration_ms                               Note: enable ble configuration wifi\n");

    rt_kprintf("\n");
    return 0;
}

static void _print_addr(uint8_t addr[6])
{
    const uint8_t *u8p;

    u8p = addr;
    rt_kprintf("%02x:%02x:%02x:%02x:%02x:%02x", u8p[5], u8p[4], u8p[3], u8p[2], u8p[1], u8p[0]);
}

static void _print_hex(uint8_t *arr, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        rt_kprintf("%02x ", arr[i]);
    }
}

// recv slave resp cmd handle
static void rw007_ble_resp_handle(rt_uint16_t resp_type, void *data, rt_uint16_t size)
{
    switch(resp_type)
    {
        case RW007_BLE_RSP_TYPE_INIT:
            rt_kprintf("RW007_BLE_RSP_TYPE_INIT\n");
            break;
        case RW007_BLE_RSP_TYPE_ADDR_GET:
        {
            struct rw007_ble_get_addr *get_addr = (struct rw007_ble_get_addr *)data;
            rt_kprintf("public_id_addr = %02x:%02x:%02x:%02x:%02x:%02x\n",
                        get_addr->public_id_addr[5], get_addr->public_id_addr[4],
                        get_addr->public_id_addr[3], get_addr->public_id_addr[2],
                        get_addr->public_id_addr[1], get_addr->public_id_addr[0]);
            rt_kprintf("random_id_addr = %02x:%02x:%02x:%02x:%02x:%02x\n",
                        get_addr->random_id_addr[5], get_addr->random_id_addr[4],
                        get_addr->random_id_addr[3], get_addr->random_id_addr[2],
                        get_addr->random_id_addr[1], get_addr->random_id_addr[0]);
        }
            break;
        case RW007_BLE_RSP_TYPE_CONN_INFO_GET:
            rt_kprintf("RW007_BLE_RSP_TYPE_CONN_INFO_GET\n");
            break;
        case RW007_BLE_RSP_TYPE_CONN_UPD_PARAMS:
            rt_kprintf("RW007_BLE_RSP_TYPE_CONN_UPD_PARAMS\n");
            break;
        case RW007_BLE_RSP_TYPE_SCAN:
            rt_kprintf("RW007_BLE_RSP_TYPE_SCAN\n");
            break;
        case RW007_BLE_RSP_TYPE_CONNECT:
            rt_kprintf("RW007_BLE_RSP_TYPE_CONNECT\n");
            break;
        case RW007_BLE_RSP_TYPE_DISCONN:
            rt_kprintf("RW007_BLE_RSP_TYPE_DISCONN\n");
            break;
        case RW007_BLE_RSP_TYPE_GATT_DIS_FULL:
            rt_kprintf("RW007_BLE_RSP_TYPE_GATT_DIS_FULL\n");
            break;
        case RW007_BLE_RSP_TYPE_GATTC_SRV:
            rt_kprintf("RW007_BLE_RSP_TYPE_GATTC_SRV\n");
            break;
        case RW007_BLE_RSP_TYPE_GATTC_CHR:
            rt_kprintf("RW007_BLE_RSP_TYPE_GATTC_CHR\n");
            break;
        case RW007_BLE_RSP_TYPE_GATTC_DSC:
            rt_kprintf("RW007_BLE_RSP_TYPE_GATTC_DSC\n");
            break;
        case RW007_BLE_RSP_TYPE_MTU_EXCHANGE:
            rt_kprintf("RW007_BLE_RSP_TYPE_MTU_EXCHANGE\n");
            break;
        case RW007_BLE_RSP_TYPE_NOTIFY:
            rt_kprintf("RW007_BLE_RSP_TYPE_NOTIFY\n");
            break;
        case RW007_BLE_RSP_TYPE_WRITE:
            rt_kprintf("RW007_BLE_RSP_TYPE_WRITE\n");
            break;
        case RW007_BLE_RSP_TYPE_READ:
        {
            rw007_ble_read_rsp_t *rsp_read;
            uint8_t *data_read;

            rsp_read = (rw007_ble_read_rsp_t *)data;
            data_read = (uint8_t*)(rsp_read + 1);

            rt_kprintf("read %s connect:%d attr_handle:%d uuid: ",
                    rsp_read->status ? "failure" : "successful",
                    rsp_read->conn_handle, rsp_read->attr_handle);
            if (rsp_read->uuid.type == BLE_UUID_TYPE_16)
            {
                _print_hex((uint8_t*)&rsp_read->uuid, 2);
            }
            else if (rsp_read->uuid.type == BLE_UUID_TYPE_32)
            {
                _print_hex((uint8_t*)&rsp_read->uuid, 4);
            }
            else if (rsp_read->uuid.type == BLE_UUID_TYPE_128)
            {
                _print_hex((uint8_t*)&rsp_read->uuid, 16);
            }

            if (rsp_read->status == 0)
            {
                rt_kprintf(" read data:\n");
                hex_dump(data_read, size - (data_read - (uint8_t*)data));
            }
            else
            {
                rt_kprintf("\n");
            }
        }
            break;
        default:
            rt_kprintf("error response\n");
            break;
    }
}

// recv slave notity handle
static void rw007_ble_ntf_handle(rt_uint16_t ntf_type, void *data, rt_uint16_t size)
{
    switch(ntf_type)
    {
        case RW007_BLE_NTF_TYPE_CONN_UPD:
            rt_kprintf("RW007_BLE_NTF_TYPE_CONN_UPD\n");
            break;
        case RW007_BLE_NTF_TYPE_CONN_UPD_PARAMS:
            rt_kprintf("RW007_BLE_NTF_TYPE_CONN_UPD_PARAMS\n");
            break;
        case RW007_BLE_NTF_TYPE_DISC:
        {
            struct rw007_ble_gap_event_discov *disc_desc = RT_NULL;
            char *dicover_data = RT_NULL;

            rt_kprintf("RW007_BLE_NTF_TYPE_DISCOVER\n");

            disc_desc = (struct rw007_ble_gap_event_discov *)data;
            dicover_data = (char*)(disc_desc + 1);

            switch(disc_desc->discov_type)
            {
                case BLE_HCI_ADV_RPT_EVTYPE_ADV_IND:
                    break;

                case BLE_HCI_ADV_RPT_EVTYPE_DIR_IND:
                    break;

                case BLE_HCI_ADV_RPT_EVTYPE_SCAN_IND:
                    break;

                case BLE_HCI_ADV_RPT_EVTYPE_NONCONN_IND:
                    break;

                case BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP:
                    break;
            }

            rt_kprintf("received advertisement; event_type=%d rssi=%d addr_type=%d addr=",
                                                                    disc_desc->discov_type,
                                                                    disc_desc->rssi,
                                                                    disc_desc->addr_type);
            rt_kprintf("%02x:%02x:%02x:%02x:%02x:%02x\n", disc_desc->addr[5], disc_desc->addr[4],
                                                          disc_desc->addr[3], disc_desc->addr[2],
                                                          disc_desc->addr[1], disc_desc->addr[0]);
            rt_kprintf("mfg_data = ");
            for (int i = 0; i < disc_desc->length_data; i++)
            {
                rt_kprintf("%02x ", dicover_data[i]);
            }
            rt_kprintf("\n\n");
            break;
        }
        case RW007_BLE_NTF_TYPE_CONNECT:
        {
            struct rw007_ble_gap_event_connect *connect_event;
            struct rw007_ble_conn_desc *conn_desc;

            rt_kprintf("RW007_BLE_NTF_TYPE_CONNECT\n");

            connect_event = (struct rw007_ble_gap_event_connect *)data;
            conn_desc = (struct rw007_ble_conn_desc*)(connect_event + 1);

            rt_kprintf("connect event status: %d, conn_handle: %d\n", connect_event->status, connect_event->conn_handle);
            rt_kprintf("our_id_type:%d our_id_addr: ", conn_desc->our_id_type);
            _print_addr(conn_desc->our_id_addr);
            rt_kprintf("\npeer_id_type:%d peer_id_addr: ", conn_desc->peer_id_type);
            _print_addr(conn_desc->peer_id_addr);
            rt_kprintf("\nconn_itv:%d, conn_latency:%d, conn_suptout:%d, role:%d\n", conn_desc->conn_itvl,
                                conn_desc->conn_latency, conn_desc->supervision_timeout, conn_desc->role);
            break;
        }
        case RW007_BLE_NTF_TYPE_DISCONN:
            rt_kprintf("RW007_BLE_NTF_TYPE_DISCONN\n");
            break;
        case RW007_BLE_NTF_TYPE_MTU_EXCHANGE:
            rt_kprintf("RW007_BLE_NTF_TYPE_MTU_EXCHANGE\n");
            break;
        case RW007_BLE_NTF_TYPE_DISC_COMPLETE:
            rt_kprintf("RW007_BLE_NTF_TYPE_DISC_COMPLETE\n");
            break;
        case RW007_BLE_NTF_TYPE_ADV_COMPLETE:
            rt_kprintf("RW007_BLE_NTF_TYPE_ADV_COMPLETE\n");
            break;
        case RW007_BLE_NTF_TYPE_SUBSCRIBE:
            rt_kprintf("RW007_BLE_NTF_TYPE_SUBSCRIBE\n");
            break;
        case RW007_BLE_NTF_TYPE_NOTIFY_RX:
        {
            struct rw007_ble_gap_event_notify_rx *notify_rx;

            rt_kprintf("RW007_BLE_NTF_TYPE_NOTIFY_RX\n");

            notify_rx = (struct rw007_ble_gap_event_notify_rx *)data;
            rt_kprintf("conn_handle:%d, attr_handle:%d, rcv (%s) length:%d, data:\n",
                    notify_rx->conn_handle, notify_rx->attr_handle,
                    notify_rx->indication ? "indication":"notification", notify_rx->length_data);
            hex_dump((const rt_uint8_t *)(notify_rx + 1), notify_rx->length_data);
            break;
        }
        default:
            rt_kprintf("error notify\n");
            break;
    }
}

static int ble_cmd_init(int argc, char *argv[])
{
    rt_kprintf("%d - %s\n", __LINE__, __FUNCTION__);

    rt_uint8_t roles = 0;
    if(argc < 3)
        return -1;

    if (rt_strcmp("central", argv[2]) == 0)
    {
        rt_kprintf("start ble central\n");
        roles = RW007_BLE_INIT_ROLE_CENTRAL;
    }
    else if(rt_strcmp("Peripheral", argv[2]) == 0)
    {
        rt_kprintf("start ble Peripheral\n");
        roles = RW007_BLE_INIT_ROLE_PERIPHERAL;
    }
    else
    {
        return -1;
    }
    rw007_ble_init(roles);

    rw007_ble_resp_handle_cb_reg(rw007_ble_resp_handle);
    rw007_ble_ntf_handle_cb_reg(rw007_ble_ntf_handle);
    return 0;
}

static int ble_cmd_get_addr(int argc, char *argv[])
{
    rt_kprintf("%d - %s\n", __LINE__, __FUNCTION__);

    rw007_ble_addr_get();
    return 0;
}

static int ble_cmd_get_connect_info(int argc, char *argv[])
{
    rt_kprintf("%d - %s\n", __LINE__, __FUNCTION__);
    // rw007_ble_conn_info_get(connect_event.connect.conn_handle);
    return 0;
}

static int ble_cmd_update_params(int argc, char *argv[])
{
    rt_kprintf("%d - %s\n", __LINE__, __FUNCTION__);
    // struct rw007_ble_conn_update_params params;

    /* TODO */

    // rw007_ble_conn_update_params(connect_event.connect.conn_handle, &params);
    return 0;
}

static int ble_cmd_scan(int argc, char *argv[])
{
    rt_kprintf("%d - %s\n", __LINE__, __FUNCTION__);
    struct rw007_ble_scan_params scan_param;

    scan_param.limited = 0;
    scan_param.passive = 0;
    scan_param.filter = 0; // 0 disable /1 wl /2 inta /3 1+2
    scan_param.duration = 3000;
    scan_param.interval = 0;
    scan_param.window = 0;

    rw007_ble_scan(&scan_param);
    return 0;
}

static int ble_cmd_stop_scan(int argc, char *argv[])
{
    rt_kprintf("%d - %s\n", __LINE__, __FUNCTION__);
    rw007_ble_scan_stop();
    return 0;
}

static int ble_cmd_connect(int argc, char *argv[])
{
    rt_kprintf("%d - %s\n", __LINE__, __FUNCTION__);
    ble_addr_t addr;

    addr.type = RW007_BLE_ADDR_PUBLIC;

    rt_kprintf("str_addr: %s\n", argv[2]);

    _hexstrtoaddr(argv[2], addr.val);

    rt_kprintf("mac addr: %2x:%2x:%2x:%2x:%2x:%2x\n", addr.val[0], addr.val[1],
                                                      addr.val[2], addr.val[3],
                                                      addr.val[4], addr.val[5]);

    rw007_ble_connect(&addr);
    return 0;
}

static int ble_cmd_disconnect(int argc, char *argv[])
{
    rt_kprintf("%d - %s\n", __LINE__, __FUNCTION__);
    rt_uint16_t conn_handle = 0;
    conn_handle = atoi(argv[2]);

    rw007_ble_disconnect(conn_handle);

    return 0;
}

static int ble_cmd_gattc_service_get(int argc, char *argv[])
{
    rt_kprintf("%d - %s\n", __LINE__, __FUNCTION__);
    // rw007_ble_gatt_discover_full(connect_event.connect.conn_handle);
    return 0;
}

static int ble_cmd_gattc_characteristic_get(int argc, char *argv[])
{
    rt_kprintf("%d - %s\n", __LINE__, __FUNCTION__);
    return 0;
}

static int ble_cmd_gattc_discriptor_get(int argc, char *argv[])
{
    rt_kprintf("%d - %s\n", __LINE__, __FUNCTION__);
    // rw007_ble_mtu_exchange(connect_event.connect.conn_handle);
    return 0;
}

static int ble_cmd_mtu_exchange(int argc, char *argv[])
{
    rt_kprintf("%d - %s\n", __LINE__, __FUNCTION__);
    return 0;
}

static int ble_cmd_gatt_notify_change(int argc, char *argv[])
{
    rt_kprintf("%d - %s\n", __LINE__, __FUNCTION__);
    return 0;
}

static int ble_cmd_gattc_write(int argc, char *argv[])
{
    rt_kprintf("%d - %s\n", __LINE__, __FUNCTION__);
    return 0;
}

static int ble_cmd_gattc_read(int argc, char *argv[])
{
    rt_kprintf("%d - %s\n", __LINE__, __FUNCTION__);
    return 0;
}

static int ble_cmd_gatt_notify_change_by_uuid(int argc, char *argv[])
{
    uint16_t conn_handle;
    uint32_t uuidint;
    rw007_ble_uuid_t uuid;
    int rc;
    int value;
    int uuidlen;

    if (argc < 5)
    {
        rt_kprintf("argc < 5\n");
        return -1;
    }

    conn_handle = atoi(argv[2]);

    uuidlen = strlen(argv[3]);

    if (4 == uuidlen) {
        uuid.type = BLE_UUID_TYPE_16;
        sscanf(argv[3], "%x", &uuidint);
        uuid.uuid.uuid16 = uuidint & 0xFFFFu;
    }
    else if (8 == uuidlen) {
        uuid.type = BLE_UUID_TYPE_32;
        sscanf(argv[3], "%x", &uuidint);
        uuid.uuid.uuid32 = uuidint & 0xFFFFFFFFu;
    }
    else if (32 == uuidlen) {
        int i;

        uuid.type = BLE_UUID_TYPE_128;
        for (i = 0; i < 16; i++) {
            sscanf(&argv[3][i * 2], "%02x", &uuidint);
            uuid.uuid.uuid128[15 - i] = uuidint & 0xFFu;
        }
    }
    else {
        rt_kprintf("uuid:%s input error\n", argv[3]);
        return -1;
    }

    value = atoi(argv[4]);
    rt_kprintf("notifychange conn_hanle:%d uuid:%s value:%02x\n", conn_handle, argv[3], value);

    rc = rw007_ble_gatt_notify_change_by_uuid(conn_handle, &uuid, value);

    rt_kprintf("notifychange rc:%d\n", rc);
    return rc;
}

static int ble_cmd_gattc_write_by_uuid(int argc, char *argv[])
{
    uint16_t conn_handle;
    uint32_t uuidint;
    rw007_ble_uuid_t uuid;
    int rc;
    int uuidlen;
    int value_len;
    int i;
    uint8_t *value;

    if (argc < 5) {
        rt_kprintf("argc < 5\n");
        return -1;
    }

    conn_handle = atoi(argv[2]);

    uuidlen = strlen(argv[3]);

    if (4 == uuidlen) {
        uuid.type = BLE_UUID_TYPE_16;
        sscanf(argv[3], "%x", &uuidint);
        uuid.uuid.uuid16 = uuidint & 0xFFFFu;
    }
    else if (8 == uuidlen) {
        uuid.type = BLE_UUID_TYPE_32;
        sscanf(argv[3], "%x", &uuidint);
        uuid.uuid.uuid32 = uuidint & 0xFFFFFFFFu;
    }
    else if (32 == uuidlen) {
        int i;

        uuid.type = BLE_UUID_TYPE_128;
        for (i = 0; i < 16; i++) {
            sscanf(&argv[3][i * 2], "%02x", &uuidint);
            uuid.uuid.uuid128[15 - i] = uuidint & 0xFFu;
        }
    }
    else {
        rt_kprintf("uuid:%s input error\n", argv[3]);
        return -1;
    }

    value_len = strlen(argv[4]) / 2;
    value = rt_malloc(value_len);
    if (!value) {
        rt_kprintf("value parse failure\n");
        return -1;
    }
    rt_kprintf("data:");
    for (i = 0; i < value_len; i++) {
        sscanf(&argv[4][i * 2], "%02x", &uuidint);
        value[i] = uuidint & 0xFFu;
        rt_kprintf(" %02x", value[i]);
    }
    rt_kprintf("\n");

    rt_kprintf("write conn_hanle:%d uuid:%s\n", conn_handle, argv[3]);

    rc = rw007_ble_gattc_write_by_uuid(conn_handle, &uuid, value, value_len);
    rt_free(value);
    rt_kprintf("write rc:%d\n", rc);
    return rc;
}

static int ble_cmd_gattc_read_by_uuid(int argc, char *argv[])
{
    uint16_t conn_handle;
    uint32_t uuidint;
    rw007_ble_uuid_t uuid;
    int rc;
    int uuidlen;

    if (argc < 4)
    {
        rt_kprintf("argc < 4\n");
        return -1;
    }

    conn_handle = atoi(argv[2]);

    uuidlen = strlen(argv[3]);

    if (4 == uuidlen) {
        uuid.type = BLE_UUID_TYPE_16;
        sscanf(argv[3], "%x", &uuidint);
        uuid.uuid.uuid16 = uuidint & 0xFFFFu;
    }
    else if (8 == uuidlen) {
        uuid.type = BLE_UUID_TYPE_32;
        sscanf(argv[3], "%x", &uuidint);
        uuid.uuid.uuid32 = uuidint & 0xFFFFFFFFu;
    }
    else if (32 == uuidlen) {
        int i;

        uuid.type = BLE_UUID_TYPE_128;
        for (i = 0; i < 16; i++) {
            sscanf(&argv[3][i * 2], "%02x", &uuidint);
            uuid.uuid.uuid128[15 - i] = uuidint & 0xFFu;
        }
    }
    else {
        rt_kprintf("uuid:%s input error\n", argv[3]);
        return -1;
    }

    rt_kprintf("read conn_hanle:%d uuid:%s\n", conn_handle, argv[3]);

    rc = rw007_ble_gattc_read_by_uuid(conn_handle, &uuid);

    rt_kprintf("read rc:%d\n", rc);
    return rc;
}

static int ble_cmd_cfgwifi(int argc, char *argv[])
{
    int32_t duration_ms;

    if (argc != 3) return -1;

    duration_ms = atoi(argv[2]);

    // duration_ms == 0 by used default 60second;
    if (duration_ms < 0) return -2;

    return rw007_ble_cfgwifi(duration_ms);
}

static int rw007_ble_msh(int argc, char *argv[])
{
    int i, result = 0;
    const struct ble_cmd_des *run_cmd = RT_NULL;

    if (argc == 1)
    {
        ble_cmd_help(argc, argv);
        return 0;
    }

    /* find fun */
    for (i = 0; i < sizeof(cmd_tab) / sizeof(cmd_tab[0]); i++)
    {
        if (rt_strcmp(cmd_tab[i].cmd, argv[1]) == 0)
        {
            run_cmd = &cmd_tab[i];
            break;
        }
    }

    /* not find fun, print help */
    if (run_cmd == RT_NULL)
    {
        ble_cmd_help(argc, argv);
        return 0;
    }

    /* run fun */
    if (run_cmd->fun != RT_NULL)
    {
        result = run_cmd->fun(argc, argv);
    }

    if (result)
    {
        ble_cmd_help(argc, argv);
    }
    return 0;
}

#if defined(RT_USING_FINSH) && defined(FINSH_USING_MSH)
FINSH_FUNCTION_EXPORT_ALIAS(rw007_ble_msh, __cmd_rw007_ble, ble command.);
#endif
