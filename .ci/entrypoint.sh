#!/bin/bash

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

# If user_id/group_id are present as an environmental variables
# Then we use them to spawn a runner account within docker
# If not supplied, just run the command


if [ -n "$user_id" ]; then
  echo "User ID, with value of [$user_id] has been supplied"

  if [ -n "$group_id" ]; then
    echo "Group ID, with value of [$group_id] has been supplied."
    echo "Creating runner user and using it to execute command."

    groupadd runner -g "$group_id"
    useradd runner -u "$user_id" -g "$group_id" -s /bin/bash
    su runner -c "$INPUT_COMMAND"
  else
    echo "No group_id supplied, triggering command using default user."
    /bin/bash -c "$INPUT_COMMAND"
  fi
else
  echo "No user_id supplied, triggering command using default user."
  /bin/bash -c "$INPUT_COMMAND"
fi
