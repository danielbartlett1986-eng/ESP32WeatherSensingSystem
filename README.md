# ESP32 Weather Sensing System

This repository contains the code for a weather sensing system using ESP32 microcontrollers. The system is divided into two main components:

        Weather Sensing Node – Collects environmental data like temperature, humidity, pressure, and battery voltage.

        Weather Sensing Server – Hosts a web server to display the latest readings and provides a web interface for monitoring.

##Table of Contents

Features

Hardware Requirements

Software Requirements

Wiring / Pinout

Installation

Configuration

Usage

License

##Features

Reads temperature, humidity, and pressure using BME280 sensors.

Monitors battery voltage.

Low-power operation (deep sleep support optional).

ESP32 node sends data via ESP-NOW or Wi-Fi to server.

Web server interface to display real-time readings.

Optionally logs readings to Google Sheets or other external databases.

##Hardware Requirements

1× ESP32 Dev Board (Node)

1× ESP32 Dev Board (Server)

1× BME280 environmental sensor per node

1x CN3065 Solar Charging Board

1x 5w Solar Panel

1x 18650 battery w/ holder

1x Boost/Buck Converter

Wires, breadboard, or PCB for connections

Optional: LEDs for status indication

Power supply (LiPo battery or USB)

##Software Requirements

Arduino IDE (v1.8.19 or later) or PlatformIO

ESP32 board support package installed

Libraries:

Wire.h

Adafruit_GFX.h

Adafruit_SSD1306.h (if using OLED display)

Adafruit_BME280.h

WiFi.h

WebServer.h

ESP32_NOW.h

HTTPClient.h (if logging to external service)

ESP_Google_Sheet_Client.h (optional, if using Google Sheets)

Wiring / Pinout
BME280 Sensor
Node Pin	BME280 Pin
26 (SDA)	SDA
27 (SCL)	SCL
3.3V	VCC
GND	GND
Battery Monitoring
Node Pin	Function
33	Battery voltage
LEDs (optional)
Pin	Color / Status
2	Node active
Other pins	Temperature gauge (optional)
Installation

Clone this repository:

git clone https://github.com/yourusername/esp32-weather-system.git
cd esp32-weather-system


Open WeatherNode.ino in Arduino IDE for the sensing node or WeatherServer.ino for the server.

Install all required libraries via the Arduino Library Manager.

Configuration

Open secrets.h (or edit WeatherNode.ino) and set your Wi-Fi credentials:

#define WIFI_SSID "your_SSID"
#define WIFI_PASSWORD "your_PASSWORD"


Set any server or ESP-NOW MAC addresses if needed:

#define SERVER_MAC "xx:xx:xx:xx:xx:xx"


Optional: Configure deep sleep duration, web server port, or Google Sheets logging.

Usage

Weather Node: Upload the sketch, power the device, and it will begin sending sensor data.

Weather Server: Upload the sketch, connect to Wi-Fi, and open the ESP32 IP address in a browser to see the dashboard.

The web page automatically shows the latest readings with a timestamp of the last sensor update.

License

This project is licensed under the MIT License. See LICENSE
 for details.