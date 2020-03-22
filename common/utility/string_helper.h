#pragma once
#include <string>

namespace cxx {

std::string encodeAnsi(const std::wstring& str);
std::string encodeUtf8(const std::wstring& str);
std::wstring decodeAnsi(const std::string& str);
std::wstring decodeUtf8(const std::string& str);

} // namespace cxx