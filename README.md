# Personal Notes

## How the nrf53dk dual core works

The dual-core architecture of the nRF53 series features an Application core and a Network core, both based on the Cortex-M33. The Application core runs user applications, while the Network core handles protocol stacks like Bluetooth or Zigbee. They communicate via Inter-Processor Communication (IPC). This design allows for efficient task separation and enhanced performance, with the Application core managing main application logic and the Network core dedicated to network operations.

<p align="center">
  <img  src="https://github.com/artorias961/thesis/assets/54751574/5045080a-6a2b-4766-a8b5-09f9f6991623">
</p>





## General information of Zigbee samples

These [samples](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.6.1/nrf/samples/zigbee.html) are designed to work with Nordic Semiconductor's development kits, such as the nRF52840 DK or the nRF5340 DK. 

- Zigbee Light Bulb: This sample demonstrates how to create a **Zigbee router node**. It implements a simple light bulb device that can be controlled by a Zigbee coordinator.

- Zigbee Light Switch: This sample shows how to implement a **Zigbee end device** that can control a Zigbee light bulb. This is useful for understanding how to interact with router nodes in the network.

- Zigbee Network Coordinator: This is for the **coordinator role**, it provides a example of how to set up a Zigbee coordinator to manage the network.

- Zigbee Shell: The Zigbee Shell sample is designed to provide an interactive command-line interface (CLI) for configuring and managing a Zigbee network **(Zigbee router or Coordinator)**
[text](about:blank#blocked)
- Zigbee Template: The Zigbee Template sample provides a minimal implementation of a **Zigbee router**, serving as a starting point for custom Zigbee applications





## Zigbee Cluster Library
The Zigbee Cluster Library (ZCL) provides a standardized way to implement device functionality within a Zigbee network. It defines collections of related attributes and commands, known as clusters, which standardize how devices such as sensors, lights, and controllers interact with each other. ZCL clusters handle common functionalities like on/off control, temperature measurement, and more, allowing interoperability between devices from different manufacturers.

### Endpoints
Logical entities within a Zigbee device, each representing a distinct application or function. Each endpoint has a unique identifier and a set of clusters it supports. For example, a light bulb might have an endpoint for basic on/off control and another for dimming.

### Clusters
Groups of attributes and commands that define a specific functionality. For example, the On/Off cluster includes attributes like OnOff state and commands like On, Off, and Toggle.


### Attributes
Data points that describe the state or configuration of a Zigbee device. Each cluster contains a list of attributes. For example, an On/Off cluster might have an OnOff attribute indicating whether the device is on or off.

### Commands
Commands are actions that can be performed on a cluster. For example, the On/Off cluster includes commands like On, Off, and Toggle. Commands allow for controlling the state of attributes within a cluster.



### Callbacks
Functions that handle events or responses in the Zigbee network. For instance, when a command is received, a callback function processes the command and performs the corresponding action.

### ZCL Macros
A structure that holds the device's configuration and state, including all endpoints and clusters it supports. ZCL Declarations: Macros that simplify the definition of attributes, clusters, and endpoints. For example, ZB_ZCL_DECLARE_ON_OFF_ATTRIB_LIST declares the attribute list for an On/Off cluster.

### Device Context
Device context is a structure that holds the device's configuration and state, including all endpoints and clusters it supports. It is essential for maintaining the operational state of the device and ensuring that it can respond to network commands appropriately.








## IEEE 802.15.4 Library
The IEEE 802.15.4 standard is a key element in wireless communication, specifically designed for low-rate wireless personal area networks (LR-WPANs).



## Zephyr API
- **PHY level Driver API**: To be implemented by all drivers, ensuring integration with the native L2 stack and basic support for OpenThread.

- **MAC level Extensions**: Optional extensions to offload performance-critical or timing-sensitive aspects of the MAC layer to the hardware.

### Physical Layer (PHY)
The layer responsible for the transmission and reception of data bits over a physical medium. It includes details like the modulation technique and frequency band.


### Medium Access Control Layer (MAC)
This layer manages protocol access to the physical network medium. It includes functions like channel access, addressing, frame validation, and acknowledgment.

### Personal Area Network (PAN)
A network organized around an individual person within a single building. It typically ranges up to 10 meters.

### Coordinator 
 A device that initializes, maintains, and manages the network. It can assign addresses to other devices in the network.

### Endpoint
A logical interface within a device, represented by an endpoint identifier (EPID). Endpoints are used to communicate specific services or capabilities of the device.

### Common key Functions and Macros
- **IEEE802154_CONFIG_RX_SLOT_NONE**
   - Used to cancel any previously configured RX slot

- **IEEE802154_CONFIG_RX_SLOT_OFF**
   - Keeps RX off when the driver starts or switches it off immediately when the driver is up

- **IEEE802154_BROADCAST_ADDRESS**
   - Represents the broadcast short address used in the network

- **IEEE802154_FCS_LENGTH**
   - Defines the frame check sequence length, typically 2 bytes

- **IEEE802154_MAX_PHY_PACKET_SIZE**
   - The maximum PHY packet size, usually 127 bytes



## ZBoss Library
ZBOSS (Zigbee Open Source Stack) is a high-performance Zigbee software protocol stack developed by Nordic Semiconductor. It ensures interoperability, customization, testing, and optimization of Zigbee solutions. ZBOSS is designed to be resource-efficient, cross-platform compatible, and thoroughly tested for real-world interoperability. It supports the Zigbee 3.0 specification, all Zigbee device roles, and is certified by the Zigbee Alliance for compliant platforms [Nordic Documentation](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/zboss_api_doc/zigbee_features.html) [Nordic Documentation](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/zboss_api_doc/zigbee_devguide.html).


### Suporroted Protocols
Compatible with Zigbee 3.0 Specification, IEEE 802.15.4-2011, Zigbee PRO Feature Set, and Zigbee Cluster Library.




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




















# PRJ Issues (SDK 2.6.1 VERSION ONLY)

## CONFIG_NET_L2_IEEE802154_MGMT
Is assigned in a configuration file, but is not directly user confiirable (has no prompt). It gets its calue indirectly from other symbols and the link the terminal provides is [Link](https://docs.zephyrproject.org/latest/kconfig.html#CONFIG_NET_L2_IEEE802154_MGMT). *Which is a dead link so i cant tell what variables i need for my network management*. So i modify the search bar with this [Link](https://docs.zephyrproject.org/latest/kconfig.html#CONFIG_NET_L2_IEEE802154_MGMT).

If enable CONFIG_NET_L2_IEEE802154_MGMT then an error will be prompted when compiling and the function does not exist in ZephyrRTOS anymore. Need to find another way to get **NET_REQUEST_IEEE802154_ED_SCAN** or the new naming variable. 


## No crypto suite for zignee stack has been selected

Make sure the **build configuration** is set to one of the prj else mess with the cpu board flasher

## Nordic Website Error List

[Issues with 2.6.1 and resolving them in 2.7.0](https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/releases_and_maturity/known_issues.html#known-issues)
































# References

- [nrf5340dk general documentation](https://docs.nordicsemi.com/bundle/ug_nrf5340_dk/page/UG/dk/intro.html)
- [nrf53 Series bundle documentation](https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/device_guides/nrf53/index.html#ug-nrf5340)

- Visual Studio SDK
   - [General build and configuration system](https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/config_and_build/config_and_build_system.html)


- Bluetooth
   - [Bluetooth Protocols](https://docs.zephyrproject.org/latest/connectivity/bluetooth/bluetooth-arch.html)

- [ZBoss Base API](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/zboss_api_doc/zigbee_devguide.html)

- Zigbee
   - [Samples](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.6.1/nrf/samples/zigbee.html)
   - [Zigbee cluster commands for ZCL send packet](https://community.home-assistant.io/t/zigbee-cluster-commands-broadcast/293700)

- Zigbee Over the Air (FOTA)
   - [Link](https://docs.nordicsemi.com/bundle/ncs-2.6.1/page/nrf/libraries/zigbee/zigbee_fota.html#lib-zigbee-fota)

- Sending information over Zigbee
   - [Buffer API (packet buffer pool)](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/zboss_api_doc/group__buf.html)

- ZBOSS and Zigbee Cluster Library
   - [ZCL naming convention](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/zboss_api_doc/group__zcl__api.html)
   - [Common ZCL terms and definitions](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/zboss/3.11.2.0/using_zigbee__z_c_l.html)
   - [Zigbee endpoint logger](https://docs.nordicsemi.com/bundle/ncs-2.6.99-cs1/page/nrf/libraries/zigbee/zigbee_logger_eprxzcl.html)

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

- Cryptography (encryption)
   - [AES CTR Nordic Documentation](https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/samples/crypto/aes_ctr/README.html)

- Multi Threading
   - [Kthread Nordic Documentation](https://docs.nordicsemi.com/bundle/ncs-2.2.0/page/zephyr/kernel/services/threads/index.html)

- REST API
   - [nRF Cloud REST API Nordic Documentation](https://docs.nordicsemi.com/bundle/nrf-cloud/page/APIs/REST/RESTOverview.html)

- Zigbee Errors Solution
   - [Link 1](https://devzone.nordicsemi.com/f/nordic-q-a/60644/combine-zigbee-roles-or-start-a-network-of-two-devices)

# Stuff to do 



Always scanning and sending (whenever someone ask to joion the commincuaton) (BLE NOTIFY ESSENTAILLY)




