/*
The purpose of the code is to use GATT to gather RSSI data from a singular device. 

Then implement to ZigBee
*/


// General libraries for zephry
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/smf.h>
#include <zephyr/logging/log.h>

// Libraries for the bluetooth
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/addr.h>
#include <zephyr/bluetooth/conn.h>


// Global varaibles
#define DEVICE_NAME             CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN         (sizeof(DEVICE_NAME) - 1)

// Declaring the Log Module and the severity level I want it to respond to (debug level, lowest level) 
LOG_MODULE_REGISTER(ble_test, LOG_LEVEL_INF);



// STEP 5.1 - Create the advertising parameter for connectable advertising 
static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM((BT_LE_ADV_OPT_CONNECTABLE|BT_LE_ADV_OPT_USE_IDENTITY), /* Connectable advertising and use identity address */
                800, /*Min Advertising Interval 500ms (800*0.625ms) */
                801, /*Max Advertising Interval 500.625ms (801*0.625ms)*/
                NULL); /* Set to NULL for undirected advertising*/



// Creating the advertising data
static const struct bt_data advertise_data[] = {
	/* STEP 3.1 - Set the flags and populate the device name in the advertising packet */
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),

};



// 
static const struct bt_data scan_data[] = {
	/* STEP 3.2.2 - Include the 16-bytes (128-Bits) UUID of the LBS service in the scan response packet */
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x1523, 0x785feabcd123)),
};

// This function handles RSSI updates
void rssi_updated(struct bt_conn *conn, int8_t rssi) {
    LOG_WRN("RSSI: %d dBm", rssi);
}

// Our bluetooth connection will be set to NULL
struct bt_conn* our_conn = NULL;

void on_connected(struct bt_conn *conn, uint8_t err){
    // Creating a conditional statement if there is an error
    if (err){
        // Log the error to the console
        LOG_ERR("Connection error %d\n", err);

        // Stop the program since we got an error
        return;
    }

    // Log the console that it was able to connect through bluetooth
    LOG_INF("Connected");
    our_conn = bt_conn_ref(conn);
}


void disconnected(struct bt_conn *conn, uint8_t err){
    // Creating a conditional statement if there is an error
    if (err){
        // Log the error to the console
        LOG_ERR("Disconnect error %d\n", err);

        // Stop the program since we got an error
        return;
    }
    


}



int main(void){
    // Creating an integer variable
    int err;

    // some counter
    int cnt = 0;

    // Logging our processes
    LOG_INF("Setting up our bluetooth . . .");

    // Storing the addresses
    bt_addr_le_t addr;
    err = bt_addr_le_from_str("FF:EE:DD:CC:BB:AA", "random", &addr);

    // Creating a conditional statement if there is an error
    if (err){
        // Log the error to the module
        LOG_ERR("Invalid BT address (err %d)\n", err);

        // Stop the program since we got an error 
        return err;
    }

    // Each manufacture has there own security (manufacture security for Bluetooth or encoding)
    err = bt_id_create(&addr, NULL);

    // Creating a conditional statement if there is an error
    if (err < 0){
        //
        LOG_ERR("Creating new identity failed (err %d)\n", err);

        // Stop the program since we got an error
        return err;
    }

    // Register the connection callbacks with the added RSSI handler
    static struct bt_conn_cb conn_callbacks = {
        .connected = on_connected,
        .disconnected = disconnected,
        .le_param_updated = rssi_updated,  // Adjust according to correct signature
    };

    //
    bt_conn_cb_register(&conn_callbacks);

    // Enabling bluetooth
    err = bt_enable(NULL);

    // Creating a conditional statement if there is an error
    if (err < 0){
        // If the device could not initialize bluetooth
        LOG_ERR("Bluetooth initialize failed (err %d)\n", err);

        // Stop the program since we got an error
        return err;
    }

    // Adding to the logging console module (we create/initiailize the bluetooth)
    LOG_INF("Bluetooth Enabled! . . . ");
    LOG_INF("Starting Advertising . . . ");

    //
    err = bt_le_adv_start(
        adv_param, advertise_data, ARRAY_SIZE(advertise_data),
        scan_data, ARRAY_SIZE(scan_data));

    // Creating a conditional statement if there is an error
    if (err < 0){
        //If the device could not advertise data
        LOG_ERR("Bluetooth advertisement start failed (err %d)\n", err);

        // Stop the program since we got an error
        return err;
    }

    // If the advertise was a success then log 
    LOG_INF("Advertise started!");

    

    // Creating an infinite loop
    while (1){
        // Have a delay
        k_sleep(K_SECONDS(1));

        // RSSI
        if (our_conn) {
            struct bt_conn_info info;
            // Retrieve connection info including RSSI
            if (bt_conn_get_info(our_conn, &info) == 0) {
                // Print the RSSI value
                LOG_INF("Current RSSI: %d dBm", info.le.rssi);
                LOG_INF("Hi %d", cnt);
            } else {
                LOG_ERR("Failed to get connection info");
            }
        } else {
            LOG_INF("Not connected");
        }

        // Log the information that it is alive
        LOG_INF("I'm alive!!!");
        cnt = cnt + 1;
    }
    return 0;
}