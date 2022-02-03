/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2020-2022 Intel Corporation
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

typedef struct _st_container st_container;

struct _st_container
{
	__global int *value;
	__global st_container *next;
};

kernel void indirectAccess(__global st_container *container) {
	int value_to_write;
	for(value_to_write = 1; container->next; ++value_to_write, container=container->next){}
	*container->value = value_to_write;
}
