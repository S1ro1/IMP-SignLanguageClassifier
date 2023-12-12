//
// Created by Matej Sirovatka on 12.12.2023.
//

#include "model_data.h"
#include "model.h"
#include "Arduino.h"

Model::Model() {
    static tflite::MicroErrorReporter micro_error_reporter;
    mErrorReporter = &micro_error_reporter;

    mModel = tflite::GetModel(model_data);
    if (mModel->version() != TFLITE_SCHEMA_VERSION) {
        TF_LITE_REPORT_ERROR(mErrorReporter,
                             "Model provided is schema version %d not equal "
                             "to supported version %d.",
                             mModel->version(), TFLITE_SCHEMA_VERSION);
        return;
    }

    static tflite::AllOpsResolver resolver;

    static tflite::MicroInterpreter static_interpreter(
            mModel, resolver, mTensorArena, kTensorArenaSize, mErrorReporter);
    mInterpreter = &static_interpreter;

    TfLiteStatus allocate_status = mInterpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        TF_LITE_REPORT_ERROR(mErrorReporter, "AllocateTensors() failed");
        return;
    }
    mInput = mInterpreter->input(0);
    mOutput = mInterpreter->output(0);
}

void Model::setInput(uint8_t *input, int size) {
    for (int i = 0; i < size; i++) {
        mInput->data.f[i] = input[i] / 255.0;
    }

}

TfLiteTensor *Model::runInference() {
    TfLiteStatus invoke_status = mInterpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
        TF_LITE_REPORT_ERROR(mErrorReporter, "Invoke failed");
        return nullptr;
    }

    auto output = mInterpreter->output(0);
    return output;
}
