//
// Created by Matej Sirovatka on 12.12.2023.
//

#ifndef IMP_SIGNLANGUAGECLASSIFIER_IMAGEUTILS_H
#define IMP_SIGNLANGUAGECLASSIFIER_IMAGEUTILS_H

#include <stdint.h>
#include <esp_camera.h>

const int WIDTH = 28;
const int HEIGHT = 28;

namespace Utils {
    uint8_t getpixel(camera_fb_t *image, unsigned int x, unsigned int y);
    void putpixel(uint8_t* dst, unsigned int x, unsigned int y, uint8_t color);
    float lerp(float s, float e, float t);
    float blerp(float c00, float c10, float c01, float c11, float tx, float ty);
    void scale(camera_fb_t *src, uint8_t *dst, float scalex, float scaley);
}

uint8_t Utils::getpixel(camera_fb_t *image, unsigned int x, unsigned int y){
    return image->buf[(y * image->width) + x];
}

void Utils::putpixel(uint8_t* dst, unsigned int x, unsigned int y, uint8_t color){
    dst[(y * WIDTH) + x] = color;
}

float Utils::lerp(float s, float e, float t) {
    return s + (e - s) * t;
}

float Utils::blerp(float c00, float c10, float c01, float c11, float tx, float ty){
    return Utils::lerp(Utils::lerp(c00, c10, tx), Utils::lerp(c01, c11, tx), ty);
}

void Utils::scale(camera_fb_t *src, uint8_t *dst, float scalex, float scaley){
    int newWidth = (int)(src->width * scalex);
    int newHeight = (int)(src->height * scaley);

    int x, y;
    for(y = 0; y < newHeight; y++) {
        for(x = 0; x < newWidth; x++) {
            float gx = x / (float)newWidth * (src->width - 1);
            float gy = y / (float)newHeight * (src->height - 1);
            int gxi = (int)gx;
            int gyi = (int)gy;
            uint8_t c00 = Utils::getpixel(src, gxi, gyi);
            uint8_t c10 = Utils::getpixel(src, gxi + 1, gyi);
            uint8_t c01 = Utils::getpixel(src, gxi, gyi + 1);
            uint8_t c11 = Utils::getpixel(src, gxi + 1, gyi + 1);
            uint8_t result = (uint8_t)Utils::blerp(c00, c10, c01, c11, gx - gxi, gy - gyi);
            Utils::putpixel(dst, x, y, result);
        }
    }
}

#endif //IMP_SIGNLANGUAGECLASSIFIER_IMAGEUTILS_H
