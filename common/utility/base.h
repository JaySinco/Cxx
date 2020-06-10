#pragma once
#include <string>
#ifndef NOT_INCLUDE_GLOG
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <gflags/gflags.h>
#include <glog/logging.h>
#endif

namespace cxx {

template <typename T>
constexpr int arrLen(T& array)
{
    return (sizeof(array) / sizeof(array[0]));
}

std::string getResAbsPath(const std::string& product, const std::string& fileName);

} // namespace cxx