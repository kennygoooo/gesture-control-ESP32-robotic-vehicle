#include "network_config.h"

const char *ssid = "ESP32-Car";
const char *password = "12345678";
WiFiUDP udp;
const int udpPort = 4210;
char incomingPacket[255];

void setupWiFi() {
  WiFi.softAP(ssid, password);
  Serial.print("✅ ESP32 AP started. IP: ");
  Serial.println(WiFi.softAPIP());
}

void setupUDP() {
  udp.begin(udpPort);
  Serial.print("✅ UDP server started on port ");
  Serial.println(udpPort);
}