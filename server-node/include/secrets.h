// secrets.h (DO NOT COMMIT THIS FILE)

#pragma once

// ---------- WiFi ----------
#define WIFI_SSID      "DasHausIot"
#define WIFI_PASSWORD  "Tundrav8!"

// ---------- Server ----------
static const char* serverIP = "192.168.10.7";   // INSIDE ESP32
static const int serverPort = 80;

// ---------- Time Zone ----------
// POSIX TZ format
// Examples:
//   Pacific:  "PST8PDT"
//   Mountain: "MST7MDT"
//   Central:  "CST6CDT"
//   Eastern:  "EST5EDT"
//   UTC:      "UTC0"

#define TZ_INFO "PST8PDT"