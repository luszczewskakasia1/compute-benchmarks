#include "framework/utility/error.h"
#include "framework/utility/linux/error.h"
#include "framework/utility/working_directory_helper.h"

#include <unistd.h>

std::filesystem::path WorkingDirectoryHelper::getExeLocation() {
    char buffer[4096];
    FATAL_ERROR_IF_SYS_CALL_FAILED(readlink("/proc/self/exe", buffer, sizeof(buffer)), "retrieving .exe location");
    return std::filesystem::path{buffer};
}
