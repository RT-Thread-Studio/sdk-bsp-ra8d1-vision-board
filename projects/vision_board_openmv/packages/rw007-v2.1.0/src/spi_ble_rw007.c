#include "spi_ble_rw007.h"
#include "spi_wifi_rw007.h"

struct ble_gap_event connect_event;
struct ble_gap_conn_desc connect_desc;

rw007_ble_resp_handle_cb resp_handle_cb;
rw007_ble_ntf_handle_cb ntf_handle_cb;

#define BLE_DISPATCH_MAILBOX_NUM    10
static struct rt_mailbox _ble_dispatch_mb;
static rt_uint32_t _ble_dispatch_mb_buffer[BLE_DISPATCH_MAILBOX_NUM];
#define BLE_DISPATCH_TASK_STACK_SIZE    512
static rt_uint32_t _ble_dispatch_task_stack[BLE_DISPATCH_TASK_STACK_SIZE / 4];
static struct rt_thread _ble_dispatch_task_obj;

void rw007_ble_resp_handle_cb_reg(rw007_ble_resp_handle_cb resp_cb)
{
    resp_handle_cb = resp_cb;
}

void rw007_ble_ntf_handle_cb_reg(rw007_ble_ntf_handle_cb notify_cb)
{
    ntf_handle_cb = notify_cb;
}

static void rw007_ble_data_input(void *buff, int len)
{
    struct rw007_ble_resp *resp = RT_NULL;
    rt_uint8_t *pos = (rt_uint8_t *)buff;
    int msglen;

    while (len > 0)
    {
        void *pmsgobj;

        resp = (struct rw007_ble_resp *)pos;

        msglen = member_offset(struct rw007_ble_resp, data) + resp->len;
        pmsgobj = rt_malloc(msglen);
        if (pmsgobj != NULL)
        {
            rt_err_t err;

            memcpy(pmsgobj, resp, msglen);

            err = rt_mb_send(&_ble_dispatch_mb, (rt_ubase_t)pmsgobj);
            if (err != RT_EOK)
            {
                rt_free(pmsgobj);
                rt_kprintf("ble data input blocking, lost packet type:%d, len:%d\n", resp->resp_type, resp->len);
            }
            else
            {
                rt_kprintf("ble data input packet resp_type: %d, len: %d\n", resp->resp_type, resp->len);
            }
        }
        else
        {
            rt_kprintf("ble data input malloc failure, lost packet type:%d, len:%d\n", resp->resp_type, resp->len);
        }

        len -= msglen;
        pos += msglen;
    }
}

static void rw007_ble_data_output(void *buff, int len)
{
    rw007_ble_send_data(buff, len);
}

int rw007_ble_init(rt_uint8_t roles)
{
    rt_uint32_t len;

    struct rw007_ble_req req;

    req.req_type = RW007_BLE_REQ_TYPE_INIT;
    req.value.int_value = roles;
    req.len = sizeof(rt_uint32_t);

    len = member_offset(struct rw007_ble_req, value) + req.len;

    rw007_ble_data_output((void *)&req, len);

    return RT_EOK;
}

int rw007_ble_addr_get(void)
{
    rt_uint32_t len;

    struct rw007_ble_req req;

    req.req_type = RW007_BLE_REQ_TYPE_ADDR_GET;
    req.len = 0;

    len = member_offset(struct rw007_ble_req, value) + req.len;

    rw007_ble_data_output((void *)&req, len);

    return RT_EOK;
}

int rw007_ble_conn_info_get(rt_uint16_t conn_handle)
{
    return RT_EOK;
}

int rw007_ble_conn_update_params(rt_uint16_t conn_handle, struct ble_gap_upd_params *params)
{
    return RT_EOK;
}

int rw007_ble_central_init(void)
{
    return RT_EOK;
}

int rw007_ble_scan(struct rw007_ble_scan_params *scan_param)
{
    rt_uint32_t len;

    struct rw007_ble_req req;

    req.req_type = RW007_BLE_REQ_TYPE_SCAN;
    req.len = sizeof(struct rw007_ble_scan_params);
    rt_memcpy((void *)req.value.bytes, (const void *)scan_param, req.len);

    len = member_offset(struct rw007_ble_req, value) + req.len;

    rw007_ble_data_output((void *)&req, len);
    return RT_EOK;
}

int rw007_ble_scan_stop(void)
{
    rt_uint32_t len;

    struct rw007_ble_req req;

    req.req_type = RW007_BLE_REQ_TYPE_STOP_SCAN;
    req.len = 0;

    len = member_offset(struct rw007_ble_req, value) + req.len;

    rw007_ble_data_output((void *)&req, len);
    return RT_EOK;
}

int rw007_ble_connect(ble_addr_t *addr)
{
    int len;

    struct rw007_ble_req req;

    req.req_type = RW007_BLE_REQ_TYPE_CONNECT;
    rt_memcpy((void *)&req.value.addr, (const void *)addr, sizeof(ble_addr_t));
    req.len = sizeof(ble_addr_t);

    len = member_offset(struct rw007_ble_req, value) + req.len;

    rw007_ble_data_output((void *)&req, len);

    return RT_EOK;
}

int rw007_ble_disconnect(rt_uint16_t conn_handle)
{
    rt_uint32_t len;

    struct rw007_ble_req req;

    req.req_type = RW007_BLE_REQ_TYPE_DISCONN;
    req.value.int_value = conn_handle;
    req.len = sizeof(rt_uint32_t);

    len = member_offset(struct rw007_ble_req, value) + req.len;

    rw007_ble_data_output((void *)&req, len);
    return RT_EOK;
}

int rw007_ble_gattc_service_get(rt_uint16_t conn_handle)
{
    return RT_EOK;
}

int rw007_ble_gattc_characteristic_get(rt_uint16_t conn_handle, rt_uint16_t attr_handle)
{
    return RT_EOK;
}

int rw007_ble_gattc_discriptor_get(rt_uint16_t conn_handle, rt_uint16_t attr_handle)
{
    return RT_EOK;
}

int rw007_ble_gattc_service_discover(rt_uint16_t conn_handle)
{
    return RT_EOK;
}

int rw007_ble_mtu_exchange(rt_uint16_t conn_handle)
{
    return RT_EOK;
}

int rw007_ble_gatt_notify_change(rt_uint16_t attr_handle)
{
    return RT_EOK;
}

int rw007_ble_gattc_write(rt_uint16_t conn_handle, rt_uint16_t attr_handle, const rt_uint8_t *buf, rt_uint16_t len)
{
    return RT_EOK;
}

int rw007_ble_gattc_read(rt_uint16_t conn_handle, rt_uint16_t attr_handle)
{
    return RT_EOK;
}

int rw007_ble_gatt_notify_change_by_uuid(rt_uint16_t conn_handle, const rw007_ble_uuid_t *uuid, rt_uint16_t value)
{
    int req_len;
    struct rw007_ble_req req;
    struct rw007_ble_notify_change_by_uuid notify_change;

    notify_change.conn_handle = conn_handle;
    notify_change.char_value = value;
    notify_change.uuid = *uuid;

    req.req_type = RW007_BLE_REQ_TYPE_NOTIFYCHANGE_BY_UUID;
    req.len = sizeof(struct rw007_ble_notify_change_by_uuid);
    rt_memcpy((void *)&req.value.notify_change, (const void *)&notify_change, req.len);

    req_len = member_offset(struct rw007_ble_req, value) + req.len;

    rw007_ble_data_output((void *)&req, req_len);

    return RT_EOK;
}

int rw007_ble_gattc_write_by_uuid(rt_uint16_t conn_handle, const rw007_ble_uuid_t *uuid, const rt_uint8_t *buf, rt_uint16_t len)
{
    int req_len;
    struct rw007_ble_req req;
    struct rw007_ble_write_by_uuid *write = RT_NULL;

    write = (struct rw007_ble_write_by_uuid *)rt_malloc(sizeof(struct rw007_ble_write_by_uuid) + len);

    if (write == RT_NULL)
    {
        return -1;
    }

    write->conn_handle = conn_handle;
    write->data_len = len;
    write->uuid = *uuid;
    rt_memcpy(write->data, buf, write->data_len);

    req.req_type = RW007_BLE_REQ_TYPE_WRITE_BY_UUID;
    req.len = sizeof(struct rw007_ble_write_by_uuid) + write->data_len;
    rt_memcpy((void *)&req.value.bytes, (const void *)write, req.len);

    req_len = member_offset(struct rw007_ble_req, value) + req.len;

    rw007_ble_data_output((void *)&req, req_len);

    rt_free(write);
    write = RT_NULL;
    return RT_EOK;
}

int rw007_ble_gattc_read_by_uuid(rt_uint16_t conn_handle, const rw007_ble_uuid_t *uuid)
{
    int req_len;
    struct rw007_ble_req req;
    struct rw007_ble_read_by_uuid *read_req;

    read_req = (struct rw007_ble_read_by_uuid *)rt_malloc(sizeof(struct rw007_ble_read_by_uuid));
    if (read_req == RT_NULL)
    {
        return -1;
    }

    read_req->conn_handle = conn_handle;
    read_req->reserved = 0;
    read_req->uuid = *uuid;
    req.req_type = RW007_BLE_REQ_TYPE_READ_BY_UUID;
    req.len = sizeof(struct rw007_ble_read_by_uuid);
    rt_memcpy((void *)&req.value.bytes, (const void *)read_req, req.len);
    req_len = member_offset(struct rw007_ble_req, value) + req.len;

    rw007_ble_data_output((void *)&req, req_len);
    rt_free(read_req);

    return RT_EOK;
}

static void _ble_dispatch_task(void *parameter)
{
    for (;;)
    {
        rt_err_t err;
        rt_ubase_t pmsgobj = 0;
        struct rw007_ble_resp *resp = RT_NULL;

        err = rt_mb_recv(&_ble_dispatch_mb, &pmsgobj, RT_WAITING_FOREVER);
        if (err != RT_EOK) continue;

        resp = (struct rw007_ble_resp *)pmsgobj;

        if(resp->resp_type == RW007_BLE_RESP_TYPE_CMD)
        {
            if(resp_handle_cb)
            {
                resp_handle_cb(resp->cmd, resp->data, resp->len);
            }
        }
        else if(resp->resp_type == RW007_BLE_RESP_TYPE_NOTIFY)
        {
            if(ntf_handle_cb)
            {
                ntf_handle_cb(resp->cmd, resp->data, resp->len);
            }
        }

        rt_free(resp);
    }
}

static int _ble_dispatch_init(void)
{
    rt_err_t err;

    err = rt_mb_init(&_ble_dispatch_mb,
                    "bledp",
                    _ble_dispatch_mb_buffer,
                    BLE_DISPATCH_MAILBOX_NUM,
                    RT_IPC_FLAG_PRIO);
    if (err != RT_EOK) {
        rt_kprintf("ble dispatch mailbox init failure\n");
        return err;
    }

    err = rt_thread_init(&_ble_dispatch_task_obj,
                        "bledp",
                        _ble_dispatch_task, 0,
                        &_ble_dispatch_task_stack,
                        BLE_DISPATCH_TASK_STACK_SIZE,
                        10, 10);
    if (err != RT_EOK) {
        rt_kprintf("ble dispatch task init failure\n");
        return err;
    }

    err = rt_thread_startup(&_ble_dispatch_task_obj);
    return err;
}

static int rw007_spi_ble_init(void)
{
    rw007_ble_recv_data_func_reg(rw007_ble_data_input);

    return _ble_dispatch_init();
}
INIT_APP_EXPORT(rw007_spi_ble_init);
