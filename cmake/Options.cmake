# Copyright (c) 2023 Arm Limited.
#
# SPDX-License-Identifier: MIT
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

include(CMakeDependentOption)

# ---------------------------------------------------------------------

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

option(DEBUG "Enable ['-O0','-g','-gdwarf-2'] compilation flags" OFF)
option(WERROR "Enable the -Werror compilation flag" OFF)
option(EXCEPTIONS "Enable C++ exception support" ON)
option(LOGGING "Enable logging" OFF)

option(GEMM_TUNER "Build gemm_tuner programs" OFF) # Not used atm
option(BUILD_EXAMPLES "Build example programs" OFF)

option(BUILD_TESTING "Build tests" OFF)
option(CPPTHREADS "Enable C++11 threads backend" OFF)
option(OPENMP "Enable OpenMP backend" ON)

#
if(CPPTHREADS)
  add_definitions(-DARM_COMPUTE_CPP_SCHEDULER)
endif()
#
if(LOGGING)
  add_definitions(-DARM_COMPUTE_LOGGING_ENABLED)
endif()

# ---------------------------------------------------------------------
# Backends

# TODO Add help string for each setting (Should user be able to )
option(ENABLE_NEON "Enable Arm® Neon™ support" ON)
option(ARM_COMPUTE_CPU_ENABLED "" ON)
option(ARM_COMPUTE_ENABLE_NEON "" ON)
option(ARM_COMPUTE_ENABLE_FP16 "" ON)
option(ARM_COMPUTE_ENABLE_I8MM "" ON)
option(ENABLE_FP16_KERNELS "" ON)
option(ENABLE_FP32_KERNELS "" ON)
option(ENABLE_QASYMM8_KERNELS "" ON)
option(ENABLE_QASYMM8_SIGNED_KERNELS "" ON)
option(ENABLE_QSYMM16_KERNELS "" ON)
option(ENABLE_INTEGER_KERNELS "" ON)
option(ENABLE_NHWC_KERNELS "" ON)
option(ENABLE_NCHW_KERNELS "" ON)
option(ARM_COMPUTE_GRAPH_ENABLED "" ON)
option(ARM_COMPUTE_ENABLE_BF16 "" ON)
option(ARM_COMPUTE_ENABLE_SVEF32MM "" ON)
option(ARM_COMPUTE_ENABLE_FIXED_FORMAT_KERNELS "" ON)

# TODO Check if this is required
if(ENABLE_NEON)
  add_definitions(-DENABLE_NEON)
endif()
if(ARM_COMPUTE_CPU_ENABLED)
  add_definitions(-DARM_COMPUTE_CPU_ENABLED)
endif()
if(ARM_COMPUTE_ENABLE_NEON)
  add_definitions(-DARM_COMPUTE_ENABLE_NEON)
endif()
if(ARM_COMPUTE_ENABLE_FP16)
  add_definitions(-DARM_COMPUTE_ENABLE_FP16)
endif()
if(ARM_COMPUTE_ENABLE_I8MM)
  add_definitions(-DARM_COMPUTE_ENABLE_I8MM)
endif()
if(ENABLE_FP16_KERNELS)
  add_definitions(-DENABLE_FP16_KERNELS)
endif()
if(ENABLE_FP32_KERNELS)
  add_definitions(-DENABLE_FP32_KERNELS)
endif()
if(ENABLE_QASYMM8_KERNELS)
  add_definitions(-DENABLE_QASYMM8_KERNELS)
endif()
if(ENABLE_QASYMM8_SIGNED_KERNELS)
  add_definitions(-DENABLE_QASYMM8_SIGNED_KERNELS)
endif()
if(ENABLE_QSYMM16_KERNELS)
  add_definitions(-DENABLE_QSYMM16_KERNELS)
endif()
if(ENABLE_INTEGER_KERNELS)
  add_definitions(-DENABLE_INTEGER_KERNELS)
endif()
if(ENABLE_NHWC_KERNELS)
  add_definitions(-DENABLE_NHWC_KERNELS)
endif()
if(ENABLE_NCHW_KERNELS)
  add_definitions(-DENABLE_NCHW_KERNELS)
endif()
if(ARM_COMPUTE_GRAPH_ENABLED)
  add_definitions(-DARM_COMPUTE_GRAPH_ENABLED)
endif()
if(ARM_COMPUTE_ENABLE_BF16)
  add_definitions(-DARM_COMPUTE_ENABLE_BF16)
endif()
if(ARM_COMPUTE_ENABLE_SVEF32MM)
  add_definitions(-DARM_COMPUTE_ENABLE_SVEF32MM)
endif()
if(ARM_COMPUTE_ENABLE_FIXED_FORMAT_KERNELS)
  add_definitions(-DARM_COMPUTE_ENABLE_FIXED_FORMAT_KERNELS)
endif()
