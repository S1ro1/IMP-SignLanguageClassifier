#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Camera.h>
#include <soc/timer_group_struct.h>
#include <soc/timer_group_reg.h>
#include <esp_system.h>

#include "model_data.h"

#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

const char* ssid = "siro";
const char* password = "sirosiro";

tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

constexpr int kTensorArenaSize = 40000;
uint8_t tensor_arena[kTensorArenaSize];

WebServer* webServer;

void setup() {
  Serial.begin(115200);
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  model = tflite::GetModel(model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  static tflite::AllOpsResolver resolver;

  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  input = interpreter->input(0);
  output = interpreter->output(0);

  Serial.printf("Input with shape: [");
  for (int i = 0; i < input->dims->size; i++) {
    Serial.printf("%d, ", input->dims->data[i]);
  }
  Serial.println("]");

  for (int i = 0; i < 784; i++) {
    uint32_t rnd = esp_random(); // Get a random 32-bit unsigned integer
    auto val = (float)rnd / UINT32_MAX;
    input->data.f[i] = val;
  }

  if (kTfLiteOk != interpreter->Invoke()) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed.");
  }

  TfLiteTensor* output = interpreter->output(0);

  Serial.printf("Output with shape: [");
  for (int i = 0; i < output->dims->size; i++) {
    Serial.printf("%d, ", output->dims->data[i]);
  }
  Serial.println("]");

  for (int i = 0; i < 26; i++) {
    Serial.print((char)(i + 64));
    Serial.print(": ");
    Serial.println(output->data.f[i], 2);
  }

  WiFi.begin();


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
