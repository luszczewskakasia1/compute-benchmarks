#pragma once

#include "framework/configuration.h"
#include "framework/ocl/cl.h"

#include <CL/intel/embargo/cl_ext_private_internal.h>

struct QueueProperties {
    cl_command_queue_properties properties[5] = {CL_QUEUE_PROPERTIES, 0, 0, 0, 0};

    operator const cl_command_queue_properties *() const {
        return &properties[0];
    }

    static QueueProperties create() {
        return create(false, false, -1);
    }

    static QueueProperties createProfilingOrNot(bool profiling) {
        return create(profiling, false, -1);
    }

    static QueueProperties createBcsOrNot(bool bcs) {
        return create(false, bcs, -1);
    }

    static QueueProperties createOoqOrNot(bool ooqArg) {
        const int ooq = ooqArg ? 1 : 0;
        return create(false, false, ooq);
    }

    static QueueProperties create(bool profiling, bool bcs, int ooq) {
        QueueProperties result{};
        if (profiling) {
            result.properties[1] |= CL_QUEUE_PROFILING_ENABLE;
        }
        if (bcs) {
            result.properties[2] = CL_QUEUE_FAMILY_INTEL;
            result.properties[3] = CL_QUEUE_FAMILY_TYPE_BCS_INTEL;
        }
        if (ooq == 1 || (ooq == -1 && ::configuration.oclUseOOQ)) {
            result.properties[1] |= CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
        }

        return result;
    }
};
