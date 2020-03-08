#pragma once
#include <string>

namespace cxx {

std::string encodeAnsi(const std::wstring& str);
std::wstring decodeUtf8(const std::string& str);

} // namespace cxx