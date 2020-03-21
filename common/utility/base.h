#pragma once
#include <string>
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <gflags/gflags.h>
#include <glog/logging.h>
#if __cplusplus >= 201703L
#include <pprint/pprint.hpp>
#endif
#define TRY_BEGIN \
    try {
#define CATCH_ALL                                            \
    }                                                        \
    catch (const std::exception& e)                          \
    {                                                        \
        LOG(ERROR) << "uncaught exception <"                 \
                   << typeid(e).name() << ">: " << e.what(); \
        std::exit(0);                                        \
    }                                                        \
    catch (...)                                              \
    {                                                        \
        LOG(ERROR) << "uncaught exception <unknow type>";    \
        std::exit(0);                                        \
    }
#include <.root>
#ifdef _WIN32
#define LOG_LAST_WIN_ERROR(msg) LOG(ERROR) << msg << ", errno=" << GetLastError()
#endif

namespace cxx {

#if __cplusplus >= 201703L
template <typename T>
std::string tostr(T&& Arg)
{
    std::ostringstream ss;
    pprint::PrettyPrinter printer(ss);
    printer.print(std::forward<T>(Arg));
    return ss.str();
}
#endif

inline std::string getResAbsPath(
    const std::string& product,
    const std::string& fileName)
{
    return std::string(ROOT_DIR) + "\\" + product + "\\" + fileName;
}

} // namespace cxx