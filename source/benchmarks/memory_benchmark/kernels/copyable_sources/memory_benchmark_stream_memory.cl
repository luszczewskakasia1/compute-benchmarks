/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2021 Intel Corporation. All Rights Reserved.
 *
 * The source code contained or described herein and all documents related to the
 * source code ("Material") are owned by Intel Corporation or its suppliers
 * or licensors. Title to the Material remains with Intel Corporation or its
 * suppliers and licensors. The Material contains trade secrets and proprietary
 * and confidential information of Intel or its suppliers and licensors. The
 * Material is protected by worldwide copyright and trade secret laws and
 * treaty provisions. No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Intel in writing.
 */

// #pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void read(const __global STREAM_TYPE *restrict x, __global STREAM_TYPE *restrict dummyOutput, STREAM_TYPE scalar) {
    const int i = get_global_id(0);
    STREAM_TYPE value = x[i];

    // A trick to ensure compiler won't optimize away the read
    if (value == 0.37221) {
        *dummyOutput = value;
    }
}

__kernel void write(__global STREAM_TYPE *restrict x, STREAM_TYPE scalar) {
    const int i = get_global_id(0);
    x[i] = scalar;
}

__kernel void scale(const __global STREAM_TYPE *restrict x, __global STREAM_TYPE *restrict y, STREAM_TYPE scalar) {
    const int i = get_global_id(0);
    y[i] = x[i] * scalar;
}

__kernel void triad(const __global STREAM_TYPE *restrict x, const __global STREAM_TYPE *restrict y,
                    __global STREAM_TYPE *restrict z, STREAM_TYPE scalar) {
    const int i = get_global_id(0);
    z[i] = x[i] + y[i] * scalar;
}

__kernel void stream_3bytesRGBtoY(const __global uchar *restrict x, __global uchar *restrict luminance, float scalar) {
    const int i = get_global_id(0) * 3;
    const int y = get_global_id(0);
    luminance[y] = (uchar)( (x[i]*0.2126f) + (x[i+1]*0.7152f) + (x[i+2]*0.0722f) );
}

__kernel void stream_3BytesAlignedRGBtoY(const __global uchar4 *restrict x, __global uchar *restrict luminance, float scalar) {
    const int i = get_global_id(0);
    luminance[i] = (uchar)( (x[i].x *0.2126f) + (x[i].y * 0.7152f) + (x[i].z * 0.0722f) );
}


