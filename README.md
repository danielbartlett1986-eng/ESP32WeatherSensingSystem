# ESP32 Weather Server with OLED & LED Temperature Gauge 🌡️📡

This project turns an ESP32 into a lightweight weather data receiver and web server.
It accepts sensor readings over HTTP, displays them locally on an SSD1306 OLED, visualizes temperature using a 7-LED gauge, and serves a live web dashboard with a timestamp showing how fresh the data is.

Think of it as a tiny weather base station that listens, remembers, and reports.

## Features

📶 WiFi-connected ESP32 web server

🌐 Live web dashboard with auto-refresh

⏱️ “Last update” age indicator (seconds since last reading)

🖥️ SSD1306 OLED display for local readout

🌡️ LED temperature gauge (cold → hot)

🔌 Accepts data via HTTP POST (or GET for testing)

🔐 WiFi credentials kept out of source via secrets.h

## Hardware Requirements

ESP32 development board

SSD1306 OLED (128x64, I²C, address 0x3C)

7 LEDs + current-limiting resistors

Jumper wires / breadboard

LED Pin Mapping
Temperature Level	GPIO Pin
Coldest	GPIO 2
	GPIO 4
	GPIO 5
	GPIO 18
	GPIO 19
	GPIO 21
Hottest	GPIO 22

## Software Requirements

Arduino IDE

ESP32 board support installed

## Libraries:

WiFi

WebServer

Adafruit_GFX

Adafruit_SSD1306

secrets.h

Create a file named secrets.h in the project directory:

#define WIFI_SSID "YourWiFiName"
#define WIFI_PASSWORD "YourWiFiPassword"


This keeps credentials out of version control, which GitHub appreciates.

# How It Works
1. ESP32 Boots

Connects to WiFi

Starts a web server on port 80

Displays IP address on the OLED

2. Data Ingestion

The ESP32 listens on:

/update


##Accepted parameters:

temp → Temperature (°F)

hum → Humidity (%)

pres → Pressure (hPa)

batt → Battery voltage (V)

On receipt:

Values are stored

OLED updates

LED gauge updates

Timestamp is refreshed

Example HTTP Request
POST (recommended)
POST /update HTTP/1.1
Content-Type: application/x-www-form-urlencoded

temp=72.5&hum=41.2&pres=1013.8&batt=3.94

GET (for browser testing)
http://ESP32_IP/update?temp=72.5&hum=41.2&pres=1013.8&batt=3.94

Web Dashboard

Navigate to the ESP32’s IP address in a browser:

http://ESP32_IP/


You’ll see:

Temperature

Humidity

Pressure

Battery voltage

Time since last update (seconds)

The page auto-refreshes every 5 seconds, so it always feels alive.
