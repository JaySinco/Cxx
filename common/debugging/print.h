#pragma once
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <pprint/pprint.hpp>

template <typename T>
void PRINT(T &&_Arg)
{
    std::ostringstream ss;
    pprint::PrettyPrinter printer(ss);
    printer.print(std::forward<T>(_Arg));
    LOG(INFO) << ss.str();
}