#pragma once
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
#include <string>

namespace cxx {

std::string luaStringfy(lua_State* L, int stackIndex, bool quoted = true);
std::string luaStackDump(lua_State* L, bool quoted = true);
void callSimpleLuaFunc(lua_State* L, const char* func, const char* sig, ...);

} // namespace cxx