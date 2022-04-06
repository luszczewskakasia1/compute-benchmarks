#
# Copyright (C) 2022 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

root_path=".."
excluded_paths="$root_path/third_party|$root_path/.git|$root_path/build|$root_path/scripts"

echo "------------ Processing C++ files"
for file in $(find "$root_path" -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.inl" \) | grep -vE "$excluded_paths"); do
    echo "Processing $file..."

    tmp_file="tmp_file"
    cat > $tmp_file <<- END_OF_HEADER
/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021 Intel Corporation
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express or
 * implied warranties, other than those that are expressly stated in the License.
 */

END_OF_HEADER

    cat $file >> $tmp_file
    mv $tmp_file $file
done

echo "------------ Processing Bash/CMake/Docker files"
for file in $(find "$root_path" -type f \( -name "*.sh" -o -name "*.cmake" -o -name "*CMakeLists.txt" -o -name "Dockerfile" \) | grep -vE "$excluded_paths"); do
    echo "Processing $file..."

    tmp_file="tmp_file"
    cat > $tmp_file <<- END_OF_HEADER
#
# INTEL CONFIDENTIAL
#
# Copyright (C) 2019-2021 Intel Corporation
#
# This software and the related documents are Intel copyrighted materials,
# and your use of them is governed by the express license under which they were
# provided to you ("License"). Unless the License provides otherwise,
# you may not use, modify, copy, publish, distribute, disclose or transmit this
# software or the related documents without Intel's prior written permission.
#
# This software and the related documents are provided as is, with no express or
# implied warranties, other than those that are expressly stated in the License.
#

END_OF_HEADER

    cat $file >> $tmp_file
    mv $tmp_file $file
done
