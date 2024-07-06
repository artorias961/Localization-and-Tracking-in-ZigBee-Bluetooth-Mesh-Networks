// #ifndef ZB_CUSTOM_CLUSTER_H
// #define ZB_CUSTOM_CLUSTER_H

// #include <zboss_api.h>
// #include <zboss_api_zcl.h>
// #include <zboss_api_af.h>

// #define ZB_ZCL_CLUSTER_ID_RSSI_REPORT 0xFC00
// #define ZB_ZCL_ATTR_RSSI_ID 0x0000
// #define RSSI_REPORT_ENDPOINT 10


// // Ensure the attributes are constants
// #define ZB_ZCL_DECLARE_RSSI_REPORT_ATTRIB_LIST(attr_list, rssi) \
//     ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list) \
//     ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_RSSI_ID, rssi) \
//     ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

// #define ZB_ZCL_DECLARE_RSSI_REPORT_CLUSTER_LIST(cluster_list, attr_list) \
//     ZB_ZCL_START_DECLARE_CLUSTER_LIST(cluster_list) \
//     ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_RSSI_REPORT, ZB_ZCL_ARRAY_SIZE(attr_list, zb_zcl_attr_t), attr_list, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_MANUF_CODE_INVALID) \
//     ZB_ZCL_FINISH_DECLARE_CLUSTER_LIST

// #define ZB_DECLARE_RSSI_REPORT_EP(ep_name, ep_id, cluster_list) \
//     ZB_AF_SIMPLE_DESC_TYPE(ep_name, ep_id, ZB_AF_HA_PROFILE_ID, ZB_HA_DIMMABLE_LIGHT_DEVICE_ID, \
//         ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t), 0) \
//     ZB_AF_SIMPLE_DESC(ep_name, ep_id, ZB_AF_HA_PROFILE_ID, ZB_HA_DIMMABLE_LIGHT_DEVICE_ID, \
//         ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t), 0, cluster_list) \
//     ZB_AF_DECLARE_ENDPOINT_DESC(ep_name, ep_id, ZB_AF_HA_PROFILE_ID, 0, NULL, \
//         ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t), cluster_list, \
//         (zb_af_simple_desc_1_1_t*)&simple_desc_##ep_name, 0, NULL, 0, NULL)

// #define NET_REQUEST_IEEE802154_ED_SCAN \
//     (NET_MGMT_LAYER_L2 | NET_MGMT_LAYER_CODE(0x123) | NET_MGMT_COMMAND(0x01))

// #endif // ZB_CUSTOM_CLUSTER_H






// #ifndef ZB_CUSTOM_CLUSTER_H
// #define ZB_CUSTOM_CLUSTER_H

// #include <zboss_api.h>
// #include <zboss_api_zcl.h>
// #include <zboss_api_af.h>

// #define ZB_ZCL_CLUSTER_ID_RSSI_REPORT 0xFC00
// #define ZB_ZCL_ATTR_RSSI_ID 0x0000
// #define RSSI_REPORT_ENDPOINT 10

// /* RSSI Report Endpoint */
// #define RSSI_REPORT_EP_ID               10

// /* RSSI Attribute List */
// typedef struct {
//     zb_int8_t rssi_value;
// } rssi_report_attrs_t;

// static rssi_report_attrs_t rssi_report_attrs = {
//     .rssi_value = 0
// };


// ZB_ZCL_DECLARE_RSSI_REPORT_ATTRIB_LIST(
//     rssi_report_attr_list,
//     &rssi_report_attrs.rssi_value
// );


// ZB_ZCL_DECLARE_RSSI_REPORT_CLUSTER_LIST(
//     rssi_report_cluster_list,
//     rssi_report_attr_list
// );


// ZB_DECLARE_RSSI_REPORT_EP(
//     rssi_report_ep,
//     RSSI_REPORT_ENDPOINT,
//     rssi_report_cluster_list
// );

// /* Declare the device context for the custom cluster */
// ZBOSS_DECLARE_DEVICE_CTX_1_EP(
//     rssi_report_ctx,
//     rssi_report_ep
// );

// #endif // ZB_CUSTOM_CLUSTER_H




















