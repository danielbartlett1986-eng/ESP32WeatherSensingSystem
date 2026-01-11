#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include "secrets.h"

// ---------- WiFi ----------
const char* ssid     = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// ---------- Server ----------
// Example: Set your server IP in include/secrets.h
// static const char* serverIP = "192.168.10.7";
// static const int serverPort = 80;

// ---------- Pins ----------
#define SDA_PIN 26
#define SCL_PIN 27
#define BATTERY_PIN 33

// ---------- Battery Divider ----------
#define ADC_MAX 4095.0
#define ADC_REF 3.3
#define R1 27000.0
#define R2 100000.0

// ---------- Deep Sleep ----------
#define SLEEP_SECONDS 300   // 5 minutes

Adafruit_BME280 bme;
int raw = 0;

// ---------- Helpers ----------
float readBattery() {
  analogSetPinAttenuation(BATTERY_PIN, ADC_11db);
  analogSetWidth(12);

  long sum = 0;
  for (int i = 0; i < 8; i++) {
    sum += analogRead(BATTERY_PIN);
    delay(2);
  }

  float raw = sum / 8.0;
  float vadc = raw * 0.0008237;
  return vadc * 1.27;
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Wire.begin(SDA_PIN, SCL_PIN);
  if (!bme.begin(0x76)) {
    Serial.println("BME280 not found");
    ESP.deepSleep(10e6);
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nWiFi failed");
    ESP.deepSleep(30e6);
  }

  Serial.println("\nWiFi connected");

  float temp = bme.readTemperature();
  float tempF = (temp * 9.0 / 5.0) + 32.0;
  float hum  = bme.readHumidity();
  float pres = bme.readPressure() / 100.0F;
  float batt = readBattery();
  
  Serial.println("----- Sensor Readings -----");
  Serial.print("Temperature: ");
  Serial.print(tempF);
  Serial.println(" °F");

  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println(" %");

  Serial.print("Pressure: ");
  Serial.print(pres);
  Serial.println(" hPa");

  Serial.print("Battery: ");
  Serial.print(batt, 2);
  Serial.println(" V");
  Serial.println("---------------------------");

  HTTPClient http;
  String url = String("http://") + serverIP + "/update"; //Uses values from secrets.h if you have your own.
  String payload =
    "temp=" + String(tempF, 2) +
    "&hum=" + String(hum, 1) +
    "&pres=" + String(pres, 1) +
    "&batt=" + String(batt, 2);

  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int code = http.POST(payload);
  Serial.print("POST result: ");
  Serial.println(code);

  http.end();

  Serial.println("Sleeping...");
  ESP.deepSleep(SLEEP_SECONDS * 1000000ULL);
}

void loop() {
  // never used
}
