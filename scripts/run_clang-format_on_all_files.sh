#
# INTEL CONFIDENTIAL
#
# Copyright (C) 2022 Intel Corporation
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

if [ `dpkg --list | grep " clang-format " | wc -l` -eq 0 ]; then
    sudo apt-get install clang-format
fi

root_dir=`echo ${BASH_SOURCE[0]} | xargs realpath | xargs dirname | xargs dirname `
find "$root_dir/source" | grep -E "*\.(cpp|inl|h|c|hpp)$" | xargs clang-format -i --verbose
