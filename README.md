# Personal Notes

## Deciding a network protocol
[OpenThread](https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/protocols/thread/overview/index.html) is an open-source implementation of the Thread protocol, released and maintained by Nest (Google). It offers flexible development and rapid prototyping with robust security and broad platform support. OpenThread is free to use and benefits from a strong open-source community and Google support.

[Thread](https://www.nordicsemi.com/Products/Wireless/Thread) is a proprietary protocol maintained by the Thread Group. It is designed for general IoT applications, providing robust security, IPv6-based communication, and reliable mesh networking. However, it requires membership fees for full access.

[Matter](https://www.nordicsemi.com/Products/Technologies/Matter), developed by the Connectivity Standards Alliance (CSA), aims to provide interoperable smart home and building devices. It combines open-source and proprietary components, offering strong security and broad support across different platforms. While it is generally free to use, some certification costs may apply.

[Zigbee](https://www.nordicsemi.com/Products/Wireless/Zigbee), developed by the Zigbee Alliance, is a widely adopted protocol for home automation and industrial applications. It uses the IEEE 802.15.4 standard for mesh networking and offers robust security with AES-128 encryption. Zigbee is supported by many vendors and platforms, and while it is free to use, some components may have proprietary elements and certification costs.



<p align="center">

| Feature              | OpenThread                       | Thread                          | Matter                                   | Zigbee                                   |
|----------------------|----------------------------------|---------------------------------|------------------------------------------|------------------------------------------|
| **Nature**           | Open-source implementation       | Protocol                        | Protocol                                 | Protocol                                 |
| **Development**      | Released and maintained by Nest (Google) | Maintained by Thread Group      | Developed by the Connectivity Standards Alliance (CSA) | Developed by the Zigbee Alliance         |
| **Licensing**        | Open-source (BSD 3-Clause License) | Proprietary, requires membership for full access | Open-source, proprietary components      | Open standard, some components may be proprietary |
| **Certification**    | Thread 1.1 compliant component   | Thread-certified                | Matter-certified                         | Zigbee-certified                         |
| **Platform Support** | Flexible, supports various hardware platforms | Specific to Thread devices      | Broad support across different platforms | Supported by many vendors and platforms |
| **Community**        | Open-source community, Google support | Industry consortium             | Industry consortium (CSA)                | Industry consortium (Zigbee Alliance)    |
| **Cost**             | Free to use                      | Requires membership fees        | Free to use, some certification costs    | Free to use, some certification costs    |
| **Use Case**         | Flexible development and rapid prototyping | General IoT applications        | Interoperable smart home and building devices | Home automation, industrial applications |
| **Security**         | Robust security with AES-128 encryption | Robust security with AES-128 encryption | Strong security with built-in encryption | Robust security with AES-128 encryption |
| **Communication**    | IPv6-based, low-power, mesh network | IPv6-based, low-power, mesh network | IPv6-based, supports multiple network layers | IEEE 802.15.4, mesh network              |

</p>


## How the nrf53dk dual core works

The dual-core architecture of the nRF53 series features an Application core and a Network core, both based on the Cortex-M33. The Application core runs user applications, while the Network core handles protocol stacks like Bluetooth or Zigbee. They communicate via Inter-Processor Communication (IPC). This design allows for efficient task separation and enhanced performance, with the Application core managing main application logic and the Network core dedicated to network operations.

<p align="center">
  <img  src="https://github.com/artorias961/thesis/assets/54751574/5045080a-6a2b-4766-a8b5-09f9f6991623">
</p>

# ZephyrRTOS
The [Nordic Zephyr RTOS](https://docs.nordicsemi.com/bundle/ncs-latest/page/zephyr/index.html) is an open-source real-time operating system tailored for Nordic Semiconductor’s nRF series of SoCs. It provides a scalable, feature-rich platform for developing low-power, connected devices, supporting multiple communication protocols like Bluetooth, Zigbee, and Thread. The RTOS includes essential features such as multi-threading, inter-thread communication, and hardware abstraction, enabling efficient and reliable application development for IoT solutions.



## K_THREAD
[k_thread](https://docs.zephyrproject.org/latest/kernel/services/threads/index.html) is part of the Zephyr RTOS, designed to manage threads, which are the basic units of execution. It provides APIs for creating, configuring, and managing threads, allowing developers to implement concurrent tasks efficiently. Key functionalities include thread creation, setting priorities, suspension, resumption, and inter-thread communication.

## K_THREAD_STACK
The [K_THREAD_STACK](https://docs.zephyrproject.org/latest/kernel/services/threads/index.html#thread-stack-objects) macro in Zephyr RTOS is used to define the stack memory for a thread. It allocates and initializes a memory region that the thread will use for its stack, which is necessary for storing the thread's context, including local variables, function calls, and return addresses.
```c
K_THREAD_STACK_DEFINE(stack_area, stack_size);
```
- Note:
   - stack_area: The name of the stack area
   - stack_size: The size of the stack in bytes



## Symmetric Multiprocessing
The Zephyr Project documentation on [Symmetric Multiprocessing (SMP)](https://docs.zephyrproject.org/latest/kernel/services/smp/smp.html) explains how Zephyr supports multiple CPUs running application code simultaneously. It covers the configuration, synchronization using spinlocks, SMP boot process, interprocessor interrupts, and managing per-CPU data. The document highlights the differences between uniprocessor and multiprocessor systems, emphasizing the use of spinlocks over traditional interrupt locks for critical sections, and details APIs for managing CPU-specific tasks and interprocessor communication. In our case, we have 1 core for application and another core for net. If it was 2 cores for application and 1 core for net then the symmetric multiprocessing will work.




# Thread 
Thread is an IPv6-based wireless networking protocol designed for low-power, secure, and scalable mesh networks. It allows seamless integration with IP networks without needing proprietary gateways.

<p align="center">
  <img  src="https://github.com/user-attachments/assets/d75d89c1-39c5-4f9f-be04-5b87cc8f636b">
</p>


## What is the difference between Thread and OpenThread
Thread is a low-power, IPv6-based wireless networking protocol designed for IoT applications. It aims to create reliable and secure mesh networks, primarily for smart home and building automation. Developed and maintained by the Thread Group, Thread enables devices to form a self-healing and scalable network, uses IPv6 addressing for large-scale deployment and internet connectivity, and is optimized for battery-operated devices. Thread also provides robust security using AES-128 encryption and secure commissioning while ensuring interoperability among devices from different manufacturers.

OpenThread, on the other hand, is an open-source implementation of the Thread networking protocol, released by Nest (now part of Google). OpenThread offers the same core functionalities as the Thread protocol but with the added benefits of being open-source. It is freely available under the BSD 3-Clause license, allowing developers to view, modify, and contribute to the source code. OpenThread is certified as a Thread 1.1 compliant component, ensuring full compatibility with the Thread specification. It can run on various hardware platforms and integrate with different operating systems, supported by a vibrant community and continuous development from Google and other contributors. Additionally, OpenThread comes with extensive documentation, development tools, and examples to help developers get started quickly.

In summary, while Thread is a standardized protocol for creating low-power, secure, and reliable IoT mesh networks, OpenThread is its open-source counterpart offering flexibility, accessibility, and robust community support. Both aim to provide reliable and secure mesh networking for IoT devices, but OpenThread's open-source nature makes it particularly appealing for flexible development and rapid prototyping.

<p align="center">

| Feature            | Thread                          | OpenThread                       |
|--------------------|---------------------------------|----------------------------------|
| **Nature**         | Protocol                        | Open-source implementation       |
| **Development**    | Maintained by Thread Group      | Released and maintained by Nest (Google) |
| **Licensing**      | Proprietary, requires membership for full access | Open-source (BSD 3-Clause License) |
| **Certification**  | Thread-certified                | Thread 1.1 compliant component   |
| **Platform Support** | Specific to Thread devices      | Flexible, supports various hardware platforms |
| **Community**      | Industry consortium             | Open-source community, Google support |
| **Cost**           | Requires membership fees        | Free to use                      |
| **Use Case**       | General IoT applications        | Flexible development and rapid prototyping |

</p>


## General information for Thread Samples
- [OpenThread Overview](https://developer.nordicsemi.com/nRF_Connect_SDK/doc-legacy/2.6.1/nrf/protocols/thread/overview/index.html): Details on using the OpenThread stack integrated with Zephyr. OpenThread is an open-source implementation of the Thread networking protocol, provided by Google and integrated with Zephyr RTOS in the nRF Connect SDK. This section provides a comprehensive overview of how OpenThread is used within the SDK, including its benefits and key features.
 
- [Configuring Thread](https://developer.nordicsemi.com/nRF_Connect_SDK/doc-legacy/2.6.1/nrf/protocols/thread/configuring.html): Guidelines for configuring Thread in the nRF Connect SDK. This section details the necessary steps and configurations to set up Thread in the nRF Connect SDK. It covers how to enable Thread support, configure network parameters, and adjust various settings to optimize performance and functionality.
 
- [Pre-built Libraries](https://developer.nordicsemi.com/nRF_Connect_SDK/doc-legacy/2.6.1/nrf/protocols/thread/prebuilt_libs.html): Access to pre-compiled Thread libraries. The nRF Connect SDK includes pre-compiled Thread libraries to simplify the development process. These libraries provide essential functionalities required for implementing Thread-based applications, allowing developers to quickly integrate and deploy Thread networks.

- [Thread Tools](https://developer.nordicsemi.com/nRF_Connect_SDK/doc-legacy/2.6.1/nrf/protocols/thread/tools.html): Tools for development and testing. A set of tools is available to aid in the development, testing, and debugging of Thread networks. These tools include network simulators, packet sniffers, and commissioning tools, which help ensure that the Thread network operates correctly and efficiently.

- [Thread Certification](https://developer.nordicsemi.com/nRF_Connect_SDK/doc-legacy/2.6.1/nrf/protocols/thread/certification.html): Certification process information. Information about the Thread certification process is provided to help developers ensure that their products meet the required standards for interoperability and performance. This includes guidelines on testing and validation procedures. 

- [Thread Device Types](https://developer.nordicsemi.com/nRF_Connect_SDK/doc-legacy/2.6.1/nrf/protocols/thread/device_types.html): Explains various Thread device types, including Sleepy End Devices. This section explains the different types of devices within a Thread network, such as Full Thread Devices (FTDs) and Minimal Thread Devices (MTDs), including Sleepy End Devices. Each device type has specific roles and characteristics that contribute to the overall network functionality.

- [Sleepy End Device types in Thread](https://developer.nordicsemi.com/nRF_Connect_SDK/doc-legacy/2.6.1/nrf/protocols/thread/sed_ssed.html):
   - **Sleepy End Device (SED)**: These are Minimal Thread Devices (MTDs) that spend most of their time in a sleep state to conserve power. They wake up periodically to check for messages from their parent router
   - **Synchronized Sleepy End Device (SSED)**: These devices are further optimized for power consumption by synchronizing their communication with their parent router, using Coordinated Sampled Listening (CSL). This reduces the need for frequent polling and lowers power usage even further
  
- [Commissioning Over Authenticated TLS](https://developer.nordicsemi.com/nRF_Connect_SDK/doc-legacy/2.6.1/nrf/protocols/thread/tcat.html): Secure commissioning methods. Secure commissioning methods are essential for maintaining the integrity and security of a Thread network. This section describes how to use Authenticated TLS (Transport Layer Security) for the secure commissioning of Thread devices, ensuring that only authorized devices can join the network.


## Thread Roles

- **End Device**: Communicates through a parent router, conserving power by sleeping.
- **Thread Router**: Provides routing and security services.
- **Thread Leader**: Dynamically elected to manage network configuration.
- **Border Router**: Connects Thread networks to external networks.

*Note:* ***Thread link*** *refers to the communication pathway between devices in a Thread network. This link enables devices to send and receive messages within the mesh network, supporting seamless communication and ensuring data can be routed efficiently across the network.*






# PHY Test Tool (Physical Layer) 
The physical layer is responsible for the transmission and reception of raw data bits over a physical medium, such as radio waves. It defines the hardware technologies used to connect devices and manage data transmission rates, modulation, and signal strength.


## General information for PHY Test Tool Samples

- [Low Power UART (LPUART)](https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/samples/peripheral/lpuart/README.html): This sample demonstrates the use of the Low Power UART (LPUART) driver. LPUART is designed for applications where power consumption is a concern and allows the UART to operate in a low-power mode.

- [Radio Test](https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/samples/peripheral/radio_test/README.html): This sample provides a framework for testing the radio performance of Nordic Semiconductor devices. It allows you to evaluate and measure the radio's capabilities under various conditions.

- [IEEE 802.15.4 Sniffer Sample](https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/samples/peripheral/802154_sniffer/README.html): This sample provides an implementation of a packet sniffer for IEEE 802.15.4 (any wireless communication protocols)

- [IEEE 802.15.4 PHY Test](https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/samples/peripheral/802154_phy_test/README.html): This sample demonstrates how to test the physical layer (PHY) of IEEE 802.15.4 communication, focusing on the modulation and demodulation processes.


# Zigbee 

## General information of Zigbee samples

These [samples](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.6.1/nrf/samples/zigbee.html) are designed to work with Nordic Semiconductor's development kits, such as the nRF52840 DK or the nRF5340 DK. 

- Zigbee Light Bulb: This sample demonstrates how to create a **Zigbee router node**. It implements a simple light bulb device that can be controlled by a Zigbee coordinator.

- Zigbee Light Switch: This sample shows how to implement a **Zigbee end device** that can control a Zigbee light bulb. This is useful for understanding how to interact with router nodes in the network.

- Zigbee Network Coordinator: This is for the **coordinator role**, it provides a example of how to set up a Zigbee coordinator to manage the network.

- Zigbee Shell: The Zigbee Shell sample is designed to provide an interactive command-line interface (CLI) for configuring and managing a Zigbee network **(Zigbee router or Coordinator)**

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





