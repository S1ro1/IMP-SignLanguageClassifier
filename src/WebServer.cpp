#include "WebServer.h"

#include <Arduino.h>
#include <WiFi.h>
#include "ImageUtils.h"

const char* FILE_PATH = "/photo";

WebServer::WebServer(uint16_t port) : mWebServer(port), mCamera(), mFileSystem(), mModel()
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
        auto fb = captureImage();

        uint8_t input[784] = {};
        Utils::scale(fb, input, 28 / 320.0, 28 / 240.0);
        mModel.setInput(input, WIDTH * HEIGHT);

        auto output = mModel.runInference();

        std::string response = "[";
        for (int i = 0; i < 26; i++) {
            response += std::to_string(output->data.f[i]) + ",";
        }
        response.pop_back();
        response.push_back(']');

        request->send_P(200, "text/plain", response.c_str());
        mCamera.ReleaseFrameBuffer(fb);
    });

    mWebServer.on("/saved-photo", HTTP_GET, [this](AsyncWebServerRequest *request) {
        Serial.println("Get request for /saved-photo");
        request->send(LittleFS, FILE_PATH, "application/octet-stream", false);
    });

    mWebServer.on("/small-photo", HTTP_GET, [this](AsyncWebServerRequest *request) {
        Serial.println("Get request for /small-photo");
        request->send(LittleFS, "/small-photo.jpg", "application/octet-stream", false);
    });

    mWebServer.on("/configure-camera", HTTP_POST, [this](AsyncWebServerRequest *request) {
        Serial.println("Post request for /configure-camera");
        request->send_P(200, "text/plain", "Not implemented yet");
    });
};

camera_fb_t* WebServer::captureImage()
{
    camera_fb_t *frameBuffer = mCamera.TakePhoto();
    mFileSystem.SaveFrameBuffer(frameBuffer, FILE_PATH);

    uint8_t input[784] = {};
    Utils::scale(frameBuffer, input, 28 / 320.0, 28 / 240.0);

    mFileSystem.SaveArray(input, WIDTH * HEIGHT, "/small-photo.jpg");

    return frameBuffer;
}


