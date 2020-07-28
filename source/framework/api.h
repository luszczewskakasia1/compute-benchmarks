#pragma once

enum class Api {
    OpenCL,
    L0,

    COUNT,
    Default = OpenCL,
    All = 0xffff,
};
