/*
 * Copyright (c) 2022 Arm Limited.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#if defined(ARM_COMPUTE_ENABLE_SME)

#include "src/core/NEON/kernels/assembly/pooling.hpp"
#include <cstdint>

namespace arm_conv {
namespace pooling {


void sme_s8q_nhwc_max_generic_depthfirst_impl(
  const uint64_t,
  const uint64_t n_valid_cells,
  uint64_t n_channels,
  const int8_t *const *const inptrs,
  int8_t *outptr,
  const Requantize32 &qp
)
{
  __asm__ __volatile__(
    ".inst 0xd503477f  // SMSTART ZA\n"
    "mov x28, #0x0\n"
    "cntb x27\n"
    "cntb x26, ALL, MUL #2\n"
    "cntb x25, ALL, MUL #3\n"
    "whilelt p4.b, x28, %x[n_channels]\n"
    "whilelt p3.b, x27, %x[n_channels]\n"
    "whilelt p2.b, x26, %x[n_channels]\n"
    "whilelt p1.b, x25, %x[n_channels]\n"
    "ptrue p0.b\n"
    "b.none 7f\n"
    "1:"  // 4-vectors of channels
    "lsr x24, %x[n_valid_cells], #0x2\n"
    "mov z4.b, #0x80\n"
    "mov z3.b, #0x80\n"
    "mov x19, %x[inptrs]\n"
    "mov z2.b, #0x80\n"
    "mov z1.b, #0x80\n"
    "cbz x24, 4f\n"
    "ldp x23, x22, [x19, #0x0]\n"
    "subs x24, x24, #0x1\n"
    "ld1b { z0.b }, p4/Z, [x23, x28]\n"
    "ldp x21, x20, [x19, #0x10]\n"
    "add x19, x19, #0x20\n"
    "ld1b { z31.b }, p4/Z, [x22, x28]\n"
    "ld1b { z23.b }, p4/Z, [x21, x28]\n"
    "ld1b { z30.b }, p4/Z, [x20, x28]\n"
    "ld1b { z18.b }, p3/Z, [x23, x27]\n"
    "ld1b { z29.b }, p3/Z, [x22, x27]\n"
    "ld1b { z22.b }, p3/Z, [x21, x27]\n"
    "ld1b { z28.b }, p3/Z, [x20, x27]\n"
    "ld1b { z17.b }, p2/Z, [x23, x26]\n"
    "ld1b { z27.b }, p2/Z, [x22, x26]\n"
    "ld1b { z21.b }, p2/Z, [x21, x26]\n"
    "ld1b { z26.b }, p2/Z, [x20, x26]\n"
    "ld1b { z16.b }, p1/Z, [x23, x25]\n"
    "ld1b { z25.b }, p1/Z, [x22, x25]\n"
    "ld1b { z20.b }, p1/Z, [x21, x25]\n"
    "ld1b { z24.b }, p1/Z, [x20, x25]\n"
    "beq 3f\n"
    "2:"  // 4-vectors of channels: 4 inputs loop
    "movprfx z19, z0\n smax z19.b, p0/M, z19.b, z31.b\n"
    "smax z23.b, p0/M, z23.b, z30.b\n"
    "ldp x23, x22, [x19, #0x0]\n"
    "subs x24, x24, #0x1\n"
    "smax z18.b, p0/M, z18.b, z29.b\n"
    "smax z22.b, p0/M, z22.b, z28.b\n"
    "ldp x21, x20, [x19, #0x10]\n"
    "add x19, x19, #0x20\n"
    "smax z17.b, p0/M, z17.b, z27.b\n"
    "smax z21.b, p0/M, z21.b, z26.b\n"
    "ld1b { z0.b }, p4/Z, [x23, x28]\n"
    "smax z16.b, p0/M, z16.b, z25.b\n"
    "smax z20.b, p0/M, z20.b, z24.b\n"
    "ld1b { z31.b }, p4/Z, [x22, x28]\n"
    "smax z19.b, p0/M, z19.b, z23.b\n"
    "smax z18.b, p0/M, z18.b, z22.b\n"
    "ld1b { z23.b }, p4/Z, [x21, x28]\n"
    "smax z17.b, p0/M, z17.b, z21.b\n"
    "smax z16.b, p0/M, z16.b, z20.b\n"
    "ld1b { z30.b }, p4/Z, [x20, x28]\n"
    "smax z4.b, p0/M, z4.b, z19.b\n"
    "smax z3.b, p0/M, z3.b, z18.b\n"
    "ld1b { z18.b }, p3/Z, [x23, x27]\n"
    "smax z2.b, p0/M, z2.b, z17.b\n"
    "smax z1.b, p0/M, z1.b, z16.b\n"
    "ld1b { z29.b }, p3/Z, [x22, x27]\n"
    "ld1b { z22.b }, p3/Z, [x21, x27]\n"
    "ld1b { z28.b }, p3/Z, [x20, x27]\n"
    "ld1b { z17.b }, p2/Z, [x23, x26]\n"
    "ld1b { z27.b }, p2/Z, [x22, x26]\n"
    "ld1b { z21.b }, p2/Z, [x21, x26]\n"
    "ld1b { z26.b }, p2/Z, [x20, x26]\n"
    "ld1b { z16.b }, p1/Z, [x23, x25]\n"
    "ld1b { z25.b }, p1/Z, [x22, x25]\n"
    "ld1b { z20.b }, p1/Z, [x21, x25]\n"
    "ld1b { z24.b }, p1/Z, [x20, x25]\n"
    "bgt 2b\n"
    "3:"  // 4-vectors of channels: 4 inputs tail
    "movprfx z19, z0\n smax z19.b, p0/M, z19.b, z31.b\n"
    "smax z23.b, p0/M, z23.b, z30.b\n"
    "smax z18.b, p0/M, z18.b, z29.b\n"
    "smax z22.b, p0/M, z22.b, z28.b\n"
    "smax z17.b, p0/M, z17.b, z27.b\n"
    "smax z21.b, p0/M, z21.b, z26.b\n"
    "smax z16.b, p0/M, z16.b, z25.b\n"
    "smax z20.b, p0/M, z20.b, z24.b\n"
    "smax z19.b, p0/M, z19.b, z23.b\n"
    "smax z18.b, p0/M, z18.b, z22.b\n"
    "smax z17.b, p0/M, z17.b, z21.b\n"
    "smax z16.b, p0/M, z16.b, z20.b\n"
    "smax z4.b, p0/M, z4.b, z19.b\n"
    "smax z3.b, p0/M, z3.b, z18.b\n"
    "smax z2.b, p0/M, z2.b, z17.b\n"
    "smax z1.b, p0/M, z1.b, z16.b\n"
    "4:"  // 4-vectors of channels: After loop
    "ands x20, %x[n_valid_cells], #0x3\n"
    "beq 6f\n"
    "5:"  // 4-vectors of channels: Single input loop
    "ldr x23, [x19], #0x8\n"
    "ld1b { z0.b }, p4/Z, [x23, x28]\n"
    "subs x20, x20, #0x1\n"
    "smax z4.b, p0/M, z4.b, z0.b\n"
    "ld1b { z18.b }, p3/Z, [x23, x27]\n"
    "smax z3.b, p0/M, z3.b, z18.b\n"
    "ld1b { z17.b }, p2/Z, [x23, x26]\n"
    "smax z2.b, p0/M, z2.b, z17.b\n"
    "ld1b { z16.b }, p1/Z, [x23, x25]\n"
    "smax z1.b, p0/M, z1.b, z16.b\n"
    "bgt 5b\n"
    "6:"  // 4-vectors of channels: Single input loop: End
    ".inst 0x4508a097  // sshllb z23.h, z4.b, #0x0\n"
    ".inst 0x4508a496  // sshllt z22.h, z4.b, #0x0\n"
    "add x19, %x[quant_params], %[offsetof_qp_per_layer_left_shift]\n"
    "ld1rw { z4.s }, p0/Z, [x19]\n"
    ".inst 0x4508a075  // sshllb z21.h, z3.b, #0x0\n"
    ".inst 0x4508a472  // sshllt z18.h, z3.b, #0x0\n"
    "add x19, %x[quant_params], %[offsetof_qp_per_layer_mul]\n"
    "ld1rw { z3.s }, p0/Z, [x19]\n"
    ".inst 0x4508a054  // sshllb z20.h, z2.b, #0x0\n"
    ".inst 0x4508a451  // sshllt z17.h, z2.b, #0x0\n"
    "add x19, %x[quant_params], %[offsetof_qp_per_layer_right_shift]\n"
    "ld1rw { z2.s }, p0/Z, [x19]\n"
    ".inst 0x4508a033  // sshllb z19.h, z1.b, #0x0\n"
    ".inst 0x4508a430  // sshllt z16.h, z1.b, #0x0\n"
    ".inst 0x4510a2e1  // sshllb z1.s, z23.h, #0x0\n"
    ".inst 0x4510a6f7  // sshllt z23.s, z23.h, #0x0\n"
    ".inst 0x4510a2c0  // sshllb z0.s, z22.h, #0x0\n"
    ".inst 0x4510a6df  // sshllt z31.s, z22.h, #0x0\n"
    ".inst 0x4510a2be  // sshllb z30.s, z21.h, #0x0\n"
    ".inst 0x4510a6b6  // sshllt z22.s, z21.h, #0x0\n"
    ".inst 0x4510a25d  // sshllb z29.s, z18.h, #0x0\n"
    ".inst 0x4510a652  // sshllt z18.s, z18.h, #0x0\n"
    ".inst 0x4510a29c  // sshllb z28.s, z20.h, #0x0\n"
    ".inst 0x4510a695  // sshllt z21.s, z20.h, #0x0\n"
    ".inst 0x4510a23b  // sshllb z27.s, z17.h, #0x0\n"
    ".inst 0x4510a631  // sshllt z17.s, z17.h, #0x0\n"
    ".inst 0x4510a27a  // sshllb z26.s, z19.h, #0x0\n"
    ".inst 0x4510a674  // sshllt z20.s, z19.h, #0x0\n"
    ".inst 0x4510a219  // sshllb z25.s, z16.h, #0x0\n"
    ".inst 0x4510a618  // sshllt z24.s, z16.h, #0x0\n"
    ".inst 0x44828081  // srshl z1.s, p0/M, z1.s, z4.s\n"
    ".inst 0x44828097  // srshl z23.s, p0/M, z23.s, z4.s\n"
    ".inst 0x44828080  // srshl z0.s, p0/M, z0.s, z4.s\n"
    ".inst 0x4482809f  // srshl z31.s, p0/M, z31.s, z4.s\n"
    ".inst 0x4482809e  // srshl z30.s, p0/M, z30.s, z4.s\n"
    ".inst 0x44828096  // srshl z22.s, p0/M, z22.s, z4.s\n"
    ".inst 0x4482809d  // srshl z29.s, p0/M, z29.s, z4.s\n"
    ".inst 0x44828092  // srshl z18.s, p0/M, z18.s, z4.s\n"
    ".inst 0x4482809c  // srshl z28.s, p0/M, z28.s, z4.s\n"
    ".inst 0x44828095  // srshl z21.s, p0/M, z21.s, z4.s\n"
    ".inst 0x4482809b  // srshl z27.s, p0/M, z27.s, z4.s\n"
    ".inst 0x44828091  // srshl z17.s, p0/M, z17.s, z4.s\n"
    ".inst 0x4482809a  // srshl z26.s, p0/M, z26.s, z4.s\n"
    ".inst 0x44828094  // srshl z20.s, p0/M, z20.s, z4.s\n"
    ".inst 0x44828099  // srshl z25.s, p0/M, z25.s, z4.s\n"
    ".inst 0x44828098  // srshl z24.s, p0/M, z24.s, z4.s\n"
    ".inst 0x04a37421  // sqrdmulh z1.s, z1.s, z3.s\n"
    ".inst 0x04a376f7  // sqrdmulh z23.s, z23.s, z3.s\n"
    ".inst 0x04a37400  // sqrdmulh z0.s, z0.s, z3.s\n"
    ".inst 0x04a377ff  // sqrdmulh z31.s, z31.s, z3.s\n"
    ".inst 0x04a377de  // sqrdmulh z30.s, z30.s, z3.s\n"
    ".inst 0x04a376d6  // sqrdmulh z22.s, z22.s, z3.s\n"
    ".inst 0x04a377bd  // sqrdmulh z29.s, z29.s, z3.s\n"
    ".inst 0x04a37652  // sqrdmulh z18.s, z18.s, z3.s\n"
    ".inst 0x04a3779c  // sqrdmulh z28.s, z28.s, z3.s\n"
    ".inst 0x04a376b5  // sqrdmulh z21.s, z21.s, z3.s\n"
    ".inst 0x04a3777b  // sqrdmulh z27.s, z27.s, z3.s\n"
    ".inst 0x04a37631  // sqrdmulh z17.s, z17.s, z3.s\n"
    ".inst 0x04a3775a  // sqrdmulh z26.s, z26.s, z3.s\n"
    ".inst 0x04a37694  // sqrdmulh z20.s, z20.s, z3.s\n"
    ".inst 0x04a37739  // sqrdmulh z25.s, z25.s, z3.s\n"
    ".inst 0x04a37718  // sqrdmulh z24.s, z24.s, z3.s\n"
    "mov z19.s, #0x7f\n"
    ".inst 0x44828041  // srshl z1.s, p0/M, z1.s, z2.s\n"
    ".inst 0x44828057  // srshl z23.s, p0/M, z23.s, z2.s\n"
    ".inst 0x44828040  // srshl z0.s, p0/M, z0.s, z2.s\n"
    ".inst 0x4482805f  // srshl z31.s, p0/M, z31.s, z2.s\n"
    ".inst 0x4482805e  // srshl z30.s, p0/M, z30.s, z2.s\n"
    ".inst 0x44828056  // srshl z22.s, p0/M, z22.s, z2.s\n"
    ".inst 0x4482805d  // srshl z29.s, p0/M, z29.s, z2.s\n"
    ".inst 0x44828052  // srshl z18.s, p0/M, z18.s, z2.s\n"
    ".inst 0x4482805c  // srshl z28.s, p0/M, z28.s, z2.s\n"
    ".inst 0x44828055  // srshl z21.s, p0/M, z21.s, z2.s\n"
    ".inst 0x4482805b  // srshl z27.s, p0/M, z27.s, z2.s\n"
    ".inst 0x44828051  // srshl z17.s, p0/M, z17.s, z2.s\n"
    ".inst 0x4482805a  // srshl z26.s, p0/M, z26.s, z2.s\n"
    ".inst 0x44828054  // srshl z20.s, p0/M, z20.s, z2.s\n"
    ".inst 0x44828059  // srshl z25.s, p0/M, z25.s, z2.s\n"
    ".inst 0x44828058  // srshl z24.s, p0/M, z24.s, z2.s\n"
    "not z16.s, p0/M, z19.s\n"
    "smax z1.s, p0/M, z1.s, z16.s\n"
    "smax z23.s, p0/M, z23.s, z16.s\n"
    "smax z0.s, p0/M, z0.s, z16.s\n"
    "smax z31.s, p0/M, z31.s, z16.s\n"
    "smax z30.s, p0/M, z30.s, z16.s\n"
    "smax z22.s, p0/M, z22.s, z16.s\n"
    "smax z29.s, p0/M, z29.s, z16.s\n"
    "smax z18.s, p0/M, z18.s, z16.s\n"
    "smax z28.s, p0/M, z28.s, z16.s\n"
    "smax z21.s, p0/M, z21.s, z16.s\n"
    "smax z27.s, p0/M, z27.s, z16.s\n"
    "smax z17.s, p0/M, z17.s, z16.s\n"
    "smax z26.s, p0/M, z26.s, z16.s\n"
    "smax z20.s, p0/M, z20.s, z16.s\n"
    "smax z25.s, p0/M, z25.s, z16.s\n"
    "smax z24.s, p0/M, z24.s, z16.s\n"
    "smin z1.s, p0/M, z1.s, z19.s\n"
    "smin z23.s, p0/M, z23.s, z19.s\n"
    "trn1 z23.h, z1.h, z23.h\n"
    "smin z0.s, p0/M, z0.s, z19.s\n"
    "smin z31.s, p0/M, z31.s, z19.s\n"
    "trn1 z16.h, z0.h, z31.h\n"
    "smin z30.s, p0/M, z30.s, z19.s\n"
    "smin z22.s, p0/M, z22.s, z19.s\n"
    "trn1 z22.h, z30.h, z22.h\n"
    "smin z29.s, p0/M, z29.s, z19.s\n"
    "smin z18.s, p0/M, z18.s, z19.s\n"
    "trn1 z18.h, z29.h, z18.h\n"
    "smin z28.s, p0/M, z28.s, z19.s\n"
    "smin z21.s, p0/M, z21.s, z19.s\n"
    "trn1 z21.h, z28.h, z21.h\n"
    "smin z27.s, p0/M, z27.s, z19.s\n"
    "smin z17.s, p0/M, z17.s, z19.s\n"
    "trn1 z17.h, z27.h, z17.h\n"
    "smin z26.s, p0/M, z26.s, z19.s\n"
    "smin z20.s, p0/M, z20.s, z19.s\n"
    "trn1 z20.h, z26.h, z20.h\n"
    "smin z25.s, p0/M, z25.s, z19.s\n"
    "smin z24.s, p0/M, z24.s, z19.s\n"
    "trn1 z19.h, z25.h, z24.h\n"
    "trn1 z16.b, z23.b, z16.b\n"
    "trn1 z18.b, z22.b, z18.b\n"
    "st1b { z16.b }, p4, [%x[outptr], x28]\n"
    "incb x28, ALL, MUL #4\n"
    "trn1 z17.b, z21.b, z17.b\n"
    "trn1 z16.b, z20.b, z19.b\n"
    "st1b { z18.b }, p3, [%x[outptr], x27]\n"
    "incb x27, ALL, MUL #4\n"
    "st1b { z17.b }, p2, [%x[outptr], x26]\n"
    "incb x26, ALL, MUL #4\n"
    "st1b { z16.b }, p1, [%x[outptr], x25]\n"
    "incb x25, ALL, MUL #4\n"
    "whilelt p1.b, x25, %x[n_channels]\n"
    "b.any 1b\n"
    "7:"  // Single vector of channels
    "whilelt p4.b, x28, %x[n_channels]\n"
    "b.none 14f\n"
    "8:"  // Single vector of channels: Loop
    "lsr x24, %x[n_valid_cells], #0x2\n"
    "mov z4.b, #0x80\n"
    "mov x19, %x[inptrs]\n"
    "cbz x24, 11f\n"
    "ldp x23, x22, [x19, #0x0]\n"
    "subs x24, x24, #0x1\n"
    "ld1b { z0.b }, p4/Z, [x23, x28]\n"
    "ldp x21, x20, [x19, #0x10]\n"
    "add x19, x19, #0x20\n"
    "ld1b { z31.b }, p4/Z, [x22, x28]\n"
    "ld1b { z23.b }, p4/Z, [x21, x28]\n"
    "ld1b { z30.b }, p4/Z, [x20, x28]\n"
    "beq 10f\n"
    "9:"  // Single vector of channels: Loop: 4 inputs loop
    "movprfx z19, z0\n smax z19.b, p0/M, z19.b, z31.b\n"
    "smax z23.b, p0/M, z23.b, z30.b\n"
    "ldp x23, x22, [x19, #0x0]\n"
    "subs x24, x24, #0x1\n"
    "smax z19.b, p0/M, z19.b, z23.b\n"
    "ldp x21, x20, [x19, #0x10]\n"
    "smax z4.b, p0/M, z4.b, z19.b\n"
    "add x19, x19, #0x20\n"
    "ld1b { z0.b }, p4/Z, [x23, x28]\n"
    "ld1b { z31.b }, p4/Z, [x22, x28]\n"
    "ld1b { z23.b }, p4/Z, [x21, x28]\n"
    "ld1b { z30.b }, p4/Z, [x20, x28]\n"
    "bgt 9b\n"
    "10:"  // Single vector of channels: Loop: 4 inputs tail
    "movprfx z19, z0\n smax z19.b, p0/M, z19.b, z31.b\n"
    "smax z23.b, p0/M, z23.b, z30.b\n"
    "smax z19.b, p0/M, z19.b, z23.b\n"
    "smax z4.b, p0/M, z4.b, z19.b\n"
    "11:"  // Single vector of channels: Loop: After loop
    "ands x20, %x[n_valid_cells], #0x3\n"
    "beq 13f\n"
    "12:"  // Single vector of channels: Loop: Single input loop
    "ldr x23, [x19], #0x8\n"
    "ld1b { z0.b }, p4/Z, [x23, x28]\n"
    "subs x20, x20, #0x1\n"
    "smax z4.b, p0/M, z4.b, z0.b\n"
    "bgt 12b\n"
    "13:"  // Single vector of channels: Loop: Single input loop: End
    ".inst 0x4508a097  // sshllb z23.h, z4.b, #0x0\n"
    ".inst 0x4508a496  // sshllt z22.h, z4.b, #0x0\n"
    "add x19, %x[quant_params], %[offsetof_qp_per_layer_left_shift]\n"
    "ld1rw { z4.s }, p0/Z, [x19]\n"
    ".inst 0x4510a2e1  // sshllb z1.s, z23.h, #0x0\n"
    ".inst 0x4510a6f7  // sshllt z23.s, z23.h, #0x0\n"
    "add x19, %x[quant_params], %[offsetof_qp_per_layer_mul]\n"
    "ld1rw { z3.s }, p0/Z, [x19]\n"
    ".inst 0x4510a2c0  // sshllb z0.s, z22.h, #0x0\n"
    ".inst 0x4510a6df  // sshllt z31.s, z22.h, #0x0\n"
    "add x19, %x[quant_params], %[offsetof_qp_per_layer_right_shift]\n"
    "ld1rw { z2.s }, p0/Z, [x19]\n"
    ".inst 0x44828081  // srshl z1.s, p0/M, z1.s, z4.s\n"
    ".inst 0x44828097  // srshl z23.s, p0/M, z23.s, z4.s\n"
    ".inst 0x44828080  // srshl z0.s, p0/M, z0.s, z4.s\n"
    ".inst 0x4482809f  // srshl z31.s, p0/M, z31.s, z4.s\n"
    ".inst 0x04a37421  // sqrdmulh z1.s, z1.s, z3.s\n"
    ".inst 0x04a376f7  // sqrdmulh z23.s, z23.s, z3.s\n"
    ".inst 0x04a37400  // sqrdmulh z0.s, z0.s, z3.s\n"
    ".inst 0x04a377ff  // sqrdmulh z31.s, z31.s, z3.s\n"
    "mov z19.s, #0x7f\n"
    ".inst 0x44828041  // srshl z1.s, p0/M, z1.s, z2.s\n"
    ".inst 0x44828057  // srshl z23.s, p0/M, z23.s, z2.s\n"
    ".inst 0x44828040  // srshl z0.s, p0/M, z0.s, z2.s\n"
    ".inst 0x4482805f  // srshl z31.s, p0/M, z31.s, z2.s\n"
    "not z16.s, p0/M, z19.s\n"
    "smax z1.s, p0/M, z1.s, z16.s\n"
    "smax z23.s, p0/M, z23.s, z16.s\n"
    "smax z0.s, p0/M, z0.s, z16.s\n"
    "smax z31.s, p0/M, z31.s, z16.s\n"
    "smin z1.s, p0/M, z1.s, z19.s\n"
    "smin z23.s, p0/M, z23.s, z19.s\n"
    "trn1 z23.h, z1.h, z23.h\n"
    "smin z0.s, p0/M, z0.s, z19.s\n"
    "smin z31.s, p0/M, z31.s, z19.s\n"
    "trn1 z16.h, z0.h, z31.h\n"
    "trn1 z16.b, z23.b, z16.b\n"
    "st1b { z16.b }, p4, [%x[outptr], x28]\n"
    "incb x28\n"
    "whilelt p4.b, x28, %x[n_channels]\n"
    "b.any 8b\n"
    "14:"  // End
    ".inst 0xd503467f  // SMSTOP\n"
    :
    : [inptrs] "r" (inptrs), [n_channels] "r" (n_channels), [n_valid_cells] "r" (n_valid_cells), [offsetof_qp_per_layer_left_shift] "I" (offsetof(Requantize32, per_layer_left_shift)), [offsetof_qp_per_layer_mul] "I" (offsetof(Requantize32, per_layer_mul)), [offsetof_qp_per_layer_right_shift] "I" (offsetof(Requantize32, per_layer_right_shift)), [outptr] "r" (outptr), [quant_params] "r" (&qp)
    : "cc", "memory", "p0", "p1", "p2", "p3", "p4", "p5", "p6", "p7", "p8", "p9", "p10", "p11", "p12", "p13", "p14", "p15", "x19", "x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "z0", "z1", "z2", "z3", "z4", "z5", "z6", "z7", "z8", "z9", "z10", "z11", "z12", "z13", "z14", "z15", "z16", "z17", "z18", "z19", "z20", "z21", "z22", "z23", "z24", "z25", "z26", "z27", "z28", "z29", "z30", "z31"
  );
}

}  // namespace pooling
}  // namespace arm_conv

#endif  // defined(ARM_COMPUTE_ENABLE_SME)
