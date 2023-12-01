#include "Camera.h"

#include <Esp.h>
#include <esp_err.h>

// These constants define the pin mappings for the ESP-EYE.
// If you are using a different board, you will need to update the mappings
// to match your board.
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 4
#define SIOD_GPIO_NUM 18
#define SIOC_GPIO_NUM 23

#define Y9_GPIO_NUM 36
#define Y8_GPIO_NUM 37
#define Y7_GPIO_NUM 38
#define Y6_GPIO_NUM 39
#define Y5_GPIO_NUM 35
#define Y4_GPIO_NUM 14
#define Y3_GPIO_NUM 13
#define Y2_GPIO_NUM 34
#define VSYNC_GPIO_NUM 5
#define HREF_GPIO_NUM 27
#define PCLK_GPIO_NUM 25

#define XCLK_FREQ 20000000

// Initialise the camera
Camera::Camera()
{
    // Define the configuration for the GPIO pins
    gpio_config_t conf;
    conf.mode = GPIO_MODE_INPUT;
    conf.pull_up_en = GPIO_PULLUP_ENABLE;
    conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    conf.intr_type = GPIO_INTR_DISABLE;
    conf.pin_bit_mask = 1LL << 13;
    gpio_config(&conf);
    conf.pin_bit_mask = 1LL << 14;
    gpio_config(&conf);

    // Define the configuration for the camera, including the
    // pins and the image format
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = XCLK_FREQ;

    // config.pixel_format = PIXFORMAT_JPEG;

    config.pixel_format = PIXFORMAT_GRAYSCALE, 
    config.grab_mode = CAMERA_GRAB_LATEST;

    //init with high specs to pre-allocate larger buffers
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 2;
    config.fb_count = 1;

    // Initialise the camera
    esp_err_t result = esp_camera_init(&config);

    // Check if the camera initialized correctly
    if (result == ESP_OK)
    {
        // drop down frame size for higher initial frame rate
        // This is necessary to avoid timeouts with the SPI file system
        sensor_t *s = esp_camera_sensor_get();
        s->set_framesize(s, FRAMESIZE_QVGA);
    }
    else
    {
        // Reboot the board if the camera init failed
        Serial.printf("Camera init failed with error 0x%x", result);
        ESP.restart();
    }
}

camera_fb_t *Camera::TakePhoto()
{
    camera_fb_t* fb = NULL;
    fb = esp_camera_fb_get();
    esp_camera_fb_return(fb);
    fb = NULL;
    fb = esp_camera_fb_get();

    if (!fb)
    {
        Serial.println("Camera capture failed");
    }
    return fb;
}

// Releases the frame buffer allocated by the camera
// freeing up the memory
void Camera::ReleaseFrameBuffer(camera_fb_t *frameBuffer)
{
    // Return the frame buffer so it can be re-used
    esp_camera_fb_return(frameBuffer);
}
