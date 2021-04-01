#pragma once

#include "framework/utility/error.h"

#include <string>

enum class Engine {
    Unknown,
    Rcs,

    Ccs0,
    Ccs1,
    Ccs2,
    Ccs3,

    Bcs,

    LinkBcs0,
    LinkBcs1,
    LinkBcs2,
    LinkBcs3,
    LinkBcs4,
    LinkBcs5,
    LinkBcs6,
    LinkBcs7,
};

enum class EngineGroup {
    Unknown,
    RenderCompute,
    Compute,
    Copy,
    LinkCopy,
};

struct EngineHelper {
    static EngineGroup parseEngineGroup(const std::string &name) {
        if (name == "rcs" || name == "cccs") {
            return EngineGroup::RenderCompute;
        }
        if (name == "ccs") {
            return EngineGroup::Compute;
        }
        if (name == "bcs") {
            return EngineGroup::Copy;
        }
        if (name == "linked bcs") {
            return EngineGroup::LinkCopy;
        }
        return EngineGroup::Unknown;
    }

    static EngineGroup getEngineGroup(Engine engine) {
        switch (engine) {
        case Engine::Rcs:
            return EngineGroup::RenderCompute;
        case Engine::Ccs0:
        case Engine::Ccs1:
        case Engine::Ccs2:
        case Engine::Ccs3:
            return EngineGroup::Compute;
        case Engine::Bcs:
            return EngineGroup::Copy;
        case Engine::LinkBcs0:
        case Engine::LinkBcs1:
        case Engine::LinkBcs2:
        case Engine::LinkBcs3:
        case Engine::LinkBcs4:
        case Engine::LinkBcs5:
        case Engine::LinkBcs6:
        case Engine::LinkBcs7:
            return EngineGroup::LinkCopy;
        default:
            FATAL_ERROR("Unknown engine");
        }
    }

    static size_t getEngineIndexWithinGroup(Engine engine) {
        switch (engine) {
        case Engine::Rcs:
            return 0;
        case Engine::Ccs0:
        case Engine::Ccs1:
        case Engine::Ccs2:
        case Engine::Ccs3:
            return static_cast<size_t>(engine) - static_cast<size_t>(Engine::Ccs0);
        case Engine::Bcs:
            return 0;
        case Engine::LinkBcs0:
        case Engine::LinkBcs1:
        case Engine::LinkBcs2:
        case Engine::LinkBcs3:
        case Engine::LinkBcs4:
        case Engine::LinkBcs5:
        case Engine::LinkBcs6:
        case Engine::LinkBcs7:
            return static_cast<size_t>(engine) - static_cast<size_t>(Engine::LinkBcs0);
        default:
            FATAL_ERROR("Unknown engine");
        }
    }
};
