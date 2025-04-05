#include "camera_server.h"
#include "esp_camera.h"


#include <WiFi.h>
#include <WiFiUdp.h>
#include "esp_camera.h"
#include <WebServer.h>
#include "camera_server.h"
#include "Freenove_4WD_Car_For_ESP32.h"

// ---------------------- CAMERA CONFIG ----------------------
// Freenove board camera pin mapping (OV2640)
#define PWDN_GPIO_NUM     -1         // 沒有使用
#define RESET_GPIO_NUM    -1         // 沒有使用
#define XCLK_GPIO_NUM     21         // GPIO21 → XCLK
#define SIOD_GPIO_NUM     26         // GPIO26 → SIOD (I2C SDA)
#define SIOC_GPIO_NUM     27         // GPIO27 → SIOC (I2C SCL)

#define Y9_GPIO_NUM       35         // CSI_Y9
#define Y8_GPIO_NUM       34         // CSI_Y8
#define Y7_GPIO_NUM       39         // CSI_Y7
#define Y6_GPIO_NUM       36         // CSI_Y6
#define Y5_GPIO_NUM       19         // CSI_Y5
#define Y4_GPIO_NUM       18         // CSI_Y4
#define Y3_GPIO_NUM        5         // CSI_Y3
#define Y2_GPIO_NUM        4         // CSI_Y2

#define VSYNC_GPIO_NUM    25         // GPIO25 → VSYNC
#define HREF_GPIO_NUM     23         // GPIO23 → HREF
#define PCLK_GPIO_NUM     22         // GPIO22 → PCLK


// Init camera
void initCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;  
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 16000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
    Serial.printf("❌ Camera init failed: 0x%x\n", err);
    
    } else {
    Serial.println("✅ Camera init success!");
    }
}

// ---------------------- CAMERA STREAM SERVER ----------------------
void handleJPGStream() {
    WiFiClient client = server.client();
  
    String response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
    server.sendContent(response);
  
    while (client.connected()) {
      camera_fb_t *fb = esp_camera_fb_get();
      if (!fb) continue;
  
      client.write("--frame\r\n");
      client.write("Content-Type: image/jpeg\r\n\r\n");
      client.write(fb->buf, fb->len);
      client.write("\r\n");
  
      esp_camera_fb_return(fb);
      delay(50);
    }
  }
  
  void startCameraServer() {
    server.on("/stream", HTTP_GET, handleJPGStream);
    server.begin();
  }