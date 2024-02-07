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

// Reference implementation of the DebugLog() function that's required for a
// platform to support the TensorFlow Lite for Microcontrollers library. This is
// the only function that's absolutely required to be available on a target
// device, since it's used for communicating test results back to the host so
// that we can verify the implementation is working correctly.
// This function should support standard C/C++ stdio style formatting
// operations. It's designed to be as easy as possible to supply an
// implementation though. On platforms that have a POSIX stack or C library, it
// can be written as a single call to `vfprintf(stderr, format, args)` to output
// a string to the error stream of the console, but if there's no OS or C
// library available, there's almost always an equivalent way to write out a
// string to some serial interface that can be used instead. To add an
// equivalent function for your own platform, create your own implementation
// file, and place it in a subfolder with named after the OS you're targeting.
// For example, see the Cortex M bare metal version in the
// tensorflow/lite/micro/bluepill/debug_log.cc file.

#include "tensorflow/lite/micro/debug_log.h"

#ifndef TF_LITE_STRIP_ERROR_STRINGS
#include <cstdio>
#endif

extern "C" void DebugLog(const char* format, va_list args) {
#ifndef TF_LITE_STRIP_ERROR_STRINGS
  // Reusing TF_LITE_STRIP_ERROR_STRINGS to disable DebugLog completely to get
  // maximum reduction in binary size. This is because we have DebugLog calls
  // via TF_LITE_CHECK that are not stubbed out by TF_LITE_REPORT_ERROR.
  vfprintf(stderr, format, args);
#endif
}

#ifndef TF_LITE_STRIP_ERROR_STRINGS
// Only called from MicroVsnprintf (micro_log.h)
extern "C" int DebugVsnprintf(char* buffer, size_t buf_size, const char* format,
                              va_list vlist) {
  return vsnprintf(buffer, buf_size, format, vlist);
}
#endif
