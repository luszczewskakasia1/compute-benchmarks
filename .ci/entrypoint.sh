#!/bin/bash

#
# Copyright (C) 2022 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

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
