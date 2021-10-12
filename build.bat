REM
REM INTEL CONFIDENTIAL
REM
REM Copyright (C) 2019-2021 Intel Corporation
REM
REM This software and the related documents are Intel copyrighted materials,
REM and your use of them is governed by the express license under which they were
REM provided to you ("License"). Unless the License provides otherwise,
REM you may not use, modify, copy, publish, distribute, disclose or transmit this
REM software or the related documents without Intel's prior written permission.
REM
REM This software and the related documents are provided as is, with no express or
REM implied warranties, other than those that are expressly stated in the License.
REM

mkdir build
cd build
cmake .. %*
cd ..
