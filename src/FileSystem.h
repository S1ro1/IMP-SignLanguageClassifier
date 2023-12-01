#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include <esp_camera.h>
#include <LittleFS.h>

class FileSystem {
    public:
        FileSystem();
        void SaveFrameBuffer(camera_fb_t* frameBuffer, const char* fileName);
};

#endif
