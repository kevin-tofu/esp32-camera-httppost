#include "Arduino.h"
#include <FS.h>
#include <ArduinoHttpClient.h>
#include <SPI.h>
#include <WiFi.h>
#include "esp_camera.h"
#include "settings.h"

///////please enter your sensitive data in the Secret setting.h
/////// WiFi Settings ///////
char ssid[] = SECRET_SSID;
char password[] = SECRET_PASS;
char serverAddress[] = SERVER_IP;
int port = int(SERVER_PORT);

void wifi_setup() {
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println(String("Connecting to ") + ssid);
  while ( WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);
    Serial.println(WiFi.status());
  }
  Serial.println("\nConnected, IP address: ");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  WiFiClient wifi;
  HttpClient client = HttpClient(wifi, serverAddress, port);
}

void camera_setup() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  // config.pin_sscb_sda = SIOD_GPIO_NUM;
  // config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size   = FRAMESIZE_QVGA; // 320x240
//    config.frame_size   = FRAMESIZE_UXGA; // 1600x1200
  config.jpeg_quality = 10;
  config.fb_count     = 2;
  config.fb_location = CAMERA_FB_IN_DRAM;

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  }
}

void setup() {
  // Serial.begin(9600);
  Serial.begin(115200);
  delay(5000);
  Serial.println("Setup start");

  // 
  Serial.print("Camera  SetUp Begin");
  camera_setup();
  Serial.print("Camera  SetUp Finished");
  // 
  Serial.print("Wifi SetUp Begin");
  wifi_setup();
  Serial.print("Wifi SetUp Finished");
}

void postImage(const uint8_t *imageData, size_t imageSize) {

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("POST request");

    WiFiClient wifi;
    HttpClient client = HttpClient(wifi, serverAddress, port);
    // Create header/footer
    // Measure content and header-footer length
    String boundary = "Boundary123";
    String header = "--" + boundary + "\r\n"
                  + "Content-Disposition: form-data; name=\"file\"; filename=\"image.jpg\"\r\n"
                  + "Content-Type: image/jpeg\r\n\r\n";
    String footer = "\r\n--" + boundary + "--\r\n";
    size_t totalLength = header.length() + imageSize + footer.length();

    // Begin Http Post Request
    client.beginRequest();
    client.post("/image");
    client.sendHeader("Content-Type", "multipart/form-data; boundary=" + boundary);
    client.sendHeader("Content-Length", totalLength);

    // write footer
    client.print(header);

    // write image file
    client.write(imageData, imageSize);

    // write footer
    client.print(footer);

    // End Request
    client.endRequest();

    // Check Response
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();
    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
  }
  else {
    Serial.println("Error in WiFi connection");
  }
  delay(500);
}

void loop() {

  unsigned long startTime = millis();

  // capture Image
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }
  Serial.printf("Captured image with size %d bytes\n", fb->len);

  postImage(fb->buf, fb->len);
  
  // release buffer
  esp_camera_fb_return(fb);

  unsigned long endTime = millis();

  // elapsedTime
  unsigned long elapsedTime = endTime - startTime;
  Serial.print("Task took ");
  Serial.print(elapsedTime);
  Serial.println(" milliseconds.");

  // wait 5 sec
  delay(CAMERA_TIMEINTERVAL_MSEC);

}
