/* This file is part of the OpenMV project.
 * Copyright (c) 2013-2023 Ibrahim Abdelkader <iabdalkader@openmv.io> & Kwabena W. Agyeman <kwagyeman@openmv.io>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 */

#include <stdio.h>
#include <string.h>
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/cortex_m_generic/debug_log_callback.h"
#include "tensorflow/lite/micro/examples/micro_speech/micro_features/micro_features_generator.h"
#include "tensorflow/lite/micro/micro_interpreter.h"

#include "libtf.h"

#define LIBTF_MAX_OPS 34

extern "C" {

    // These are set by openmv py_tf.c code to redirect printing to an error message buffer...
    char *py_tf_putchar_buffer = NULL;
    size_t py_tf_putchar_buffer_index = 0;
    size_t py_tf_putchar_buffer_len = 0;

    static void libtf_debug_log(const char *s)
    {
        for (size_t i = 0, j = strlen(s); i < j; i++) {
            if (py_tf_putchar_buffer_len) {
                py_tf_putchar_buffer[py_tf_putchar_buffer_index++] = s[i];
                py_tf_putchar_buffer_len--;
            } else {
                putchar(s[i]);
            }
        }
    }

    static int libtf_align_tensor_arena(unsigned char **tensor_arena, size_t *tensor_arena_size)
    {
        size_t alignment = ((size_t) (*tensor_arena)) % LIBTF_TENSOR_ARENA_ALIGNMENT;

        if (alignment) {

            size_t fix = LIBTF_TENSOR_ARENA_ALIGNMENT - alignment;

            if ((*tensor_arena_size) < fix) {
                return 1;
            }

            (*tensor_arena) += fix;
            (*tensor_arena_size) -= fix;
        }

        return 0;
    }

    static bool libtf_valid_dataype(TfLiteType type)
    {
        return (type == kTfLiteUInt8) || (type == kTfLiteInt8) || (type == kTfLiteFloat32);
    }

    static libtf_datatype_t libtf_map_datatype(TfLiteType type)
    {
        if (type == kTfLiteUInt8) {
            return LIBTF_DATATYPE_UINT8;
        } else if (type == kTfLiteInt8) {
            return LIBTF_DATATYPE_INT8;
        } else {
            return LIBTF_DATATYPE_FLOAT;
        }
    }

    static void libtf_init_op_resolver(tflite::MicroMutableOpResolver<LIBTF_MAX_OPS> &resolver)
    {
        resolver.AddAbs();
        resolver.AddAdd();
        resolver.AddAddN();
        // resolver.AddArgMax();
        resolver.AddArgMin();
        // resolver.AddAssignVariable();
        resolver.AddAveragePool2D();
        // resolver.AddBatchToSpaceNd();
        // resolver.AddCallOnce();
        // resolver.AddCeil();
        // resolver.AddConcatenation();
        resolver.AddConv2D();
        // resolver.AddCos();
        // resolver.AddCumSum();
        // resolver.AddDepthToSpace();
        resolver.AddDepthwiseConv2D();
        resolver.AddDequantize();
        // resolver.AddDetectionPostprocess();
        // resolver.AddElu();
        // resolver.AddEqual();
        // resolver.AddEthosU();
        // resolver.AddExpandDims();
        resolver.AddFloor();
        // resolver.AddFloorDiv();
        // resolver.AddFloorMod();
        resolver.AddFullyConnected();
        // resolver.AddGreater();
        // resolver.AddGreaterEqual();
        // resolver.AddHardSwish();
        // resolver.AddL2Normalization();
        // resolver.AddL2Pool2D();
        resolver.AddLeakyRelu();
        resolver.AddLess();
        // resolver.AddLessEqual();
        // resolver.AddLog();
        // resolver.AddLogicalAnd();
        // resolver.AddLogicalNot();
        // resolver.AddLogicalOr();
        resolver.AddLogistic();
        resolver.AddMaximum();
        resolver.AddMaxPool2D();
        resolver.AddMean();
        resolver.AddMinimum();
        resolver.AddMul();
        // resolver.AddNeg();
        resolver.AddNotEqual();
        resolver.AddPack();
        resolver.AddPad();
        // resolver.AddPadV2();
        // resolver.AddPrelu();
        resolver.AddQuantize();
        // resolver.AddReadVariable();
        // resolver.AddReduceMax();
        resolver.AddRelu();
        resolver.AddRelu6();
        resolver.AddReshape();
        // resolver.AddResizeBilinear();
        resolver.AddResizeNearestNeighbor();
        // resolver.AddRound();
        // resolver.AddRsqrt();
        resolver.AddShape();
        // resolver.AddSin();
        resolver.AddSoftmax();
        // resolver.AddSpaceToBatchNd();
        // resolver.AddSpaceToDepth();
        // resolver.AddSplit();
        // resolver.AddSplitV();
        resolver.AddSqrt();
        // resolver.AddSquare();
        // resolver.AddSqueeze();
        resolver.AddStridedSlice();
        resolver.AddSub();
        // resolver.AddSvdf();
        resolver.AddTanh();
        // resolver.AddTranspose();
        resolver.AddTransposeConv();
        resolver.AddUnpack();
        // resolver.AddVarHandle();
    }

    int libtf_get_parameters(const unsigned char *model_data,
                             unsigned char *tensor_arena, size_t tensor_arena_size,
                             libtf_parameters_t *params)
    {
        RegisterDebugLogCallback(libtf_debug_log);

        tflite::MicroErrorReporter micro_error_reporter;
        tflite::ErrorReporter *error_reporter = &micro_error_reporter;

        const tflite::Model *model = tflite::GetModel(model_data);

        if (model->version() != TFLITE_SCHEMA_VERSION) {
            error_reporter->Report("Model provided is schema version is not equal to supported version!");
            return 1;
        }

        if (libtf_align_tensor_arena(&tensor_arena, &tensor_arena_size)) {
            error_reporter->Report("Align failed!");
            return 1;
        }

        tflite::MicroMutableOpResolver<LIBTF_MAX_OPS> resolver;
        libtf_init_op_resolver(resolver);

        tflite::MicroInterpreter interpreter(model, resolver, tensor_arena, tensor_arena_size);

        if (interpreter.AllocateTensors() != kTfLiteOk) {
            error_reporter->Report("AllocateTensors() failed!");
            return 1;
        }

        params->tensor_arena_size = interpreter.arena_used_bytes() + 1024;

        // Handle input parameters.
        {
            TfLiteTensor *model_input = interpreter.input(0);

            if (!libtf_valid_dataype(model_input->type)) {
                error_reporter->Report("Input model data type should be 8-bit quantized!");
                return 1;
            }

            if (model_input->dims->size == 2) {

                params->input_height = model_input->dims->data[0];
                params->input_width = model_input->dims->data[1];
                params->input_channels = 1;

            } else if (model_input->dims->size == 3) {

                if ((model_input->dims->data[2] != 1) && (model_input->dims->data[2] != 3)) {
                    error_reporter->Report("Input dimension [c] should be 1 or 3!");
                    return 1;
                }

                params->input_height = model_input->dims->data[0];
                params->input_width = model_input->dims->data[1];
                params->input_channels = model_input->dims->data[2];

            } else if (model_input->dims->size == 4) {

                if (model_input->dims->data[0] != 1) {
                    error_reporter->Report("Input dimension [n] should be 1!");
                    return 1;
                }

                if ((model_input->dims->data[3] != 1) && (model_input->dims->data[3] != 3)) {
                    error_reporter->Report("Input dimension [c] should be 1 or 3!");
                    return 1;
                }

                params->input_height = model_input->dims->data[1];
                params->input_width = model_input->dims->data[2];
                params->input_channels = model_input->dims->data[3];

            } else {
                error_reporter->Report("Input dimensions should be [h][w](c=1), [h][w][c==1||c==3], or [n==1][h][w][c==1||c==3]!");
                return 1;
            }

            params->input_datatype = libtf_map_datatype(model_input->type);
            params->input_scale = model_input->params.scale;
            params->input_zero_point = model_input->params.zero_point;
        }

        // Handle output parameters.
        {
            TfLiteTensor *model_output = interpreter.output(0);

            if (!libtf_valid_dataype(model_output->type)) {
                error_reporter->Report("Output model data type should be 8-bit quantized!");
                return 1;
            }

            if (model_output->dims->size == 1) {

                params->output_height = 1;
                params->output_width = 1;
                params->output_channels = model_output->dims->data[0];

            } else if (model_output->dims->size == 2) {

                if (model_output->dims->data[0] != 1) {
                    error_reporter->Report("Output dimension [n] should be 1!");
                    return 1;
                }

                params->output_height = 1;
                params->output_width = 1;
                params->output_channels = model_output->dims->data[1];

            } else if (model_output->dims->size == 3) {

                params->output_height = model_output->dims->data[0];
                params->output_width = model_output->dims->data[1];
                params->output_channels = model_output->dims->data[2];

            } else if (model_output->dims->size == 4) {

                if (model_output->dims->data[0] != 1) {
                    error_reporter->Report("Output dimension [n] should be 1!");
                    return 1;
                }

                params->output_height = model_output->dims->data[1];
                params->output_width = model_output->dims->data[2];
                params->output_channels = model_output->dims->data[3];

            } else {
                error_reporter->Report("Output dimensions should be [c], [n==1][c], [h][w][c], or [n==1][h][w][c]!");
                return 1;
            }

            params->output_datatype = libtf_map_datatype(model_output->type);
            params->output_scale = model_output->params.scale;
            params->output_zero_point = model_output->params.zero_point;
        }

        return 0;
    }

    int libtf_invoke(const unsigned char *model_data,
                     unsigned char *tensor_arena,
                     libtf_parameters_t *params,
                     libtf_input_data_callback_t input_callback,
                     void *input_callback_data,
                     libtf_output_data_callback_t output_callback,
                     void *output_callback_data)
    {
        RegisterDebugLogCallback(libtf_debug_log);

        tflite::MicroErrorReporter micro_error_reporter;
        tflite::ErrorReporter *error_reporter = &micro_error_reporter;

        const tflite::Model *model = tflite::GetModel(model_data);

        if (model->version() != TFLITE_SCHEMA_VERSION) {
            error_reporter->Report("Model provided is schema version is not equal to supported version!");
            return 1;
        }

        size_t tensor_arena_size = params->tensor_arena_size;

        if (libtf_align_tensor_arena(&tensor_arena, &tensor_arena_size)) {
            error_reporter->Report("Align failed!");
            return 1;
        }

        tflite::MicroMutableOpResolver<LIBTF_MAX_OPS> resolver;
        libtf_init_op_resolver(resolver);

        tflite::MicroInterpreter interpreter(model, resolver, tensor_arena, tensor_arena_size);

        if (interpreter.AllocateTensors() != kTfLiteOk) {
            error_reporter->Report("AllocateTensors() failed!");
            return 1;
        }

        input_callback(input_callback_data, interpreter.input(0)->data.data, params);

        if (interpreter.Invoke() != kTfLiteOk) {
            error_reporter->Report("Invoke() failed!");
            return 1;
        }

        output_callback(output_callback_data, interpreter.output(0)->data.data, params);

        return 0;
    }

    int libtf_initialize_micro_features()
    {
        RegisterDebugLogCallback(libtf_debug_log);

        tflite::MicroErrorReporter micro_error_reporter;
        tflite::ErrorReporter *error_reporter = &micro_error_reporter;

//        if (InitializeMicroFeatures(error_reporter) != kTfLiteOk) {
//            return 1;
//        }

        return 0;
    }

    int libtf_generate_micro_features(const int16_t *input, int input_size,
            int output_size, int8_t *output, size_t *num_samples_read)
    {
        RegisterDebugLogCallback(libtf_debug_log);

        tflite::MicroErrorReporter micro_error_reporter;
        tflite::ErrorReporter *error_reporter = &micro_error_reporter;

//        if (GenerateMicroFeatures(error_reporter, input, input_size,
//                    output_size, output, num_samples_read) != kTfLiteOk) {
//            return 1;
//        }

        return 0;
    }

    int libtf_regression(const unsigned char *model_data, uint8_t* tensor_arena, libtf_parameters_t* params, float* input_data, float* output_data)
    {
        RegisterDebugLogCallback(libtf_debug_log);

        tflite::MicroErrorReporter micro_error_reporter;
        tflite::ErrorReporter *error_reporter = &micro_error_reporter;

        const tflite::Model* model = tflite::GetModel(model_data);
        if (model->version() != TFLITE_SCHEMA_VERSION) {
            error_reporter->Report("Model provided is schema version is not equal to supported version!");
            return 1;
        }

        tflite::MicroMutableOpResolver<LIBTF_MAX_OPS> resolver;
        libtf_init_op_resolver(resolver);

        size_t kTensorArenaSize = params->tensor_arena_size;

        tflite::MicroInterpreter interpreter(model, resolver, tensor_arena, 
                                        kTensorArenaSize);
        if (interpreter.AllocateTensors() != kTfLiteOk) {
            error_reporter->Report("AllocateTensors() failed!");
            return 1;
        }

        TfLiteTensor* input = interpreter.input(0);
        float input_scale = input->params.scale;
        int input_zero_point = input->params.zero_point;

        if (!libtf_valid_dataype(input->type)) {
                error_reporter->Report("Input model data type should be 8-bit quantized!");
                return 1;
        }

        if (input->type == kTfLiteUInt8) {
            for(size_t i=0; i < params->input_height; i++){
                for(size_t j=0; j < params->input_width; j++){
                    input->data.uint8[i * (params->input_width) + j] = (uint8_t)(input_data[i * (params->input_width) + j] / input_scale + input_zero_point);
                }
            }
        }
        else if (input->type == kTfLiteInt8) {
            for(size_t i=0; i < params->input_height; i++){
                for(size_t j=0; j < params->input_width; j++){
                    input->data.int8[i * (params->input_width) + j] = (int8_t)(input_data[i * (params->input_width) + j] / input_scale + input_zero_point);
                }
            }

        }
        else if ((input->type == kTfLiteFloat32) || (input->type == kTfLiteFloat16)) {
            for(size_t i=0; i < params->input_height; i++){
                for(size_t j=0; j < params->input_width; j++){
                    input->data.f[i * (params->input_width) + j] = (float)(input_data[i * (params->input_width) + j]);
                }
            }
        }

        TfLiteStatus invoke_status = interpreter.Invoke();
        if (invoke_status != kTfLiteOk) {
            error_reporter->Report("Invoke() failed!");
            return 1;
        }

        TfLiteTensor* output = interpreter.output(0);
        float output_scale = output->params.scale;
        int output_zero_point = output->params.zero_point;
        if (input->type == kTfLiteUInt8) {
                for(size_t i=0; i<params->output_channels; i++){
                    output_data[i] = (float) ((output->data.uint8[i] - output_zero_point) * output_scale);
            } 
        }
        else if (input->type == kTfLiteInt8) {
                for(size_t i=0; i<params->output_channels; i++){
                    output_data[i] = (float) ((output->data.int8[i] - output_zero_point) * output_scale);
            } 
        }
        else if (input->type == kTfLiteFloat32) {
                for(size_t i=0; i<params->output_channels; i++){
                    output_data[i] = (float) (output->data.f[i]);
            }
        }

        return 0;
    }
}
