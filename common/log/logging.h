#pragma once
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>

#define CHECK_RTN_STATUS(ok) \
    if (!ok)                 \
    {                        \
        return false;        \
    }

#define CHECK_RTN_WITH_MSG(ok, msg) \
    if (!ok)                        \
    {                               \
        LOG(ERROR) << msg;          \
        return false;               \
    }