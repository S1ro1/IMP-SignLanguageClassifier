//
// Created by Matej Sirovatka on 12.12.2023.
//

#ifndef IMP_SIGNLANGUAGECLASSIFIER_MODEL_H
#define IMP_SIGNLANGUAGECLASSIFIER_MODEL_H

#include <cstdint>

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"


class Model {
public:
    Model();
    void setInput(uint8_t* input, int size);
    TfLiteTensor* runInference();

private:
    tflite::ErrorReporter* mErrorReporter = nullptr;
    const tflite::Model* mModel = nullptr;
    tflite::MicroInterpreter* mInterpreter = nullptr;
    TfLiteTensor* mInput = nullptr;
    TfLiteTensor* mOutput = nullptr;

    constexpr static int kTensorArenaSize = 40000;
    uint8_t mTensorArena[kTensorArenaSize];
};


#endif //IMP_SIGNLANGUAGECLASSIFIER_MODEL_H
