#=========================== begin_copyright_notice ============================
#
# INTEL CONFIDENTIAL
#
# Copyright (C) 2020-2021 Intel Corporation
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
#============================ end_copyright_notice =============================

FROM dockerv2-gfx-build.gfx-assets.intel.com/upstream/ubuntu:20.04
RUN cat /etc/os-release

ENV PATH="/:$PATH"

# Install OCL and L0 Loaders
RUN apt-get update && apt-get install software-properties-common -y
RUN add-apt-repository ppa:intel-opencl/intel-opencl -y
RUN apt-get update && apt-get install libze-loader ocl-icd-opencl-dev -y

# Install utility applications
RUN apt-get update && apt-get install -y \
      git                                \
      cmake                              \
      gcc                                \
      g++                                \
      clang-format

# Setup entrypoint
COPY ./entrypoint.sh /
ENTRYPOINT ["/entrypoint.sh"]
