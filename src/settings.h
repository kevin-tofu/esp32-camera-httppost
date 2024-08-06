
#ifndef SETTINGS_H
#define SETTINGS_H

#define CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 21
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 19
#define Y4_GPIO_NUM 18
#define Y3_GPIO_NUM 5
#define Y2_GPIO_NUM 4
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22
#define TFT_SCLK 14
#define TFT_MISO -1
#define TFT_MOSI 13
#define TFT_CS 15 // Chip select control pin
#define TFT_DC 32 // Data Command control pin
#define TFT_RST 33

// Pin button
#define PIN_BTN 12

#define SECRET_SSID "yourssid"
#define SECRET_PASS "yourpass"

#define SERVER_IP "0.0.0.0"
#define SERVER_PORT 8080

#define CAMERA_TIMEINTERVAL_MSEC 5000

#endif