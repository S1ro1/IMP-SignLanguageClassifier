#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

#include <ESPAsyncWebServer.h>
#include <string>

#include "Camera.h"
#include "FileSystem.h"

using namespace std;

class WebServer
{
public:
    WebServer(uint16_t port = 50);

private:
    AsyncWebServer mWebServer;
    Camera mCamera;
    FileSystem mFileSystem;

    void captureImage();
};

#endif
