
# IoT Garden System

This is a simple IoT project designed to monitor sensors in a garden area. The project consists of three main files within the Arduino IDE:

## Files

1. **Garden1.ino**: This file implements the functionality of sensor node 1 in the IoT system. It's designed to collect data from the sensor and transmit information to the gateway.

2. **Garden2.ino**: This file functions as sensor node 2 in the IoT system. Similar to Garden1, it collects data from the second sensor and sends it to the gateway.

3. **Gateway ESP32.ino**: This file acts as the gateway in the IoT system. It collects data from the sensor nodes and can process or forward the data to another central point and send data from sensor node 1 and sensor node 2 to MQTT server (MQTT broker).

## How to Use

1. **Requirements**: To use this, you'll need the Arduino IDE installed on your computer and ESP32-compatible boards. (Arduino UNO R3 and ESP32 Dev Kit Module)

2. **Usage**:
   - Load the respective `.ino` files into the Arduino IDE.
   - Connect the ESP32 board and select the correct port in the Arduino IDE.
   - Upload the code to each corresponding board: Garden1 for sensor node 1, Garden2 for sensor node 2, and Gateway ESP32 for the gateway.
   - Power up the boards and wait for them to establish communication with each other.

## Notes

- Ensure that the network configuration and connection parameters are correctly set in the source code for the sensor nodes to communicate with the gateway.

- This project serves as a basic example and can be expanded to integrate additional functionalities and sensors into the IoT system.

- If you have a issue in process run the program , about issue a library, you can check it error in terminal and install a related library, such as: library about serial commnuication, MQTT communication, Collect sensor data library.
