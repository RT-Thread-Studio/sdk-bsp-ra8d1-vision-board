/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef SIGNAL_SRC_FILTER_BANK_SQUARE_ROOT_H_
#define SIGNAL_SRC_FILTER_BANK_SQUARE_ROOT_H_

#include <stdint.h>

namespace tflite {
namespace tflm_signal {
// TODO(b/286250473): remove namespace once de-duped libraries above

// Apply square root to each element in `input`, then shift right by
// `scale_down_bits` before writing the result to `output`,
// `input` and `output` must both be of size `num_channels`
void FilterbankSqrt(const uint64_t* input, int num_channels,
                    int scale_down_bits, uint32_t* output);

}  // namespace tflm_signal
}  // namespace tflite

#endif  // SIGNAL_SRC_FILTER_BANK_SQUARE_ROOT_H_
