#ifndef NETWORK_CONFIG_H
#define NETWORK_CONFIG_H

#include <WiFi.h>
#include <WiFiUdp.h>

extern const char *ssid;
extern const char *password;
extern WiFiUDP udp;
extern const int udpPort;
extern char incomingPacket[255];

void setupWiFi();
void setupUDP();

#endif
