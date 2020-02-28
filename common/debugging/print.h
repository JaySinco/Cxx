#pragma once
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <gflags/gflags.h>
#ifdef CXX_STANDARD_GTE_17
// #pragma clang diagnostic push
// #pragma clang diagnostic ignored "-Wreorder"
#include <pprint/pprint.hpp>
// #pragma clang diagnostic pop
#endif

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

#ifdef CXX_STANDARD_GTE_17
template <typename T>
std::string TOSTR(T &&Arg)
{
    std::ostringstream ss;
    pprint::PrettyPrinter printer(ss);
    printer.print(std::forward<T>(Arg));
    return ss.str();
}
#endif
