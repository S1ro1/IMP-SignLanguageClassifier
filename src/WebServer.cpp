#include "WebServer.h"

#include <Arduino.h>
#include <WiFi.h>

const char* FILE_PATH = "/photo";

WebServer::WebServer(uint16_t port) : mWebServer(port), mCamera(), mFileSystem()
{

    mWebServer.begin();

    Serial.println("Web server started:");
    Serial.print("URL: http://");
    Serial.print(WiFi.localIP());
    Serial.printf(":%d\r\n", port);

    mWebServer.serveStatic("/", LittleFS, "/");

    mWebServer.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        Serial.println("Get request for /");
        request->send(LittleFS, "/index.html", "text/html");
    });

    mWebServer.on("/capture", HTTP_GET, [this](AsyncWebServerRequest *request) {
        Serial.println("Get request for /capture");
        captureImage();
        request->send_P(200, "text/plain", "OK");
    });

    mWebServer.on("/raw", HTTP_GET, [this](AsyncWebServerRequest *request) {
        Serial.println("Get request for /raw");
        camera_fb_t *frameBuffer = mCamera.TakePhoto();
        request->send_P(200, "application/octet-stream", (const uint8_t *)frameBuffer->buf, frameBuffer->len);
        mCamera.ReleaseFrameBuffer(frameBuffer);
    });

    mWebServer.on("/saved-photo", HTTP_GET, [this](AsyncWebServerRequest *request) {
        Serial.println("Get request for /saved-photo");
        request->send(LittleFS, FILE_PATH, "application/octet-stream", false);
    });

    mWebServer.on("/configure-camera", HTTP_POST, [this](AsyncWebServerRequest *request) {
        Serial.println("Post request for /configure-camera");
        request->send_P(200, "text/plain", "Not implemented yet");
    });
};

void WebServer::captureImage()
{
    camera_fb_t *frameBuffer = mCamera.TakePhoto();
    mFileSystem.SaveFrameBuffer(frameBuffer, FILE_PATH);
    mCamera.ReleaseFrameBuffer(frameBuffer);
}


