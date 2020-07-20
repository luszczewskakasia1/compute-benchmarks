/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2019 - 2020 Intel Corporation. All Rights Reserved.
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

#ifndef _ZEX_CMDGRAPH_H
#define _ZEX_CMDGRAPH_H
#if defined(__cplusplus)
#pragma once
#endif
#include "zex_common.h"

#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
/// @brief Creates a command graph on the device for submitting commands to any
///        command queue.
///
/// @details
///     - The application may call this function from simultaneous threads.
///     - The implementation of this function should be lock-free.
///
/// @returns
///     - ::ZE_RESULT_SUCCESS
///     - ::ZE_RESULT_ERROR_UNINITIALIZED
///     - ::ZE_RESULT_ERROR_DEVICE_LOST
///     - ::ZE_RESULT_ERROR_INVALID_ARGUMENT
///         + nullptr == hDevice
///         + nullptr == desc
///         + nullptr == phCommandGraph
///     - ::ZE_RESULT_ERROR_UNKNOWN
///         + ::ZEX_COMMAND_GRAPH_DESC_VERSION_CURRENT < desc->version
///     - ::ZE_RESULT_ERROR_OUT_OF_HOST_MEMORY
///     - ::ZE_RESULT_ERROR_OUT_OF_DEVICE_MEMORY
__zedllexport ze_result_t __zecall
zexCommandGraphCreate(
    ze_device_handle_t hDevice,                ///< [in] handle of the device object
    const ze_command_list_desc_t *desc,        ///< [in] pointer to command graph descriptor
    zex_command_graph_handle_t *phCommandGraph ///< [out] pointer to handle of command graph object created
);

///////////////////////////////////////////////////////////////////////////////
/// @brief Destroys a command graph.
///
/// @details
///     - The implementation of this function will immediately free all Host
///       allocations associated with this command graph.
///     - The application may **not** call this function from simultaneous
///       threads with the same command graph handle.
///     - The implementation of this function should be lock-free.
///
/// @returns
///     - ::ZE_RESULT_SUCCESS
///     - ::ZE_RESULT_ERROR_UNINITIALIZED
///     - ::ZE_RESULT_ERROR_DEVICE_LOST
///     - ::ZE_RESULT_ERROR_INVALID_ARGUMENT
///         + nullptr == hCommandGraph
///     - ::ZE_RESULT_ERROR_UNKNOWN
__zedllexport ze_result_t __zecall
zexCommandGraphDestroy(
    zex_command_graph_handle_t hCommandGraph ///< [in][release] handle of command graph object to destroy
);

///////////////////////////////////////////////////////////////////////////////
/// @brief Closes a command graph; ready to be executed by a command queue.
///
/// @details
///     - The command graph will optimize the execution order of the command
///       lists.
///     - A command list may **not** be reset after the command graph is closed.
///     - The application may **not** call this function from simultaneous
///       threads with the same command graph handle.
///     - The implementation of this function should be lock-free.
///
/// @returns
///     - ::ZE_RESULT_SUCCESS
///     - ::ZE_RESULT_ERROR_UNINITIALIZED
///     - ::ZE_RESULT_ERROR_DEVICE_LOST
///     - ::ZE_RESULT_ERROR_INVALID_ARGUMENT
///         + nullptr == hCommandGraph
///     - ::ZE_RESULT_ERROR_UNKNOWN
__zedllexport ze_result_t __zecall
zexCommandGraphClose(
    zex_command_graph_handle_t hCommandGraph ///< [in] handle of command graph object to close
);

__zedllexport ze_result_t __zecall
zexCommandGraphCreateNode(
    zex_command_graph_handle_t hCommandGraph,
    zex_command_graph_handle_t *phCommandNode,
    zex_command_graph_handle_t *phParentNodes,
    size_t noParentNodes,
    COMMANDGRAPH_TYPE nodeType);
__zedllexport ze_result_t __zecall
zexCommandGraphCreateLoadRegImemNode(
    zex_command_graph_handle_t hCommandGraph,
    zex_command_graph_handle_t *phCommandNode,
    zex_command_graph_handle_t *phParentNodes,
    size_t noParentNodes,
    ALU_REG regDestination,
    ALU_REG regSourceAddress);
__zedllexport ze_result_t __zecall
zexCommandGraphCreateStoreRegImemNode(
    zex_command_graph_handle_t hCommandGraph,
    zex_command_graph_handle_t *phCommandNode,
    zex_command_graph_handle_t *phParentNodes,
    size_t noParentNodes,
    ALU_REG regDestinationAddress,
    ALU_REG regSource);
__zedllexport ze_result_t __zecall
zexCommandGraphNodeAddChildren(
    zex_command_graph_handle_t hCommandNode,
    zex_command_graph_handle_t *phChildrenNodes,
    size_t noChildrenNodes);
__zedllexport ze_result_t __zecall
zexCommandGraphOpenNode(
    zex_command_graph_handle_t hCommandNode,
    zex_command_list_handle_t *phCommandList);
__zedllexport ze_result_t __zecall
zexCommandGraphCloseNode(
    zex_command_graph_handle_t hCommandNode);
#if defined(__cplusplus)
} // extern "C"
#endif

#endif // _ZEX_CMDGRAPH_H
