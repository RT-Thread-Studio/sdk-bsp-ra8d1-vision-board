/* Copyright 2023 The TensorFlow Authors. All Rights Reserved.

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

#ifndef SIGNAL_SRC_COMPLEX_H_
#define SIGNAL_SRC_COMPLEX_H_

#include <stdint.h>

// We would use the standard complex type in complex.h, but there's
// no guarantee that all architectures will support it.
template <typename T>
struct Complex {
  T real;
  T imag;
};

#endif  // SIGNAL_SRC_COMPLEX_H_
