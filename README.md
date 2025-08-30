# A2Hackathon

# Eco Watch

This project enables an ESP32 microcontroller to collect environmental data from various sensors and send it to the qRaptor.ai platform via HTTP POST for real-time analysis and monitoring.

---

## Features

- Reads temperature and humidity using the DHT22 sensor.
- Measures air quality using the MQ-135 sensor.
- Monitors soil moisture levels with an analog sensor.
- Sends JSON-formatted sensor data every 30 seconds to a configurable qRaptor webhook URL.
- Uses WiFi connectivity for cloud communication.
- Includes error handling for sensor reads and connection status.

---

## Hardware Requirements

- ESP32 Development Board
- DHT22 Temperature & Humidity Sensor
- MQ-135 Air Quality Sensor (analog output)
- Soil Moisture Sensor (analog output)
- USB cable for programming and power
- Optional: Power supply with battery or solar for standalone deployment

---

## Software Dependencies

- Arduino IDE or PlatformIO for ESP32 firmware development.
- ArduinoJson library by Benoit Blanchon for JSON serialization.
- DHT sensor library for Arduino.
- WiFi and HTTPClient libraries (built into ESP32 Arduino core).

---

## Setup Instructions

1. **Install Required Libraries**  
   Open Arduino IDE and install the following libraries via Library Manager:  
   - ArduinoJson  
   - DHT sensor library

2. **Configure WiFi and qRaptor Endpoint**  
   In the source code, update the following variables:  
const char* ssid = "YOUR_WIFI_SSID"; // Your WiFi network SSID
const char* password = "YOUR_WIFI_PASSWORD";// Your WiFi password
const char* qraptorWebhookUrl = "https://your-qraptor-webhook-url"; // POST API webhook URL from qRaptor

text

3. **Connect Sensors to ESP32**  
- DHT22 data pin → GPIO 4 (or change in code)  
- MQ-135 analog output → GPIO 34 (or change in code)  
- Soil Moisture sensor output → GPIO 35 (or change as needed)  

4. **Upload the Firmware**  
Compile and upload the code to your ESP32 board using the Arduino IDE.

5. **Monitor Serial Output**  
Open Serial Monitor at 115200 baud to see connection status, sensor readings, and HTTP responses.

---

## Code Overview

- Initializes WiFi and sensors on startup.
- Reads temperature, humidity, air quality, and soil moisture values.
- Maps raw sensor values to human-readable scales.
- Packages sensor data into a JSON object.
- Sends data via HTTP POST to your configured qRaptor webhook URL every 30 seconds.
- Handles WiFi disconnects and HTTP errors gracefully.

---

## JSON Payload Format

{
"device_id": "env_monitor_001",
"temperature": 25.5,
"humidity": 65.2,
"air_quality_index": 85,
"soil_moisture": 45,
"timestamp": "2025-08-30T10:12:00Z"
}

text

---

