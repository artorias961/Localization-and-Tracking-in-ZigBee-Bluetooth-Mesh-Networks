/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <dk_buttons_and_leds.h>
#include <zephyr/logging/log.h>
#include <ram_pwrdn.h>
#include <zephyr/device.h>
#include <zephyr/pm/device.h>
#include <openthread/instance.h>
#include <openthread/link.h>

#include "coap_client_utils.h"

#if CONFIG_BT_NUS
#include "ble_utils.h"
#endif





#define RSSI_UPDATE_INTERVAL K_SECONDS(5) // Update interval in seconds

static struct k_timer rssi_timer;

void rssi_timer_handler(struct k_timer *dummy) {
    get_and_print_rssi();
}















LOG_MODULE_REGISTER(coap_client, CONFIG_COAP_CLIENT_LOG_LEVEL);

#define OT_CONNECTION_LED DK_LED1
#define BLE_CONNECTION_LED DK_LED2
#define MTD_SED_LED DK_LED3

#if CONFIG_BT_NUS

#define COMMAND_REQUEST_UNICAST 'u'
#define COMMAND_REQUEST_MULTICAST 'm'
#define COMMAND_REQUEST_PROVISIONING 'p'

static void on_nus_received(struct bt_conn *conn, const uint8_t *const data, uint16_t len)
{
	LOG_INF("Received data: %c", data[0]);

	switch (*data) {
	case COMMAND_REQUEST_UNICAST:
		coap_client_toggle_one_light();
		break;

	case COMMAND_REQUEST_MULTICAST:
		coap_client_toggle_mesh_lights();
		break;

	case COMMAND_REQUEST_PROVISIONING:
		coap_client_send_provisioning_request();
		break;

	default:
		LOG_WRN("Received invalid data from NUS");
	}
}

static void on_ble_connect(struct k_work *item)
{
	ARG_UNUSED(item);

	dk_set_led_on(BLE_CONNECTION_LED);
}

static void on_ble_disconnect(struct k_work *item)
{
	ARG_UNUSED(item);

	dk_set_led_off(BLE_CONNECTION_LED);
}

#endif /* CONFIG_BT_NUS */

static void on_ot_connect(struct k_work *item)
{
	ARG_UNUSED(item);

	dk_set_led_on(OT_CONNECTION_LED);
}

static void on_ot_disconnect(struct k_work *item)
{
	ARG_UNUSED(item);

	dk_set_led_off(OT_CONNECTION_LED);
}

static void on_mtd_mode_toggle(uint32_t med)
{
#if IS_ENABLED(CONFIG_PM_DEVICE)
	const struct device *cons = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

	if (!device_is_ready(cons)) {
		return;
	}

	if (med) {
		pm_device_action_run(cons, PM_DEVICE_ACTION_RESUME);
	} else {
		pm_device_action_run(cons, PM_DEVICE_ACTION_SUSPEND);
	}
#endif
	dk_set_led(MTD_SED_LED, med);
}

static void on_button_changed(uint32_t button_state, uint32_t has_changed)
{
	uint32_t buttons = button_state & has_changed;

	if (buttons & DK_BTN1_MSK) {
		coap_client_toggle_one_light();
	}

	if (buttons & DK_BTN2_MSK) {
		coap_client_toggle_mesh_lights();
		get_and_print_rssi();
	}

	if (buttons & DK_BTN3_MSK) {
		coap_client_toggle_minimal_sleepy_end_device();
	}

	if (buttons & DK_BTN4_MSK) {
		coap_client_send_provisioning_request();
	}
}

void get_and_print_rssi(void){
	otInstance *instance = openthread_get_default_instance();
	int8_t rssi;
	otError error;

	// instance = otInstanceInitSingle();
	if(instance == NULL){
		LOG_ERR("Failed to get OpenThread instance");
		return 0;
	}
	// rssi = otThreadGetParentLastRssi(instance);
	// avg_rssi = otThreadGetParentAverageRssi();
	// LOG_WRN("Parent Last RSSI: %d dBm\n", rssi);


	otThreadLinkInfo linkInfo;
	// error = otThreadGetParentLastRssi(instance, &rssi);
	error = otThreadGetParentLastRssi(instance, &linkInfo);
	
    if (error == OT_ERROR_NONE) {
		rssi = linkInfo.mRss;
        LOG_INF("Parent Last RSSI: %d dBm \n", rssi);
		printk("Parent Last RSSI: %d dBm \n", rssi);
    } 
	else {
        LOG_ERR("Failed to get RSSI: %d", error);
    }

}

// static void perform_energy_scan(otInstance *instance)
// {
//     otError error;
//     uint32_t scanChannels = OT_RADIO_2P4GHZ_OQPSK_CHANNEL_MASK; 
//     uint16_t scanDuration = 300; // Duration in milliseconds

//     error = otLinkEnergyScan(instance, scanChannels, scanDuration, handle_energy_scan_result, instance);
//     if (error != OT_ERROR_NONE)
//     {
//         LOG_ERR("Failed to start energy scan: %d", error);
//     }
// }

// static void handle_energy_scan_result(otInstance *aInstance, uint8_t aChannel, int8_t aMaxRssi)
// {
//     LOG_INF("Energy Scan Result - Channel: %d, Max RSSI: %d dBm", aChannel, aMaxRssi);
// }

int main(void)
{
	int ret;

	// otInstance *instance;
	// int8_t rssi;

	LOG_INF("Start CoAP-client sample");

	if (IS_ENABLED(CONFIG_RAM_POWER_DOWN_LIBRARY)) {
		power_down_unused_ram();
	}

	ret = dk_buttons_init(on_button_changed);
	if (ret) {
		LOG_ERR("Cannot init buttons (error: %d)", ret);
		return 0;
	}

	ret = dk_leds_init();
	if (ret) {
		LOG_ERR("Cannot init leds, (error: %d)", ret);
		return 0;

	

	}

#if CONFIG_BT_NUS
	struct bt_nus_cb nus_clbs = {
		.received = on_nus_received,
		.sent = NULL,
	};

	ret = ble_utils_init(&nus_clbs, on_ble_connect, on_ble_disconnect);
	if (ret) {
		LOG_ERR("Cannot init BLE utilities");
		return 0;
	}

#endif /* CONFIG_BT_NUS */

	coap_client_utils_init(on_ot_connect, on_ot_disconnect, on_mtd_mode_toggle);

	k_timer_init(&rssi_timer, rssi_timer_handler, NULL);
    k_timer_start(&rssi_timer, RSSI_UPDATE_INTERVAL, RSSI_UPDATE_INTERVAL);

	return 0;
}
