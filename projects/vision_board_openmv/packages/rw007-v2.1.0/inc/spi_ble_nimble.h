#ifndef __SPI_BLE_NIMBLE__
#define __SPI_BLE_NIMBLE__

#include "rtthread.h"

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif
#define MYNEWT_VAL(x)                           MYNEWT_VAL_ ## x

/* Advertising report */
#define BLE_HCI_ADV_RPT_EVTYPE_ADV_IND      (0)
#define BLE_HCI_ADV_RPT_EVTYPE_DIR_IND      (1)
#define BLE_HCI_ADV_RPT_EVTYPE_SCAN_IND     (2)
#define BLE_HCI_ADV_RPT_EVTYPE_NONCONN_IND  (3)
#define BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP     (4)

/** Connection security state */
struct ble_gap_sec_state {
    /** If connection is encrypted */
    unsigned encrypted:1;

    /** If connection is authenticated */
    unsigned authenticated:1;

    /** If connection is bonded (security information is stored)  */
    unsigned bonded:1;

    /** Size of a key used for encryption */
    unsigned key_size:5;
};

typedef struct {
    rt_uint8_t type;
    rt_uint8_t val[6];
} ble_addr_t;

/** @brief Connection descriptor */
struct ble_gap_conn_desc {
    /** Connection security state */
    struct ble_gap_sec_state sec_state;
    /** Local identity address */
    ble_addr_t our_id_addr;
    /** Peer identity address */
    ble_addr_t peer_id_addr;
    /** Local over-the-air address */
    ble_addr_t our_ota_addr;
    /** Peer over-the-air address */
    ble_addr_t peer_ota_addr;
    /** Connection handle */
    rt_uint16_t conn_handle;
    /** Connection interval */
    rt_uint16_t conn_itvl;
    /** Connection latency */
    rt_uint16_t conn_latency;
    /** Connection supervision timeout */
    rt_uint16_t supervision_timeout;
    /** Connection Role
     * Possible values BLE_GAP_ROLE_SLAVE or BLE_GAP_ROLE_MASTER
     */
    rt_uint8_t role;
    /** Master clock accuracy */
    rt_uint8_t master_clock_accuracy;
};

struct ble_gap_disc_desc {
    /*** Common fields. */
    rt_uint8_t event_type;
    rt_uint8_t length_data;
    ble_addr_t addr;
    rt_int8_t rssi;
    rt_uint8_t *data;

    /***
     * LE direct advertising report fields; direct_addr is BLE_ADDR_ANY if
     * direct address fields are not present.
     */
    ble_addr_t direct_addr;
};

struct ble_gap_upd_params {
    rt_uint16_t itvl_min;
    rt_uint16_t itvl_max;
    rt_uint16_t latency;
    rt_uint16_t supervision_timeout;
    rt_uint16_t min_ce_len;
    rt_uint16_t max_ce_len;
};

struct ble_gap_passkey_params {
    rt_uint8_t action;
    rt_uint32_t numcmp;
};

struct ble_gap_repeat_pairing {
    /** The handle of the relevant connection. */
    rt_uint16_t conn_handle;

    /** Properties of the existing bond. */
    rt_uint8_t cur_key_size;
    rt_uint8_t cur_authenticated:1;
    rt_uint8_t cur_sc:1;

    /**
     * Properties of the imminent secure link if the pairing procedure is
     * allowed to continue.
     */
    rt_uint8_t new_key_size;
    rt_uint8_t new_authenticated:1;
    rt_uint8_t new_sc:1;
    rt_uint8_t new_bonding:1;
};

struct ble_gap_event {
    rt_uint8_t type;

    union {
        struct {
            int status;
            rt_uint16_t conn_handle;
        } connect;

        struct {
            int reason;
            struct ble_gap_conn_desc conn;
        } disconnect;

        struct ble_gap_disc_desc disc;

#if MYNEWT_VAL(BLE_EXT_ADV)
        struct ble_gap_ext_disc_desc ext_disc;
#endif

        struct {
            int reason;
        } disc_complete;

        struct {
            int reason;

#if MYNEWT_VAL(BLE_EXT_ADV)
            rt_uint8_t instance;

            rt_uint16_t conn_handle;
            rt_uint8_t num_ext_adv_events;
#endif
        } adv_complete;

        struct {
            int status;
            rt_uint16_t conn_handle;
        } conn_update;

        struct {
            const struct ble_gap_upd_params *peer_params;
            struct ble_gap_upd_params *self_params;
            rt_uint16_t conn_handle;
        } conn_update_req;

        struct {
            int status;
            rt_uint16_t conn_handle;
        } term_failure;

        struct {
            int status;
            rt_uint16_t conn_handle;
        } enc_change;

        struct {
            struct ble_gap_passkey_params params;

            rt_uint16_t conn_handle;
        } passkey;

        struct {
            struct os_mbuf *om;
            rt_uint16_t attr_handle;
            rt_uint16_t conn_handle;
            rt_uint8_t indication:1;
        } notify_rx;

        struct {
            int status;
            rt_uint16_t conn_handle;

            rt_uint16_t attr_handle;
            rt_uint8_t indication:1;
        } notify_tx;

        struct {
            rt_uint16_t conn_handle;
            rt_uint16_t attr_handle;
            rt_uint8_t reason;
            rt_uint8_t prev_notify:1;

            rt_uint8_t cur_notify:1;
            rt_uint8_t prev_indicate:1;
            rt_uint8_t cur_indicate:1;
        } subscribe;

        struct {
            rt_uint16_t conn_handle;
            rt_uint16_t channel_id;
            rt_uint16_t value;
        } mtu;

        struct {
            rt_uint16_t conn_handle;
        } identity_resolved;

        struct ble_gap_repeat_pairing repeat_pairing;

        struct {
            int status;
            rt_uint16_t conn_handle;
            rt_uint8_t tx_phy;
            rt_uint8_t rx_phy;
        } phy_updated;
    };
};

#endif
