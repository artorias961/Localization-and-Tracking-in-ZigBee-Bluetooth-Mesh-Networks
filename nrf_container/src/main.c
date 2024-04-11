/*
Christopher Morales

Purpose is to create a simple protocol that can get RSSI from a device using GATT.

Link 1: https://docs.zephyrproject.org/latest/connectivity/bluetooth/api/connection_mgmt.html
Link 2: https://lists.zephyrproject.org/g/devel/topic/ble_read_rssi_of_connected/16760867
Link 3: https://devzone.nordicsemi.com/f/nordic-q-a/102148/reading-rssi-with-ncs-with-softdevice-controler-does-not-work
Link 4: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrfxlib/softdevice_controller/doc/api.html#c.sdc_hci_cmd_sp_read_rssi
Link 5 (MUST): https://github.com/nrfconnect/sdk-nrfxlib
Link 6 (MUST): https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrfxlib/README.html
Link 7: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/libraries/networking/nrf_cloud_rest.html
Link 8: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/1.9.99-dev1/nrfxlib/nrf_dm/README.html 
Link 9: https://devzone.nordicsemi.com/f/nordic-q-a/80182/how-to-add-a-ncs-nrf-library-to-my-project 
LINK 10: https://devzone.nordicsemi.com/f/nordic-q-a/108438/add-open-source-drivers-libraries-to-projects-using-cmake-and-externalproject_add
*/


// ============================================================================================
// Setup and Initialization
// ============================================================================================

// General libraries for zephry
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

// Libraries for the bluetooth
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/addr.h>
#include <zephyr/bluetooth/conn.h>

// Libraries to use bit modification/padding
#include <zephyr/sys/byteorder.h>

// Libraries to get RSSI using the nrfxlib
// #include <soc/nrfx_coredep.h>
// #include <ncs_version.h>

// ============================================================================================
// Global Variables
// ============================================================================================

// Creating the LOG Module 
LOG_MODULE_REGISTER(zb_monitor, LOG_LEVEL_DBG);

// Declaring global variables (can not be changed)
#define BT_UUID_SST_SVC_BYTES  BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x18282, 0x785fdeadbeef)
#define BT_UUID_SST_SVC  BT_UUID_DECLARE_128(BT_UUID_SST_SVC_BYTES)

// This will only get data when the board is connected our_conn can work but need to create conditions
uint16_t global_conn_handle;

// ============================================================================================
// Declaring functions
// ============================================================================================
int my_sst_svc_gatt_notify(uint32_t sensor_value);
void on_connected(struct bt_conn *conn, uint8_t error);
void on_disconnected(struct bt_conn *conn, uint8_t reason);
int api_ble_gatt_notify(uint32_t sensor_value);

// ============================================================================================
// BLUETOOTH PROTOCOLS START
// ============================================================================================
// Creating the bluetooth parameter advertising interval
static struct bt_le_adv_param* adv_param = BT_LE_ADV_PARAM(
    (BT_LE_ADV_OPT_CONNECTABLE|BT_LE_ADV_OPT_USE_IDENTITY), /* Connectable advertising and use identity address */
    800, /*Min Advertising Interval 500ms (800*0.625ms) */
    801, /*Max Advertising Interval 500.625ms (801*0.625ms)*/
    NULL); /* Set to NULL for undirected advertising*/


// Creating advertising protocols and name
static const struct bt_data adv_data[] = {
    /* STEP 3.1 - Set the flags and populate the device name in the advertising packet */
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, (sizeof(CONFIG_BT_DEVICE_NAME) - 1)),
};


// The UUID for the nRF52-dk board
static const struct bt_data scan_data[] = {
    /* STEP 3.2.2 - Include the 16-bytes (128-Bits) UUID of our service in the scan response packet */
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_SST_SVC_BYTES), // was BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_SST_SVC_BYTES)
};

// Begin defining BLE GATT SST (Sensor Streaming)
#define BT_UUID_SST_MYSENSOR  BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x00001526, 0x1212, 0xefde, 0x1523, 0x785fdeadbeef)) // 0x785fdeadbeef

// Declaring global variables that can be change
static bool notify_mysensor_enabled;

/// @brief Define the configuration change callback function for the MYSENSOR characteristic
static void sst_ccc_mysensor_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    notify_mysensor_enabled = (value == BT_GATT_CCC_NOTIFY);
}

// The entire bluetooth protocol system
BT_GATT_SERVICE_DEFINE(
    sst_service,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_SST_SVC),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_SST_MYSENSOR,
        BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_NONE, 
        NULL, 
        NULL,
        NULL),
    BT_GATT_CCC(
        sst_ccc_mysensor_cfg_changed,
        BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
        
);

// ============================================================================================
// BLUETOOTH FUNCTIONS
// ============================================================================================
/// @brief Notify clients on the sensor via GATT
/// @param sensor_value 

// A global variable to declare the status of the board (Essentially our real BLE manager)
struct bt_conn* our_conn = NULL;

int my_sst_svc_gatt_notify(uint32_t sensor_value)
{
    // Creating an variable to store the error
    int error;

    // Creating a conditional statement to determine if there is an error
    if (!notify_mysensor_enabled) {
        // If so, then return the following
        return -EACCES;
    }
    // &sst_service.attrs is the Service Attribute table: 0 = Service, 1 = Primary service, 2 = Characteristic
    error = bt_gatt_notify(NULL, &sst_service.attrs[2], &sensor_value, sizeof(sensor_value));

    // An error for the Bluetooth GATT Notify service
    if (error < 0) {
        // Log the module
        LOG_ERR("Warning: bt_gatt_notify failed sending %d with error: %d", sensor_value, error);
    }
    else {
        // Else, log the sensor value
        LOG_DBG("Sent sensor value: %d", sensor_value);
    }
    return error;
}

void on_connected(struct bt_conn *conn, uint8_t error){
    // Creating a conditional statement if an error occur with the bluetooth connection
    if (error) {
        // Logging the module
        LOG_ERR("Connection error %d", error);
        
        // Stop the program
        return;
    }
    // Logging the module
    LOG_WRN("Connected");
    
    // The connection of the bluetooth device
    our_conn = bt_conn_ref(conn);

    // Save the connection handle #todo <----------------------------------------------------------------------------------------------------- 
    // global_conn_handle = sdc_
}

void on_disconnected(struct bt_conn *conn, uint8_t reason){
    // Logging the module
    LOG_WRN("Disconnected. Reason %d", reason);
    
    // Disconnection of the bluetooth device
    bt_conn_unref(our_conn);
}

struct bt_conn_cb bluetooth_connection_callbacks = {
    .connected = on_connected,
    .disconnected = on_disconnected,
};


int api_ble_gatt_notify(uint32_t sensor_value){
    // Creating a variable to to store any error values
    int error;

    // Creating a conditional statement to determine if there is an error
	if (!notify_mysensor_enabled) {
        // Return the status of the access/status
		return -EACCES;
	}

    // &sst_service.attrs is the Service Attribute table: 0 = Service, 1 = Primary service, 2 = Characteristic
	error = bt_gatt_notify(NULL, &sst_service.attrs[2], &sensor_value, sizeof(sensor_value));
    
    // Creating a conditional statement if an error occurs
    if (error < 0) {
        // Logging the module
        LOG_ERR("(BLE NOTIFICATION) Warning: bt_gatt_notify failed sending %d with error: %d", sensor_value, error);
    }
    else {
        // Logging the module
        LOG_DBG("(BLE NOTIFICATION) Sent sensor value: %d", sensor_value);
    }

    // Return the error
    return error;
}

// ============================================================================================
// RSSI RELATED
// ============================================================================================

static void read_conn_rssi(uint16_t handle, int8_t *rssi)
{
    // Declaration of buffer variables for the command and response
	struct net_buf *buf, *rsp = NULL;

    // Pointers for command and response structures
	struct bt_hci_cp_read_rssi *cp;
	struct bt_hci_rp_read_rssi *rp;

    // Variable for error handling
	int err;

    // Creating a command buffer for the READ RSSI operation
	buf = bt_hci_cmd_create(BT_HCI_OP_READ_RSSI, sizeof(*cp));

    // If buffer allocation fails, print an error message and exit the function
	if (!buf) {
        // Logging the module
		LOG_ERR("Unable to allocate command buffer\n");

        // Stop the program
		return;
	}

    // Adding the command payload to the buffer and setting the connection handle
	cp = net_buf_add(buf, sizeof(*cp));
	cp->handle = sys_cpu_to_le16(handle);

    // Sending the command synchronously and receiving a response
	err = bt_hci_cmd_send_sync(BT_HCI_OP_READ_RSSI, buf, &rsp);

    // If an error occurs, extract the error reason and print it, then exit the function
	if (err) {
		uint8_t reason = rsp ?
			((struct bt_hci_rp_read_rssi *)rsp->data)->status : 0;
		LOG_ERR("Read RSSI err: %d reason 0x%02x\n", err, reason);

        // Stop the program
		return;
	}

    // Extracting the RSSI value from the response
	rp = (void *)rsp->data;
	*rssi = rp->rssi;

    // Releasing the memory allocated for the response buffer (essentially properly managing memory)
	net_buf_unref(rsp);
}

// ============================================================================================
// MAIN FUNCTION
// ============================================================================================

void main(void){
    // Creating a variable that will be able to detect an error
    int error;

    int8_t rssi_value;

    // Creating the counter to determine if notifications is updating (more for the logging module)
    uint32_t counter = 0;

    // Logging the module
    LOG_DBG("Setting up bluetooth . . . ");

    // Getting the bluetooth low energy device address (MAC Address)
    bt_addr_le_t ble_device_address;

    // Getting a boolean value if an error occur from the BLE device (converts the string to binary values, the bt function) ["ADDRESS", "TYPE", &ADDRESS]
    error = bt_addr_le_from_str("FF:EE:DE:AD:BE:EF", "random", &ble_device_address);

    // Creating an conditional statement if an error occur
    if(error){
        // Logging the module
        LOG_ERR("(BLE MANAGER) Invalid Bluetooth Address (error %d)", error);

        // Stop the program
        return;
    }

    // Creating a new identity for the advertisement
    error = bt_id_create(&ble_device_address, NULL);

    // Creating a conditional statement if the connectioned failed
    if (error < 0){
        // Logging the module
        LOG_ERR("(BLE MANAGER) Creating new identity failed (error %d)", error);

        // Stop the program
        return;
    }

    // Register callbacks to monitor the state of the connections (calling the struct if the BLE device is connected or not)
    bt_conn_cb_register(&bluetooth_connection_callbacks);

    // Getting a value to determine if an error occur or not
    error = bt_enable(NULL);

    // Determining if initializing bluetooth is a success or not
    if (error < 0){
        // Logging the module
        LOG_ERR("Bluetooth init failed (err %d)", error);

        // Stop the application
        return;
    }

    // Logging the module
    LOG_DBG("Bluetooth enabled!!! Starting advertising. . . ");

    // Start advertising reponse and scan response
    error = bt_le_adv_start(adv_param, adv_data, ARRAY_SIZE(adv_data), scan_data, ARRAY_SIZE(scan_data));

    // Creating an conditional statement if the values are wrong/incorrect
    if (error < 0){
        // Logging the module
        LOG_ERR("(BLE MANAGER) Bluetooth advertisement failed (error %d)", error);

        // Stop the program
        return;
    }

    // Logging the module
    LOG_DBG("Advertising started!!!");

    // Then do these operations
    while(1){
        LOG_WRN("==========================================================");
        // printk("Hi %d", counter);
        LOG_INF("Hi %d", counter);
        counter++;
        
        k_sleep(K_SECONDS(1));
        read_conn_rssi(global_conn_handle, &rssi_value);
        LOG_WRN("RSSI: %d dBm", rssi_value);

        LOG_WRN("==========================================================");
        k_sleep(K_SECONDS(1));
    }
}



/*
Option 1: The edge server be the control of time for the network topology but there will be delay when transimitting data to the network.
Option 2: 
*/














