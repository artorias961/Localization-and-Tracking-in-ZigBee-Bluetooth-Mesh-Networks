/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *  @brief Nordic Mesh light sample
 */
#include <zephyr/bluetooth/bluetooth.h>
#include <bluetooth/mesh/models.h>
#include <bluetooth/mesh/dk_prov.h>
#include <dk_buttons_and_leds.h>
#include "model_handler.h"

#include <zephyr/logging/log.h>about:blank#blocked

// Creating a logger event (ERROR, INF, WRN, DBG, PRINTK)
LOG_MODULE_REGISTER(chat, CONFIG_LOG_DEFAULT_LEVEL);

static void bt_ready(int err)
{
	// An error will occur if the value is non zero
	if (err) {
		// An error occur
		printk("Bluetooth init failed (err %d)\n", err);

		// Stop the bluetooth chat
		return;
	}

	// Letting the user know that the bluetooth has been a success
	printk("Bluetooth initialized\n");

	// Module interface with the LEDS 
	dk_leds_init();

	// Module interface with the BUTTONS (do not understand NULL yet) <-----------------
	dk_buttons_init(NULL);

	// Initialize Bluetooth Mesh Stack/Support ("bt_mesh_dk_prov_init" and "model_handler_init"  initializes the applications models and related components, necessary)
	err = bt_mesh_init(bt_mesh_dk_prov_init(), model_handler_init());

	// An error will occur if the value is non zero
	if (err) {
		// An error occur
		printk("Initializing mesh failed (err %d)\n", err);
		
		// Stop the bluetooth chat
		return;
	}

	// Verifying Bluetooth is on
	if (IS_ENABLED(CONFIG_SETTINGS)) {
		
		// Load the preset settings for the BLE environment
		settings_load();
	}

	/* This will be a no-op if settings_load() loaded provisioning info */
	bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);

	// Bluetooth success
	printk("Mesh initialized\n");
}

int main(void)
{
	// Creating an constant value for error
	int err;

	// Prompting the user 
	printk("Initializing...\n");

	// Determining the value of the bluetooth
	err = bt_enable(bt_ready);

	// An error will occur if the value is non zero
	if (err) {
		// When an error occurs, print out the value of the error
		printk("Bluetooth init failed (err %d)\n", err);
	}

	return 0;
}
