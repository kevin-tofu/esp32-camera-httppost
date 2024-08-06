# esp32-camera-httppost

As I couldn't find any existing code that posts images captured by an ESP32 cam to a HttpServer using the ArduinoHttpClient, I implemented it myself.

Before running the code in your environment, you'll need to set up some variables in `./src/setting.h`.

| Name of Variables      | Definition                             | Example   |
|------------------------|----------------------------------------|-----------|
| SECRET_SSID            | Your network SSID                      | "YourSSID" |
| SECRET_PASS            | Your network password                  | "YourPassword" |
| SERVER_IP              | IP address of the HttpServer to receive images from the ESP32 | "192.168.1.1" |
| SERVER_PORT            | Port number of the HttpServer          | 80        |
| CAMERA_TIMEINTERVAL_MSEC | Interval in milliseconds for posting images | 5000     |
