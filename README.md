🌤️ ESP32 Weather Sensing Node
A compact, low-power Wi-Fi-connected weather monitoring system using two ESP32 boards — one as a sensor node and the other as a display server. Together, they collect and display real-time environmental data without relying on external cloud services.

🧩 System Overview
text
[SENSOR NODE] --> HTTP POST --> [SERVER NODE]
  ESP32 + BME280                  ESP32 + OLED + LEDs + Web Dashboard
Sensor Node:
Reads temperature, humidity, pressure, and battery voltage, then transmits the data to the server via Wi-Fi before entering deep sleep to conserve power.

Server Node:
Receives data, updates an OLED display, manages a multi-LED temperature gauge, and serves a live web dashboard showing the latest readings.

💡 Features
📡 Local Wi-Fi communication (no internet needed)

🌡️ Real-time environmental readings

🔋 Battery voltage monitoring

💤 Low-power deep sleep on sensor node

🖥️ OLED and LED visual output

🌐 Web dashboard auto-refreshing every few seconds

⚙️ Hardware Components
(x2) ESP32 Development Boards

BME280 sensor (I²C)

SSD1306 OLED display (128×64)

7 LEDs with resistors (temperature gauge)

Voltage divider for battery monitoring

🔄 Data Flow
Step	Action
1️⃣	Sensor node wakes from sleep and connects to Wi-Fi.
2️⃣	Reads temperature, humidity, pressure, and battery voltage.
3️⃣	Sends data to server’s /update endpoint.
4️⃣	Server updates OLED, LEDs, and web dashboard.
5️⃣	Sensor returns to deep sleep until the next cycle.
🖥️ Web Dashboard Example
Access the server’s IP in your browser (e.g. http://192.168.1.42) to see live sensor data:

🌡️ Temperature: xx.x °F

💧 Humidity: xx.x %

📉 Pressure: xxxx.x hPa

🔋 Battery: x.xx V

⏱️ Last Update: seconds since last post

🧰 Configuration
Edit your secrets.h file for both ESP32 devices:

cpp
#define WIFI_SSID "YourNetworkName"
#define WIFI_PASSWORD "YourPassword"
static const char* serverIP = "192.168.x.x";
static const int serverPort = 80;
Adjust sleep timing, LED thresholds, and other parameters as needed inside the code.

🚀 Applications
Local weather or greenhouse monitoring

Smart home environment sensor

Classroom or IoT learning project

Off-grid or solar-powered data logging

🧠 Summary
Two ESP32 boards — one sensing, one displaying — work together to create an elegant, efficient, and cloud-free weather sensing node that’s simple to build, energy-efficient, and visually engaging.

