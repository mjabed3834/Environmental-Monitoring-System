## Environmental Monitoring System with ESP8266

### Overview

This project involves building an environmental monitoring system using an ESP8266 microcontroller. The system reads data from various sensors to monitor gas levels, temperature, humidity, and dust concentration. The collected data is displayed on an LCD and sent to the ThingSpeak platform for remote monitoring and analysis.

### Components Used

- **ESP8266**: Microcontroller with Wi-Fi capabilities.
- **DHT22 Sensor**: Measures temperature and humidity.
- **MQ-7 Sensor**: Detects carbon monoxide gas levels.
- **Sharp GP2Y10**: Optical dust sensor to measure air quality.
- **LiquidCrystal_I2C**: 16x2 LCD for displaying sensor data.
- **LEDs**: Indicate different status or warning levels.

### Pin Definitions

- **LED_PIN**: 21
- **LED_PIN2**: 22
- **DHT22_PIN**: 34
- **MQ7_ANALOG_PIN**: 35
- **GP2Y10_SIGNAL_PIN**: 27

### Setup Instructions

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/yourusername/environmental-monitoring-system.git
   cd environmental-monitoring-system
   ```

2. **Install Dependencies**:
   - Ensure you have the Arduino IDE installed.
   - Install necessary libraries: `DHT`, `Wire`, `LiquidCrystal_I2C`, `ESP8266WiFi`.

3. **Hardware Connections**:
   - Connect the DHT22 sensor to the ESP8266 on pin 34.
   - Connect the MQ-7 sensor to the analog pin 35.
   - Connect the Sharp GP2Y10 dust sensor to the analog pin 27.
   - Connect the 16x2 LCD with I2C interface.
   - Connect LEDs to pins 21 and 22.

4. **Wi-Fi and ThingSpeak Configuration**:
   - Update the Wi-Fi SSID and password in the code.
   - Update the ThingSpeak API key and channel number in the code.

5. **Upload the Code**:
   - Open the Arduino IDE.
   - Select the correct board and port for your ESP8266.
   - Upload the provided code to your ESP8266.

### Code Explanation

The provided code performs the following tasks:

1. **Setup**:
   - Initializes the serial communication, sensors, and LCD.
   - Connects to the Wi-Fi network.

2. **Loop**:
   - Reads data from the MQ-7 gas sensor.
   - Reads temperature and humidity from the DHT22 sensor.
   - Reads data from the Sharp GP2Y10 dust sensor.
   - Displays the sensor readings and warnings on the LCD.
   - Sends the collected data to ThingSpeak for remote monitoring.

3. **Wi-Fi Connection**:
   - Connects to the specified Wi-Fi network and prints the status.

4. **ThingSpeak Communication**:
   - Sends sensor data to the ThingSpeak channel via HTTP GET requests.

5. **Warning Levels**:
   - Determines warning levels for gas, temperature, humidity, and dust concentration.

6. **Display Functions**:
   - Updates the LCD with the sensor readings and their corresponding warning levels.

### Conclusion

This project demonstrates the use of an ESP8266 microcontroller for environmental monitoring. By collecting and displaying data from various sensors and sending it to ThingSpeak, this system provides a comprehensive solution for real-time environmental monitoring. This setup can be further extended or customized to include additional sensors or functionality as needed.
