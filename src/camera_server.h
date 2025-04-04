#ifndef CAMERA_SERVER_H
#define CAMERA_SERVER_H

#include <WebServer.h>

// 在其他檔案中使用這個 server（main 裡初始化）
extern WebServer server;

void initCamera();
void startCameraServer();
void handleJPGStream();

#endif
