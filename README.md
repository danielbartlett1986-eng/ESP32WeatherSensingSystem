# ESP32 BME280 HTTP Sensor Node
# This project implements a low‑power ESP32 sensor node that:

Connects to Wi‑Fi

Reads temperature, humidity, pressure from a BME280 sensor

Measures battery voltage using an analog input and resistor divider

Sends all readings to an HTTP endpoint on your network

Enters deep sleep between measurements to save power

The sketch targets the Arduino‑ESP32 core and uses the Adafruit BME280 library for sensor access.
​

## Hardware overview
MCU: ESP32 development board (Arduino‑compatible)

Sensor: BME280 (I²C, address 0x76)

Solar panel: Generic 5w solar panel

Solar Charger: CN3065

Boost/Buck Converter - required because the CN3065 outputs 3.3v and our board requires 5v to operate. Converter is adjustable so measure and turn the potentiometer up until your volt meter reads 5v. 

Wirerouting as follows: 

Solar panel --> CN3065

CN3065 Batt out --> 18650 Battery

CN3065 Sys out --> Boost Buck converter --> ESP32

## Battery measurement:

Analog input on GPIO 33 (BATTERY_PIN)

Resistor divider from battery to ADC input (e.g. 27 kΩ / 100 kΩ)

### I²C wiring:

SDA_PIN → GPIO 26

SCL_PIN → GPIO 27

Update the pin assignments in the sketch if your wiring is different.
​

## What the code does
### At each wake‑up cycle the ESP32:

Initializes serial, I²C, and the BME280 sensor.

Connects to the configured Wi‑Fi network using the provided SSID and password.
​
Reads:

Temperature (°C), converts to Fahrenheit

Relative humidity (%)

Pressure (hPa)

Battery voltage (V) using the ADC and divider scaling

Logs readings to the serial monitor for debugging.

Builds a URL‑encoded payload (e.g. temp=72.34&hum=40.1&pres=1008.5&batt=3.98).

Sends the payload via HTTP POST to http://<serverIP>/update on port 80 using HTTPClient.
​

Prints the HTTP status code to serial.

Enters deep sleep for SLEEP_SECONDS seconds (default 300 s / 5 min).
​

On the next timer wake‑up, the chip restarts at setup() and repeats the process.

## Configuration
Edit these sections near the top of the sketch before building:

```arduino 
// ---------- WiFi ----------
const char* ssid     = "YOUR_NETWORK";
const char* password = "PASSWORD";

// ---------- Server ----------
const char* serverIP = "XXX.XXX.XX.XX";
const int   serverPort = 80;
Set ssid / password to your Wi‑Fi network credentials.
```

Set serverIP to the host that will receive the HTTP POSTs (for example, a local HTTP server, ESP32 web server, or home automation bridge).
​

serverPort is currently fixed at 80; update if your server listens on a different port.

### Deep sleep interval:

```arduino
#define SLEEP_SECONDS 300   // 5 minutes
Adjust SLEEP_SECONDS to change how often the node wakes up and reports data.
```
### Battery measurement scaling:

```arduino
#define R1 27000.0
#define R2 100000.0
These correspond to the resistor values in your battery voltage divider (top and bottom).
The readBattery() helper uses the ADC reading and a fixed factor to reconstruct approximate battery voltage based on this divider.
```

