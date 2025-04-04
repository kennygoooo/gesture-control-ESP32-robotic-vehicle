#include "esp_camera.h"
#include <WebServer.h>
#include "camera_server.h"
#include "Freenove_4WD_Car_For_ESP32.h"
#include "network_config.h"


// Sean_Kenny_3rd
//封包camera_server.cpp, camera_server.h, 
//封包network_config.cpp, network_config.h


// ===================
// Select camera model
// ===================
// #define CAMERA_MODEL_WROVER_KIT // Has PSRAM
// #include "camera_pins.h"v

// ---------------------- SERVER ----------------------
WebServer server(80);

// ---------------------- HANDLE COMMANDS ----------------------
void handleCommand(String cmd) {
  if (cmd.startsWith("move")) {
    Motor_Move(2000, 2000, 2000, 2000);
  } else if (cmd.startsWith("left")) {
    Motor_Move(-2000, -2000, 2000, 2000);
  } else if (cmd.startsWith("right")) {
    Motor_Move(2000, 2000, -2000, -2000);
  } else if (cmd == "0") {
    Motor_Move(0, 0, 0, 0);
  }
}

// ---------------------- SETUP ----------------------
void setup() {
  Serial.begin(115200);
  delay(2000); // Let serial stabilize
  Serial.println("Starting setup...");

  // Init motors
  PCA9685_Setup();
  Motor_Move(0, 0, 0, 0);
  Serial.println("Motor setup done.");

  initCamera();
  setupWiFi();   // Start Wi-Fi AP        
  setupUDP();    // Start UDP

  // Start stream
  startCameraServer();
  Serial.println("✅ Camera stream server started at /stream");
}

// ---------------------- MAIN LOOP ----------------------
void loop() {
  // Handle UDP commands
  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(incomingPacket, 255);
    if (len > 0) incomingPacket[len] = '\0';

    String cmd = String(incomingPacket);
    Serial.println("📩 Received: " + cmd);
    handleCommand(cmd);
  }

  server.handleClient();
}
