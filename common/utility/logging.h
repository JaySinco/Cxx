#pragma once
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <gflags/gflags.h>
#define TRY_BEGIN \
    try           \
    {
#define CATCH_ALL                                            \
    }                                                        \
    catch (const std::exception &e)                          \
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
#if __cplusplus >= 201703L
#include <pprint/pprint.hpp>
namespace cxx
{
template <typename T>
std::string tostr(T &&Arg)
{
    std::ostringstream ss;
    pprint::PrettyPrinter printer(ss);
    printer.print(std::forward<T>(Arg));
    return ss.str();
}
} // namespace cxx
#endif