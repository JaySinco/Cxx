#include "string_helper.h"
#include <codecvt>
#include <locale>

namespace cxx {

std::string encodeUtf8(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(str);
}

std::wstring decodeUtf8(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(str);
}

} // namespace cxx