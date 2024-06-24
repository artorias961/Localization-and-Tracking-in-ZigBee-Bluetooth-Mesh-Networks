# Personal Notes
These [samples](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.6.1/nrf/samples/zigbee.html) are designed to work with Nordic Semiconductor's development kits, such as the nRF52840 DK or the nRF5340 DK. 

- Zigbee Light Bulb: This sample demonstrates how to create a **Zigbee router node**. It implements a simple light bulb device that can be controlled by a Zigbee coordinator.

- Zigbee Light Switch: This sample shows how to implement a **Zigbee end device** that can control a Zigbee light bulb. This is useful for understanding how to interact with router nodes in the network.

- Zigbee Network Coordinator: Although this is for the **coordinator role**, it provides a example of how to set up a Zigbee coordinator to manage the network.


## Zigbee Cluster Library
The Zigbee Cluster Library (ZCL) provides a standardized way to implement device functionality within a Zigbee network. It defines collections of related attributes and commands, known as clusters, which standardize how devices such as sensors, lights, and controllers interact with each other. ZCL clusters handle common functionalities like on/off control, temperature measurement, and more, allowing interoperability between devices from different manufacturers. By using ZCL, developers can ensure their devices can communicate effectively within the Zigbee ecosystem.

### Endpoints
Logical entities within a Zigbee device, each representing a distinct application or function. Each endpoint has a unique identifier and a set of clusters it supports. For example, a light bulb might have an endpoint for basic on/off control and another for dimming.

### Clusters
Groups of attributes and commands that define a specific functionality. For example, the On/Off cluster includes attributes like OnOff state and commands like On, Off, and Toggle.


### Attributes
Data points that describe the state or configuration of a Zigbee device. Each cluster contains a list of attributes. For example, an On/Off cluster might have an OnOff attribute indicating whether the device is on or off.
Commands

### Callbacks
Functions that handle events or responses in the Zigbee network. For instance, when a command is received, a callback function processes the command and performs the corresponding action.

### ZCL Macros
A structure that holds the device's configuration and state, including all endpoints and clusters it supports. ZCL Declarations: Macros that simplify the definition of attributes, clusters, and endpoints. For example, ZB_ZCL_DECLARE_ON_OFF_ATTRIB_LIST declares the attribute list for an On/Off cluster.

### Zigbee Application Framework




## Scanning local devices Methods
### Enable the Interface
This function brings a network interface up, enabling it to start communication. Before performing any network operations, the interface must be in an "up" state.
```c
#include <zephyr/net/net_if.h>

void enable_network_interface(struct net_if *iface) {
    int result = net_if_up(iface);
    if (result == 0) {
        printk("Interface brought up successfully.\n");
    } else {
        printk("Failed to bring interface up.\n");
    }
}
```


### Set fixed Channel
This function sets the operating channel for the IEEE 802.15.4 radio. The channel determines the frequency on which the radio will communicate.
```c
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/ieee802154_radio.h>

void set_ieee802154_channel(struct net_if *iface, uint16_t channel) {
    struct ieee802154_req_params params = {
        .channel = channel
    };

    int result = net_mgmt(NET_REQUEST_IEEE802154_SET_CHANNEL, iface, &params, sizeof(params));
    if (result == 0) {
        printk("Channel set successfully to %u.\n", channel);
    } else {
        printk("Failed to set channel.\n");
    }
}
```

### Energy Detection
This function performs an energy detection scan on the current channel. It measures the energy level to determine the presence of other transmissions or interference.
```c
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/ieee802154_radio.h>

void perform_energy_detection(struct net_if *iface) {
    int result = net_mgmt(NET_REQUEST_IEEE802154_ED_SCAN, iface, NULL, 0);
    if (result == 0) {
        printk("Energy detection scan started.\n");
    } else {
        printk("Failed to start energy detection scan.\n");
    }
}
```


### Promiscuous Mode
This function enables or disables promiscuous mode for the IEEE 802.15.4 radio. In promiscuous mode, the radio listens to all packets on the channel, not just those addressed to it.
```c
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/ieee802154_radio.h>

void set_promiscuous_mode(struct net_if *iface, bool enable) {
    int result = net_mgmt(NET_REQUEST_IEEE802154_SET_PROMISCUOUS_MODE, iface, &enable, sizeof(enable));
    if (result == 0) {
        printk("Promiscuous mode %s.\n", enable ? "enabled" : "disabled");
    } else {
        printk("Failed to set promiscuous mode.\n");
    }
}
```

## Energy Detection vs Promiscuous Mode:
### Promiscuous Mode
- **Functionality**: In promiscuous mode, the radio listens to all packets on the network, irrespective of their destination address. This mode bypasses standard filtering mechanisms, allowing the device to capture every packet, including those not addressed to it.
- **Usage**: Promiscuous mode is particularly useful for debugging and network analysis. It allows network administrators to monitor all traffic within the network, which is crucial for troubleshooting and ensuring network security.
- **Example**: Enabling promiscuous mode on an nRF IEEE 802.15.4 radio driver can be done using the *nrf_802154_promiscuous_set* function.
- **Reference**:
   - [Silicon Labs Doc API](https://docs.silabs.com)
   - [Nordic Semiconductors Wi-Fi: Promiscuous](https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/samples/wifi/promiscuous/README.html)
      - Can be used in BLE to monitor traffic!!!


### Energy Detection
- **Functionality**: Energy detection mode measures the signal strength or energy level on a given channel. This mode is used to detect the presence of other transmissions or interference in the channel.

- **Usage**: Energy detection is useful for network setup and optimization. By measuring the energy levels across different channels, devices can select the least congested channel, enhancing communication reliability and performance.

- **Example**: Performing an energy detection scan on a Zephyr-supported IEEE 802.15.4 device can be done using the net_mgmt API.

- **Reference**:
   - [Zephry Documentations](https://docs.zephyrproject.org/latest/connectivity/networking/api/ieee802154.html)



# References
- Bluetooth
   - [Bluetooth Protocols](https://docs.zephyrproject.org/latest/connectivity/bluetooth/bluetooth-arch.html)

- Zigbee
   - [Samples](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.6.1/nrf/samples/zigbee.html)
   - [Zigbee cluster commands for ZCL send packet](https://community.home-assistant.io/t/zigbee-cluster-commands-broadcast/293700)

- ZBOSS and Zigbee Cluster Library
   - [ZCL naming convention](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/zboss_api_doc/group__zcl__api.html)
   - [Common ZCL terms and definitions](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/zboss/3.11.2.0/using_zigbee__z_c_l.html)

- Network Management
   - [Nordoc Documentation](https://docs.nordicsemi.com/bundle/ncs-2.4.0/page/zephyr/connectivity/networking/api/net_mgmt.html)

- IEEE 802.15.4
   - Scanning local devices using radio waves (BLE) where most of them uses the *net_mgmt.h* library.

      - Initiates communications (enable interface)
         - This function brings a network interface up, enabling it to start communication. Before performing any network operations, the interface must be in an "up" state.
         - [More about the functions](https://docs.zephyrproject.org/apidoc/latest/group__net__if.html#ga02644cc623fc7a8878c17d54984e4210)


      - Fixed radio channel (set the channel)
         - This one can only monitor one channel out of the 16 for IEEE 802.15.4
         - [net_mgmt](https://github.com/zephyrproject-rtos/zephyr/blob/main/include/zephyr/net/net_mgmt.h)
         - [About the functions](https://docs.nordicsemi.com/bundle/ncs-latest/page/zephyr/doxygen/html/group__ieee802154__driver.html)

      - Energy detection
         - This one can monitor all the channels for IEEE 802.15.4 
         - [ieee802154_radio](https://github.com/zephyrproject-rtos/zephyr/blob/main/include/zephyr/net/ieee802154_radio.h) 

      - Promiscuous Mode
         - In promiscuous mode, the radio listens to all packets on the channel, not just those addressed to it.
         - [net_mgmt](https://github.com/zephyrproject-rtos/zephyr/blob/main/include/zephyr/net/net_mgmt.h)

