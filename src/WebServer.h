#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

#include <ESPAsyncWebServer.h>
#include <string>

#include "Camera.h"
#include "FileSystem.h"
#include "Model.h"

using namespace std;

class WebServer
{
public:
    WebServer(uint16_t port = 50);

private:
    Model mModel;
    AsyncWebServer mWebServer;
    Camera mCamera;
    FileSystem mFileSystem;

    camera_fb_t* captureImage();
};

#endif
