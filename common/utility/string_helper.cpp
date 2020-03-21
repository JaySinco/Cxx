#include "string_helper.h"
#include "base.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace cxx {

static std::string encode(const std::wstring& str, unsigned codePage)
{
    int len = WideCharToMultiByte(codePage, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
    auto buf = new char[len] {};
    if (0 == WideCharToMultiByte(codePage, 0, str.c_str(), -1, buf, len, NULL, NULL)) {
        LOG_LAST_WIN_ERROR("failed to encode wstring");
    }
    auto ret = std::string(buf);
    delete[] buf;
    return ret;
}

static std::wstring decode(const std::string& str, unsigned codePage)
{
    int len = MultiByteToWideChar(codePage, 0, str.c_str(), -1, NULL, 0);
    auto buf = new wchar_t[len] {};
    if (0 == MultiByteToWideChar(codePage, 0, str.c_str(), -1, buf, len)) {
        LOG_LAST_WIN_ERROR("failed to decode string");
    }
    auto ret = std::wstring(buf);
    delete[] buf;
    return ret;
}

std::string encodeAnsi(const std::wstring& str)
{
    return encode(str, CP_ACP);
}

std::wstring decodeUtf8(const std::string& str)
{
    return decode(str, CP_UTF8);
}

} // namespace cxx