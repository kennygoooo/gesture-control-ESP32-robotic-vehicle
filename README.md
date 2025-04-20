# AirControl Car - Wireless control using hand and foot movements

This project enables real-time control and video streaming of the Freenove 4WD Car using an ESP32 with ESP32-WROVER CAM. A unique feature of this project is the use of an **Android phone’s front camera** to track the user's **hand and foot movements** via computer vision. These gestures are translated into **UDP control commands**, allowing for intuitive, touchless interaction with the robotic vehicle.

The system is built with [FreeRTOS](), and allows:

- Remote control movement over UDP
- Real-time MJPEG video streaming through HTTP

It is designed for the [Freenove 4WD Smart Car Kit for ESP32](https://store.freenove.com/products/fnk0053)

---

## Features

- **Dual-Core FreeRTOS Tasks**: Parallel handling of control commands and video streaming.
- **Camera Integration**: OV2640 camera streaming via HTTP using MJPEG.
- **Wi-Fi AP Mode**: ESP32 acts as its own hotspot for remote control.
- **UDP-based Control**: Motor commands sent over UDP for fast, low-latency input.
- **Motor + Servo Control**: Fine-tuned control using PCA9685 PWM controller.

---

## Getting Started

### 1. Prerequisites

- **Hardware**:
  - [Freenove 4WD Smart Car Kit for ESP32](https://store.freenove.com/products/fnk0053)  
  Includes:
    - ESP32-WROVER CAM Module  
    - ESP32 Car Shield  
    - 4× DC Motors + Wheels  
    - 2× Servo Motors    
- **Software**:
  - [PlatformIO](https://platformio.org/) in VSCode
  - Required libraries: `WiFi.h`, `esp_camera.h`, `WebServer.h`, `PCA9685.h`, `Arduino.h`
 
- **Android App for Gesture Control**:
  - Run this project’s [Android app](https://github.com/kennygoooo/gesture-control-app.git) on your phone.  
  - The app uses the **front camera** to track **hand and foot movements**.
  - Connect your phone to the ESP32’s Wi-Fi Access Point:  
    - **SSID:** `ESP32-Car`  
    - **Password:** `12345678`

 

### 2. Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/kennygoooo/gesture-control-ESP32-robotic-vehicle.git
   
2. **Open the project** in **VSCode** with the **PlatformIO extension** installed.

3. **Connect your ESP32 board** to your computer via **USB**.

4. **Upload the code to the Freenove 4WD Smart Car**:
   - Make sure the correct board is selected in `platformio.ini`.
   - Select the correct serial port.
   - Upload the firmware:
     ```bash
     pio run --target upload
     ```

5. **Open the Serial Monitor** (for debug output):
   ```bash
   pio device monitor
   ```

### Demo Video

Watch the project in action:  
[YouTube Demo](https://youtube.com/shorts/OtwPKSwx-Y8)


---
