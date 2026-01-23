#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <secrets.h>
#include "time.h"

// ---------- WiFi ----------
const char* ssid     = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// ---------- Web Server ----------
WebServer server(80);

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------- Latest Readings ----------
float temperature = 0;
float humidity    = 0;
float pressure    = 0;
float battery     = 0;

unsigned long lastUpdateMillis = 0;

const unsigned long EXPECTED_UPDATE_INTERVAL = 5UL * 60UL * 1000UL; // 5 minutes
unsigned long lastOLEDRefresh = 0;
const unsigned long OLED_REFRESH_INTERVAL = 1000; // 1 second

// ---------- LED Gauge ----------
int ledPins[] = {2, 4, 5, 18, 19, 25, 2};
const int ledCount = sizeof(ledPins)/sizeof(ledPins[0]);

// ---------- Display State ----------
enum DisplayState {
  DISPLAY_STARTUP,
  DISPLAY_DATA,
  DISPLAY_PROCESSING
};

DisplayState displayState = DISPLAY_STARTUP;
unsigned long displayStateStart = 0;
unsigned long lastFlashMillis = 0;
int flashCount = 0;
bool flashVisible = true;


// ---------- Function Prototypes ----------
void handleUpdate();
void updateOLED();
void updateLEDGauge(float temp);

void setup() {
  Serial.begin(115200);

  // LED pins
  for (int i=0;i<ledCount;i++){
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // OLED init
  Wire.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    Serial.println("OLED failed");
    while(true);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  // WiFi
  WiFi.mode(WIFI_STA);
  //WiFi.config(local_IP, gateway, subnet, dns);
  WiFi.begin(ssid,password);
  Serial.print("Connecting to WiFi");
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected: " + WiFi.localIP().toString());

  // ---------- Time (NTP) ----------
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  setenv("TZ", "PST8PDT,M3.2.0/2,M11.1.0/2", 1);
  tzset();

  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
   Serial.println("Failed to obtain time");
    return;
  }

  char timeString[30];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
  Serial.println(timeString);

  // Server route
  server.on("/update", HTTP_POST, handleUpdate);
  server.on("/update", HTTP_GET, handleUpdate);  // temporary GET for browser testing

  server.on("/", HTTP_GET, []() {
  unsigned long ageSeconds = 0;
  if (lastUpdateMillis > 0) {
    ageSeconds = (millis() - lastUpdateMillis) / 1000;
  }
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta http-equiv='refresh' content='5'>";  // auto-refresh every 5s
  html += "<title>ESP32 Weather Server</title>";
  html += "<style>";
  html += "body { font-family: Arial; background:#111; color:#eee; padding:20px; }";
  html += "h1 { color:#6cf; }";
  html += ".box { font-size:1.2em; margin:10px 0; }";
  html += ".stamp { color:#aaa; font-size:0.9em; margin-top:15px; }";
  html += "</style></head><body>";

  html += "<h1>ESP32 Weather Server</h1>";
  html += "<div class='box'>🌡️ Temperature: " + String(temperature, 1) + " °F</div>";
  html += "<div class='box'>💧 Humidity: " + String(humidity, 1) + " %</div>";
  html += "<div class='box'>📉 Pressure: " + String(pressure, 1) + " hPa</div>";
  html += "<div class='box'>🔋 Battery: " + String(battery, 2) + " V</div>";
  if (lastUpdateMillis > 0) {
    html += "<div class='stamp'>Last update: ";
    html += String(ageSeconds);
    html += " seconds ago</div>";
  } else {
    html += "<div class='stamp'>No data received yet</div>";
  }
  html += "</body></html>";

  server.send(200, "text/html", html);
});

  server.begin();

  // ---------- Startup Screen ----------
display.clearDisplay();

// Title
display.setTextSize(2);
display.setCursor(0,0);
display.println("WEATHER");

// Subtitle
display.setTextSize(1);
display.setCursor(0,18);
display.println("Telemetry Node");

// Divider
display.drawLine(0,28,127,28,SSD1306_WHITE);

// Status
display.setCursor(0,32);
display.println("Status: ONLINE");

// IP Address
display.setCursor(0,44);
display.print("IP ");
display.println(WiFi.localIP());

display.display();

displayStateStart = millis();
displayState = DISPLAY_STARTUP;

}



void handleUpdate() {
  // Read values from form-urlencoded
  if (server.hasArg("temp")) temperature = server.arg("temp").toFloat();
  if (server.hasArg("hum"))  humidity    = server.arg("hum").toFloat();
  if (server.hasArg("pres")) pressure    = server.arg("pres").toFloat();
  if (server.hasArg("batt")) battery     = server.arg("batt").toFloat();

  lastUpdateMillis = millis();

  Serial.print("Raw args: ");
  for(int i=0;i<server.args();i++){
  Serial.print(server.argName(i)); Serial.print("="); Serial.print(server.arg(i)); Serial.print(" ");
  }
  Serial.println();

  Serial.println("Received data:");
  Serial.print("Temp: "); Serial.println(temperature);
  Serial.print("Hum:  "); Serial.println(humidity);
  Serial.print("Pres: "); Serial.println(pressure);
  Serial.print("Batt: "); Serial.println(battery);

  displayState = DISPLAY_PROCESSING;
  displayStateStart = millis();
  flashCount = 0;
  flashVisible = true;

  updateLEDGauge(temperature);

  server.send(200,"text/plain","OK");
}

void drawProcessingScreen(uint8_t progress);

void drawProcessingScreen(uint8_t progress) {
  display.clearDisplay();

  // Title
  display.setTextSize(1);
  display.setCursor(20, 8);
  display.println("Weather Data");

  display.setCursor(28, 18);
  display.println("Incoming");

  // Progress bar outline
  int barX = 14;
  int barY = 36;
  int barW = 100;
  int barH = 10;

  display.drawRect(barX, barY, barW, barH, SSD1306_WHITE);

  // Filled portion
  int fillWidth = (progress * (barW - 2)) / 5; // 0–5 seconds
  display.fillRect(barX + 1, barY + 1, fillWidth, barH - 2, SSD1306_WHITE);

  // Status ticks
  display.setCursor(40, 50);
  display.printf("%d / 5", progress);

  display.display();
}

void updateDisplayState() {
  unsigned long now = millis();

  if (displayState == DISPLAY_STARTUP) {
    // Show startup screen for 3 seconds
    if (now - displayStateStart >= 3000) {
      displayState = DISPLAY_DATA;
      lastOLEDRefresh = 0; // force redraw
    }
  }

  else if (displayState == DISPLAY_PROCESSING) {
    unsigned long elapsed = now - displayStateStart;
    uint8_t progress = elapsed / 1000;
    if (progress > 5) progress = 5;

    drawProcessingScreen(progress);

    if (elapsed >= 5000) {
      displayState = DISPLAY_DATA;
      lastOLEDRefresh = 0;
    }
  }

  else if (displayState == DISPLAY_DATA) {
    if (now - lastOLEDRefresh >= OLED_REFRESH_INTERVAL) {
      updateOLED();
      lastOLEDRefresh = now;
    }
  }
}

void updateOLED() {
  display.clearDisplay();

  // ---------- TOP: Temperature ----------
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("TEMP ");
  display.print((int)temperature);
  display.print("F");

  // ---------- Divider ----------
  display.drawLine(0,18,127,18,SSD1306_WHITE);

  // ---------- MIDDLE: Stats ----------
  display.setTextSize(1);

  display.setCursor(0,22);
  display.print("Hum ");
  display.print((int)humidity);
  display.print("%");

  display.setCursor(64,22);
  display.print("Pres ");
  display.print((int)pressure);

  display.setCursor(0,34);
  display.print("Batt ");
  display.print(battery,2);
  display.print("V");

    // ---------- NEXT UPDATE / OVERDUE ----------
  display.setCursor(0, 42);

  if (lastUpdateMillis > 0) {
    unsigned long ageMs = millis() - lastUpdateMillis;
    long remainingMs = EXPECTED_UPDATE_INTERVAL - ageMs;

    if (remainingMs <= 0) {
      // Blink overdue warning every 500 ms
      if ((millis() / 500) % 2 == 0) {
        display.print("UPDATE OVERDUE");
      }
    } else {
      // MM:SS countdown
      int remainingSec = remainingMs / 1000;
      int minutes = remainingSec / 60;
      int seconds = remainingSec % 60;

      display.print("Next update ");
      display.printf("%02d:%02d", minutes, seconds);
    }
  } else {
    display.print("Waiting for data...");
  }


  // ---------- BOTTOM: Date + Time ----------
  struct tm timeinfo;
  display.setCursor(0,52);

  if (getLocalTime(&timeinfo)) {
    const char* months[] = {
      "Jan","Feb","Mar","Apr","May","Jun",
      "Jul","Aug","Sep","Oct","Nov","Dec"
    };

  display.printf(
    "%02d %s %04d  %02d:%02d",
    timeinfo.tm_mday,
    months[timeinfo.tm_mon],
    timeinfo.tm_year + 1900,
    timeinfo.tm_hour,
    timeinfo.tm_min
  );
} else {
  display.print("Time not set");
}


  display.display();
}


void updateLEDGauge(float temp){
  int level = 0;
  if (temp<40) level=1;
  else if (temp<50) level=2;
  else if (temp<60) level=3;
  else if (temp<70) level=4;
  else if (temp<80) level=5;
  else if (temp<90) level=6;
  else level=7;

  for(int i=0;i<ledCount;i++){
    digitalWrite(ledPins[i], i<level ? HIGH : LOW);
  }
}
void loop() {
  server.handleClient();
  updateDisplayState();
}