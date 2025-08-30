#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Replace with your WiFi credentials
const char* ssid = "Tute";
const char* password = "dknx1909";

const char* qraptorWebhookUrl = "https://api.example.com/data";


#define DHTPIN 4            
#define DHTTYPE DHT22
#include <DHT.h>
DHT dht(DHTPIN, DHTTYPE);

#define MQ135PIN 34         
#define SOIL_MOISTURE_PIN 35


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
    

    StaticJsonDocument<512> jsonDoc;
    jsonDoc["device_id"] = "env_monitor_001";
    

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    

    int airQualityRaw = readSensor(MQ135PIN);
    int soilMoistureRaw = readSensor(SOIL_MOISTURE_PIN);
    
 
    int airQualityIndex = map(airQualityRaw, 0, 4095, 0, 500);
    int soilMoisturePercent = map(soilMoistureRaw, 4095, 0, 0, 100); 

    jsonDoc["temperature"] = isnan(temperature) ? -999 : temperature;
    jsonDoc["humidity"] = isnan(humidity) ? -999 : humidity;
    jsonDoc["air_quality_index"] = airQualityIndex;
    jsonDoc["soil_moisture"] = soilMoisturePercent;
    

    jsonDoc["timestamp"] = String("2025-08-30T") + String(hour()) + ":" + String(minute()) + ":" + String(second()) + "Z";
    

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


  delay(30000);
}
