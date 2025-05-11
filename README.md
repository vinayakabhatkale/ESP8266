
The project aims to collect environmental data from the sensor and sending it to amazon S3 bucket for storage and monitoring purposes.

Please download the entire project pdf file using [Download PDF](./Scientific_project_report.pdf)

**How does the code work?**

ESP8266 IoT Sensor Data Logger with AWS MQTT Integration
This project demonstrates how to use the ESP8266 microcontroller to collect environmental sensor data using the Adafruit BME680 sensor and publish it to an AWS MQTT broker. The code is designed for real-time temperature, humidity, pressure, and gas readings, which are securely transmitted using SSL/TLS encryption.

Features
Wi-Fi Connectivity: Connect to a Wi-Fi network to enable IoT functionality.
AWS IoT Integration: Publish sensor data to an AWS IoT Core MQTT broker.
Secure Communication: Uses SSL/TLS for secure data transmission.
NTP Client: Synchronizes device time using an NTP server.
Adafruit BME680 Sensor: Gathers environmental metrics:
Temperature
Humidity
Pressure
Gas Resistance
Approximate Altitude
 
Hardware Requirements

**Requirements**

```
ESP8266 Development Board
Adafruit BME680 Sensor
SPIFFS storage on ESP8266 for certificates
Power source and required wiring components
Software Requirements
Arduino IDE with the following libraries installed:
ESP8266WiFi
PubSubClient
NTPClient
Adafruit_Sensor
Adafruit_BME680
FS (File System for SPIFFS)
AWS IoT Core setup with:
Device Certificates
Private Key
CA Certificate
Setup Instructions
Hardware Wiring
```

Connect the BME680 sensor to the ESP8266 using I2C (SCL to D1, SDA to D2).
Power the sensor with 3.3V or 5V as per your hardware configuration.
Prepare the Code

Replace placeholders in the code with your:
Wi-Fi SSID and password.
AWS IoT endpoint.
Upload your AWS IoT certificates (Amazon.cert.der, private.der, and ca.der) to the ESP8266 SPIFFS filesystem.
Upload Code

Configure the Arduino IDE for your ESP8266 board.
Upload the sketch to your ESP8266.
Monitor Data

Open the serial monitor to observe real-time logs and debug messages.
Verify the connection to AWS IoT Core and observe the published sensor data.
Configuration Details
Wi-Fi Credentials
cpp

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
AWS IoT Configuration
Replace the AWS_endpoint variable with your AWS IoT endpoint.
cpp

const char* AWS_endpoint = "XXXXXXXXXXXXX-ats.iot.eu-central-1.amazonaws.com";
Certificates
Store certificates in the ESP8266 SPIFFS filesystem with these filenames:
Amazon.cert.der
private.der
ca.der
Functionality Overview
Initialization
Establish Wi-Fi connection.
Sync device time with an NTP server.
Load SSL/TLS certificates from SPIFFS.
Data Collection
Reads sensor data:
Temperature
Humidity
Pressure
Gas Resistance
Approximate Altitude
Data Transmission
Publishes collected data to the outTopic MQTT topic in JSON format:

```json

{
  "temperature": 25.3,
  "humidity": 50.7,
  "altitude": 123.4
}
```

**MQTT Callback**
Receives messages on the subscribed inTopic MQTT topic.
**Troubleshooting**
Wi-Fi Connection Issues: Ensure SSID and password are correct.
AWS IoT Connectivity Issues: Verify certificates and MQTT endpoint.
Sensor Initialization: Confirm the BME680 is correctly wired and detected.
SPIFFS Errors: Ensure certificates are correctly uploaded.

**License**
This project is open-source under the MIT License.

