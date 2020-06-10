#include ".root"
#include "base.h"

namespace cxx {

std::string getResAbsPath(
    const std::string& product,
    const std::string& fileName)
{
    return std::string(ROOT_DIR) + "/external/resources/" + product + "/" + fileName;
}

} // namespace cxx