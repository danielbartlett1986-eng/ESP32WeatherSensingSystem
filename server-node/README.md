# 🌤️ ESP32 Weather Telemetry Server

This project turns an **ESP32** into a simple web-based **weather telemetry display** using an **OLED screen**, an **LED temperature gauge**, and a lightweight **HTTP server** for sensor updates. It’s designed to receive environmental readings over WiFi and show them locally on an OLED and remotely via a browser interface.

---

## 📋 Features

- 🌡️ Displays **temperature, humidity, pressure**, and **battery voltage**  
- 🧠 Automatically syncs clock using **NTP time servers**  
- 💡 LED gauge visually represents current temperature range  
- 🕸️ Hosts a **web dashboard** showing the latest sensor values  
- 🔄 Supports both **HTTP GET** and **POST** for telemetry updates  
- 🖥️ Automatic OLED updates upon data receive  
- ⚙️ Startup screen with WiFi connection info  

---

## 🧰 Hardware Requirements

- **ESP32 development board**  
- **SSD1306 OLED display (128x64, I2C, 0x3C address)**  
- **LEDs (x7)** with current-limiting resistors  
- **WiFi access point**  
- Any external telemetry node capable of sending POST data  

---

## 🧩 Wiring

| Component | ESP32 Pin | Notes |
|------------|------------|-------|
| OLED SDA   | GPIO 21    | I2C data line |
| OLED SCL   | GPIO 22    | I2C clock line |
| OLED VCC   | 3.3V       | Power |
| OLED GND   | GND        | Ground |
| LED1-LED7  | GPIO 2, 4, 5, 18, 19, 25, 2 | Set as temperature gauge outputs |

> Adjust pin assignments in the `ledPins[]` array if needed.

---

## 💻 Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software) or [PlatformIO](https://platformio.org/)
- ESP32 board support package installed
- Libraries:
  - **WiFi.h**
  - **WebServer.h**
  - **Adafruit_GFX**
  - **Adafruit_SSD1306**

You can install libraries via the Arduino Library Manager.

---

## 🔐 secrets.h

### Configuration

This project uses a `secrets.h` file for local configuration.

1. Copy `secrets.example.h` to `secrets.h`
2. Edit the following values:
   - WiFi SSID
   - WiFi password
   - Timezone (POSIX format)

Example timezones:
- Pacific: `PST8PDT`
- Mountain: `MST7MDT`
- Central: `CST6CDT`
- Eastern: `EST5EDT`
- UTC: `UTC0`

🚀 Usage
Connect and flash your ESP32 with this sketch.

Open the Serial Monitor (115200 baud) to view connection and debug info.

Once WiFi connects, note the IP address displayed.

Visit the ESP32’s web page in your browser — it updates every 5 seconds showing the current telemetry data.

Send updates from any client using HTTP (e.g., another microcontroller or a script):

Example (via cURL)
```bash
curl -X POST "http://<ESP32-IP>/update" \
     -d "temp=72.5&hum=48.2&pres=1015.6&batt=3.93"
```

🧮 Display Overview
OLED Screen Layout

Temperature prominently at the top

Humidity, pressure, and battery below

Date and time (via NTP) at the bottom

LED Gauge Mapping

Temperature Range (°F)	LEDs Lit
< 40	1
40–49	2
50–59	3
60–69	4
70–79	5
80–89	6
≥ 90	7
🧠 Code Behavior Summary
On boot:

Initializes serial, OLED, LED pins, and WiFi

Starts web server and NTP sync

Shows status + IP on display

On data update:

Parses temperature, humidity, pressure, and battery

Updates OLED and LED gauge accordingly

Returns "OK" to sender

🧾 License
This project is open-source under the MIT License.
Use, modify, and share freely — just keep attribution.

🛠️ Future Improvements
Add onboard sensor readings (e.g., BME280)

Implement JSON API with authentication

Add WiFi config portal for easier setup

Store historical data with timestamps