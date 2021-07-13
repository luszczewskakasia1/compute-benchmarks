#pragma once

#include <filesystem>
#include <string>

struct WorkingDirectoryHelper {
    static std::filesystem::path getExeLocation(); // OS-specific implementation

    static void changeDirectory(const std::filesystem::path &directory) {
        std::filesystem::current_path(directory);
    }

    static void changeDirectoryToExeDirectory() {
        const std::filesystem::path exeLocation = getExeLocation();
        const std::filesystem::path exeDirectory = exeLocation.parent_path();
        changeDirectory(exeDirectory);
    }
};
