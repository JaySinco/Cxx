#include "base.h"

namespace cxx {

std::string getResAbsPath(
    const std::string& product,
    const std::string& fileName)
{
    auto resAbsPath = std::string(ROOT_DIR).substr(1) + "/external/resources/"
        + product + "/" + fileName;
    resAbsPath.replace(1, 1, ":\\");
    for (auto& c : resAbsPath) {
        if (c == '/')
            c = '\\';
    }
    return resAbsPath;
}

} // namespace cxx