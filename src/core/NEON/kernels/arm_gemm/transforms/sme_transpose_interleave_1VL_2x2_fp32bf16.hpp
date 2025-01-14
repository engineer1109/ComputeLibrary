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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#ifdef __ARM_FEATURE_SVE


namespace {

void sme_transpose_interleave_1VL_2x2_fp32bf16(bfloat16 *out, const float *in, size_t width, size_t in_stride, size_t height)
{
    float *pad_row = reinterpret_cast<float *>(alloca(width * sizeof(float)));

    if (height % 2) {
        memset(pad_row, 0, width * sizeof(float));
    }

    size_t out_stride = 1 * roundup<size_t>(height, 2) * sme::get_vector_length<uint16_t>();

    __asm__ __volatile__(
      ".inst 0xd503477f  // SMSTART ZA\n"
      "cmp %x[height], #0x4\n"
      "ptrue p1.b\n"
      "blt 6f\n"
      "1:"  // Main row loop: Head
      "mov x25, %x[in]\n"
      "add x24, x25, %x[in_stride]\n"
      "add x23, x24, %x[in_stride]\n"
      "mov x22, %x[width]\n"
      "cnth x20, ALL, MUL #2\n"
      "add x19, x23, %x[in_stride]\n"
      "cmp x22, x20\n"
      "add %x[in], x19, %x[in_stride]\n"
      "mov x21, %x[out]\n"
      "sub %x[height], %x[height], #0x4\n"
      "blt 3f\n"
      "2:"  // Main row loop: Unroll column loop
      "ld1w { z16.s }, p1/Z, [x25]\n"
      ".inst 0x658aa618  // bfcvt z24.h, p1/M, z16.s\n"
      "sub x22, x22, x20\n"
      "cmp x22, x20\n"
      "ld1w { z16.s }, p1/Z, [x23]\n"
      ".inst 0x658aa617  // bfcvt z23.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x25, #1, MUL VL]\n"
      ".inst 0x658aa616  // bfcvt z22.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x23, #1, MUL VL]\n"
      ".inst 0x658aa615  // bfcvt z21.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x25, #2, MUL VL]\n"
      ".inst 0x658aa614  // bfcvt z20.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x23, #2, MUL VL]\n"
      ".inst 0x658aa613  // bfcvt z19.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x25, #3, MUL VL]\n"
      ".inst 0x658aa612  // bfcvt z18.h, p1/M, z16.s\n"
      "addvl x25, x25, #4\n"
      "ld1w { z16.s }, p1/Z, [x23, #3, MUL VL]\n"
      ".inst 0x658aa611  // bfcvt z17.h, p1/M, z16.s\n"
      "addvl x23, x23, #4\n"
      "ld1w { z16.s }, p1/Z, [x24]\n"
      ".inst 0x648aa618  // bfcvtnt z24.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x19]\n"
      ".inst 0x648aa617  // bfcvtnt z23.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x24, #1, MUL VL]\n"
      ".inst 0x648aa616  // bfcvtnt z22.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x19, #1, MUL VL]\n"
      ".inst 0x648aa615  // bfcvtnt z21.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x24, #2, MUL VL]\n"
      ".inst 0x648aa614  // bfcvtnt z20.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x19, #2, MUL VL]\n"
      ".inst 0x648aa613  // bfcvtnt z19.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x24, #3, MUL VL]\n"
      "addvl x24, x24, #4\n"
      ".inst 0x648aa612  // bfcvtnt z18.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x19, #3, MUL VL]\n"
      "st1h { z24.h }, p1, [x21]\n"
      "addvl x19, x19, #4\n"
      ".inst 0x648aa611  // bfcvtnt z17.h, p1/M, z16.s\n"
      "st1h { z23.h }, p1, [x21, #1, MUL VL]\n"
      "add x21, x21, %x[out_stride]\n"
      "st1h { z22.h }, p1, [x21]\n"
      "st1h { z21.h }, p1, [x21, #1, MUL VL]\n"
      "add x21, x21, %x[out_stride]\n"
      "st1h { z20.h }, p1, [x21]\n"
      "st1h { z19.h }, p1, [x21, #1, MUL VL]\n"
      "add x21, x21, %x[out_stride]\n"
      "st1h { z18.h }, p1, [x21]\n"
      "st1h { z17.h }, p1, [x21, #1, MUL VL]\n"
      "add x21, x21, %x[out_stride]\n"
      "bge 2b\n"
      "3:"  // Main row loop: Unroll column loop skip
      "cbz x22, 5f\n"
      "4:"  // Main row loop: Column loop
      "whilelt p0.s, XZR, x22\n"
      "ld1w { z16.s }, p0/Z, [x25]\n"
      ".inst 0x658aa612  // bfcvt z18.h, p1/M, z16.s\n"
      "decw x22\n"
      "ld1w { z16.s }, p0/Z, [x23]\n"
      ".inst 0x658aa611  // bfcvt z17.h, p1/M, z16.s\n"
      "cmp x22, #0x0\n"
      "addvl x25, x25, #1\n"
      "ld1w { z16.s }, p0/Z, [x24]\n"
      "addvl x24, x24, #1\n"
      "addvl x23, x23, #1\n"
      ".inst 0x648aa612  // bfcvtnt z18.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p0/Z, [x19]\n"
      "addvl x19, x19, #1\n"
      ".inst 0x648aa611  // bfcvtnt z17.h, p1/M, z16.s\n"
      "st1h { z18.h }, p1, [x21]\n"
      "st1h { z17.h }, p1, [x21, #1, MUL VL]\n"
      "add x21, x21, %x[out_stride]\n"
      "bgt 4b\n"
      "5:"  // Main row loop: Column loop skip
      "cmp %x[height], #0x4\n"
      "addvl %x[out], %x[out], #2\n"
      "bge 1b\n"
      "cbz %x[height], 12f\n"
      "6:"  // Main loop skip
      "7:"  // Tail row loop: Head
      "mov x25, %x[in]\n"
      "add x24, x25, %x[in_stride]\n"
      "cmp %x[height], #0x1\n"
      "mov x20, %x[width]\n"
      "cnth x19, ALL, MUL #2\n"
      "add %x[in], x24, %x[in_stride]\n"
      "csel x24, x24, %x[pad_row], GT\n"
      "cmp x20, x19\n"
      "mov x21, %x[out]\n"
      "sub %x[height], %x[height], #0x2\n"
      "blt 9f\n"
      "8:"  // Tail row loop: Unroll column loop
      "ld1w { z16.s }, p1/Z, [x25]\n"
      ".inst 0x658aa614  // bfcvt z20.h, p1/M, z16.s\n"
      "sub x20, x20, x19\n"
      "cmp x20, x19\n"
      "ld1w { z16.s }, p1/Z, [x25, #1, MUL VL]\n"
      ".inst 0x658aa613  // bfcvt z19.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x25, #2, MUL VL]\n"
      ".inst 0x658aa612  // bfcvt z18.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x25, #3, MUL VL]\n"
      ".inst 0x658aa611  // bfcvt z17.h, p1/M, z16.s\n"
      "addvl x25, x25, #4\n"
      "ld1w { z16.s }, p1/Z, [x24]\n"
      ".inst 0x648aa614  // bfcvtnt z20.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x24, #1, MUL VL]\n"
      ".inst 0x648aa613  // bfcvtnt z19.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x24, #2, MUL VL]\n"
      ".inst 0x648aa612  // bfcvtnt z18.h, p1/M, z16.s\n"
      "ld1w { z16.s }, p1/Z, [x24, #3, MUL VL]\n"
      "st1h { z20.h }, p1, [x21]\n"
      "add x21, x21, %x[out_stride]\n"
      "addvl x24, x24, #4\n"
      "st1h { z19.h }, p1, [x21]\n"
      "add x21, x21, %x[out_stride]\n"
      ".inst 0x648aa611  // bfcvtnt z17.h, p1/M, z16.s\n"
      "st1h { z18.h }, p1, [x21]\n"
      "add x21, x21, %x[out_stride]\n"
      "st1h { z17.h }, p1, [x21]\n"
      "add x21, x21, %x[out_stride]\n"
      "bge 8b\n"
      "9:"  // Tail row loop: Unroll column loop skip
      "cbz x20, 11f\n"
      "10:"  // Tail row loop: Column loop
      "whilelt p0.s, XZR, x20\n"
      "ld1w { z16.s }, p0/Z, [x25]\n"
      ".inst 0x658aa611  // bfcvt z17.h, p1/M, z16.s\n"
      "decw x20\n"
      "ld1w { z16.s }, p0/Z, [x24]\n"
      "cmp x20, #0x0\n"
      "addvl x25, x25, #1\n"
      ".inst 0x648aa611  // bfcvtnt z17.h, p1/M, z16.s\n"
      "addvl x24, x24, #1\n"
      "st1h { z17.h }, p1, [x21]\n"
      "add x21, x21, %x[out_stride]\n"
      "bgt 10b\n"
      "11:"  // Tail row loop: Column loop skip
      "cmp %x[height], #0x1\n"
      "addvl %x[out], %x[out], #1\n"
      "bge 7b\n"
      "12:"  // Done
      ".inst 0xd503467f  // SMSTOP\n"
      : [height] "+&r" (height), [in] "+&r" (in), [out] "+&r" (out)
      : [in_stride] "r" (in_stride), [out_stride] "r" (out_stride), [pad_row] "r" (pad_row), [width] "r" (width)
      : "cc", "memory", "p0", "p1", "p2", "p3", "p4", "p5", "p6", "p7", "p8", "p9", "p10", "p11", "p12", "p13", "p14", "p15", "x19", "x20", "x21", "x22", "x23", "x24", "x25", "z0", "z1", "z2", "z3", "z4", "z5", "z6", "z7", "z8", "z9", "z10", "z11", "z12", "z13", "z14", "z15", "z16", "z17", "z18", "z19", "z20", "z21", "z22", "z23", "z24", "z25", "z26", "z27", "z28", "z29", "z30", "z31"
    );
}

} // anonymous namespace
template<>
void Transform<1, 2, true, VLType::SME>(
    bfloat16 *out, const float *in, int stride, int x0, int xmax, int k0, int kmax)
{
    sme_transpose_interleave_1VL_2x2_fp32bf16(
        out,
        in + k0 * stride + x0,
        (xmax-x0),
        stride * sizeof(float),
        (kmax-k0)
    );
}

#endif
