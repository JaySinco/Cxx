#pragma once
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <gflags/gflags.h>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreorder"
#include <pprint/pprint.hpp>
#pragma clang diagnostic pop

#define TRY_BEGIN \
    try           \
    {
#define CATCH_ALL                                                \
    }                                                            \
    catch (...)                                                  \
    {                                                            \
        try                                                      \
        {                                                        \
            if (auto eptr = std::current_exception())            \
            {                                                    \
                std::rethrow_exception(eptr);                    \
            }                                                    \
        }                                                        \
        catch (const std::exception &e)                          \
        {                                                        \
            LOG(ERROR) << "uncaught exception <"                 \
                       << typeid(e).name() << ">: " << e.what(); \
        }                                                        \
        catch (...)                                              \
        {                                                        \
            LOG(ERROR) << "uncaught exception <unknow type>";    \
        }                                                        \
    }

template <typename T>
std::string TOSTR(T &&Arg)
{
    std::ostringstream ss;
    pprint::PrettyPrinter printer(ss);
    printer.print(std::forward<T>(Arg));
    return ss.str();
}
