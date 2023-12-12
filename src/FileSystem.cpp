#include "FileSystem.h"

FileSystem::FileSystem() {
    if (!LittleFS.begin()) {
        Serial.println("An Error has occurred while mounting ");
    }
    Serial.println("Filesystem mounted successfully");
}

void FileSystem::SaveFrameBuffer(camera_fb_t* frameBuffer, const char* fileName) {
    Serial.printf("Saving framebuffer to file: %s\r\n", fileName);
    int retry = 0;

    while (retry < 10) {
        if (LittleFS.exists(fileName))
        {
            LittleFS.remove(fileName);
        }

        File file = LittleFS.open(fileName, FILE_WRITE);

        if (!file)
        {
            Serial.println("Failed to open file in writing mode");
            ++retry;
        }
        else
        {
            int ret = file.write(frameBuffer->buf, frameBuffer->len);
            if (ret <= 0) {
                Serial.println("Failed to write to file");
            }

            file.close();
            return;
        }
    }
}

void FileSystem::SaveArray(uint8_t * array, int len, const char * fileName)
{
    Serial.printf("Saving array to file: %s\r\n", fileName);
    int retry = 0;

    while (retry < 10) {
        if (LittleFS.exists(fileName))
        {
            LittleFS.remove(fileName);
        }

        File file = LittleFS.open(fileName, FILE_WRITE);

        if (!file)
        {
            Serial.println("Failed to open file in writing mode");
            ++retry;
        }
        else
        {
            int ret = file.write(array, len);
            if (ret <= 0) {
                Serial.println("Failed to write to file");
            }

            file.close();
            return;
        }
    }
}
