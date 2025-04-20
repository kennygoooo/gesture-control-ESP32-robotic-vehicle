#include "esp_camera.h"
#include <WebServer.h>
#include "camera_server.h"
#include "Freenove_4WD_Car_For_ESP32.h"
#include "network_config.h"

// ---------------------- SERVER ----------------------
WebServer server(80);

// ---------------------- HANDLE COMMANDS ----------------------
void handleCommand(String cmd) {
  Serial.println("📩 Raw command: " + cmd);

  // If multiple commands are sent together (e.g., "move2, left2")
  int commaIndex = cmd.indexOf(",");
  if (commaIndex != -1) {
    // Split into two parts and handle both
    String part1 = cmd.substring(0, commaIndex);
    part1.trim();

    String part2 = cmd.substring(commaIndex + 1);
    part2.trim();

    handleCommand(part1);
    handleCommand(part2);
    return;
  }

  // Handle single command (like "left2" or "move2")
  if (cmd.startsWith("move1")) {
    Motor_Move(-2000,-2000, -2000, -2000);
  } else if (cmd.startsWith("left1")) {
    Motor_Move(1500, 1500, -1500, -1500);
  } else if (cmd.startsWith("right1")) {
    Motor_Move(-1500, -1500, 1500, 1500);
  } else if (cmd.startsWith("move2")) {
    Motor_Move(-2000, -2000, -2000, -2000);
  } else if (cmd.startsWith("left2")) {
    Motor_Move(0, 0, -2000, -2000);
  } else if (cmd.startsWith("right2")) {
    Motor_Move(-2000, -2000, 0, 0);
  } else if (cmd == "0") {
    Motor_Move(0, 0, 0, 0);
  }
}


// ---------------------- FreeRTOS Task: UDP 控制 ----------------------
void commandTask(void *pvParameters) {
  while (1) {
    int packetSize = udp.parsePacket();
    if (packetSize) {
      int len = udp.read(incomingPacket, 255);
      if (len > 0) incomingPacket[len] = '\0';

      String cmd = String(incomingPacket);
      Serial.println("📩 [UDP] Received: " + cmd);
      handleCommand(cmd);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

// ---------------------- FreeRTOS Task: HTTP 串流 ----------------------
void streamTask(void *pvParameters) {
  while (1) {
    server.handleClient();  // 維持 HTTP stream
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}

// ---------------------- SETUP ----------------------
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("🚀 Starting setup...");

  PCA9685_Setup();
  Motor_Move(0, 0, 0, 0);
  Serial.println("✅ Motor setup done.");

  initCamera();
  setupWiFi();
  setupUDP();
  startCameraServer();
  Serial.println("✅ Camera stream server started at /stream");

  // --------- Create FreeRTOS Tasks ---------
  xTaskCreatePinnedToCore(
    commandTask,              // Task function
    "UDP Command Task",       // Name
    4096,                     // Stack size
    NULL,                     // Parameters
    1,                        // Priority
    NULL,                     // Task handle
    0                         // Core 0 (控制)
  );

  xTaskCreatePinnedToCore(
    streamTask,
    "HTTP Stream Task",
    4096,
    NULL,
    1,
    NULL,
    1                         // Core 1 (串流)
  );
}

// ---------------------- LOOP ----------------------
void loop() {
  // 空 loop，讓 FreeRTOS 任務負責全部邏輯
  vTaskDelay(portMAX_DELAY);
}
