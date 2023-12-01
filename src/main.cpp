#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Camera.h>
#include <soc/timer_group_struct.h>
#include <soc/timer_group_reg.h>

const char* ssid = "siro";
const char* password = "sirosiro";

WebServer* webServer;

void setup() {
  WiFi.begin();

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }
  Serial.println("Connected to WiFi");

  webServer = new WebServer(80);
}

void loop() {
  delay(1);
  // Ensure the CPU watchdog doesn't get triggered by processing the images
  TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
  TIMERG0.wdt_feed = 1;
  TIMERG0.wdt_wprotect = 0;
}
