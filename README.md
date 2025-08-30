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

## License

This project is licensed under the MIT License.

---

## Support

For help with setting up sensors, the qRaptor platform integration, or code modifications, please open an issue or contact the project maintainer.

---

## Example Arduino Sketch

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Replace with your WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Replace with your qRaptor webhook URL (from "Configure & Test API")
const char* qraptorWebhookUrl = "https://your-qraptor-webhook-url";

// Sensor pins and libraries (example placeholders)
#define DHTPIN 4 // DHT22 sensor pin
#define DHTTYPE DHT22
#include <DHT.h>
DHT dht(DHTPIN, DHTTYPE);

// Example: Analog pins for sensors (adjust according to your wiring)
#define MQ135PIN 34 // Air Quality sensor analog pin
#define SOIL_MOISTURE_PIN 35

// Helper to read analog sensor with smoothing
int readSensor(int pin) {
long sum = 0;
const int samples = 10;
for (int i = 0; i < samples; i++) {
sum += analogRead(pin);
delay(10);
}
return sum / samples;
}

void setup() {
Serial.begin(115200);
delay(1000);
dht.begin();

// Connect to WiFi
WiFi.begin(ssid, password);
Serial.print("Connecting to WiFi");
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("Connected!");
}

void loop() {
if (WiFi.status() == WL_CONNECTED) {
HTTPClient http;

text
// Construct JSON payload
StaticJsonDocument<512> jsonDoc;
jsonDoc["device_id"] = "env_monitor_001";

// Read temperature and humidity from DHT22
float temperature = dht.readTemperature();
float humidity = dht.readHumidity();

// Read analog sensors
int airQualityRaw = readSensor(MQ135PIN);
int soilMoistureRaw = readSensor(SOIL_MOISTURE_PIN);

// Map analog readings to approximate values (calibrate as needed)
int airQualityIndex = map(airQualityRaw, 0, 4095, 0, 500);
int soilMoisturePercent = map(soilMoistureRaw, 4095, 0, 0, 100); 

jsonDoc["temperature"] = isnan(temperature) ? -999 : temperature;
jsonDoc["humidity"] = isnan(humidity) ? -999 : humidity;
jsonDoc["air_quality_index"] = airQualityIndex;
jsonDoc["soil_moisture"] = soilMoisturePercent;

// Add timestamp in ISO 8601 format
jsonDoc["timestamp"] = String("2025-08-30T") + String(hour()) + ":" + String(minute()) + ":" + String(second()) + "Z";

// Serialize JSON to String
String payload;
serializeJson(jsonDoc, payload);

http.begin(qraptorWebhookUrl);
http.addHeader("Content-Type", "application/json");

int httpResponseCode = http.POST(payload);

if (httpResponseCode > 0) {
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  String response = http.getString();
  Serial.println("Response: " + response);
} else {
  Serial.print("Error on sending POST: ");
  Serial.println(httpResponseCode);
}

http.end();
} else {
Serial.println("WiFi Disconnected");
}
// Wait before sending next reading (e.g., 30 seconds)
delay(30000);
}

text

---

Thank you for using this Environmental Monitor ESP32 project!  
Feel free to contribute, suggest improvements, or report issues.
