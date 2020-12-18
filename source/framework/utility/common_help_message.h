#pragma once

#include <string>

struct CommonHelpMessage {
    static std::string compression(const char *target);
    static std::string forceBlitter();
    static std::string useEvents();
    static std::string measuredCommandsCount();
};