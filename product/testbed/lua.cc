#include "lua.h"
#include "common/utility/logging.h"
#include "common/utility/string_helper.h"
#include "lauxlib.h"
#include "lualib.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace cxx;

std::string stringfy(lua_State* L, int stackIndex)
{
    if (stackIndex <= 0)
        luaL_error(L, "%s %d", "wrong stack index", stackIndex);
    std::ostringstream ss;
    int t = lua_type(L, stackIndex);
    switch (t) {
    case LUA_TSTRING: {
        ss << "\"" << encodeAnsi(decodeUtf8(lua_tostring(L, stackIndex))) << "\"";
        break;
    }
    case LUA_TBOOLEAN: {
        ss << (lua_toboolean(L, stackIndex) ? "true" : "false");
        break;
    }
    case LUA_TNUMBER: {
        if (lua_isinteger(L, stackIndex))
            ss << lua_tointeger(L, stackIndex);
        else
            ss << lua_tonumber(L, stackIndex);
        break;
    }
    case LUA_TNIL: {
        ss << "nil";
        break;
    }
    case LUA_TTABLE: {
        ss << "{";
        int top = lua_gettop(L);
        lua_pushnil(L);
        bool first = true;
        while (lua_next(L, stackIndex)) {
            if (!first)
                ss << ", ";
            ss << stringfy(L, top + 1) << ": " << stringfy(L, top + 2);
            lua_pop(L, 1);
            first = false;
        }
        ss << "}";
        break;
    }
    default: {
        ss << "<" << lua_typename(L, t) << ">";
        break;
    }
    }
    return ss.str();
}

std::string stackDump(lua_State* L)
{
    std::ostringstream ss;
    int top = lua_gettop(L);
    for (int i = 1; i <= top; i++) {
        ss << stringfy(L, i);
        if (i != top)
            ss << ", ";
    }
    return ss.str();
}

int l_log(lua_State* L)
{
    LOG(INFO) << stackDump(L);
    lua_pushnil(L);
    return 1;
}

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    std::string source = "D:\\Jaysinco\\Cxx\\product\\testbed\\resources\\demo.lua";
    if (argc > 1)
        source = argv[1];
    std::ifstream file(source);
    if (!file) {
        LOG(ERROR) << "failed to open source file: " << source;
        exit(0);
    }
    std::string code(
        std::istreambuf_iterator<char> { file },
        std::istreambuf_iterator<char> {});
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_pushcfunction(L, l_log);
    lua_setglobal(L, "log");
    if (luaL_loadstring(L, code.c_str()) || lua_pcall(L, 0, LUA_MULTRET, 0)) {
        std::cerr << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    } else {
        if (lua_gettop(L) > 0)
            std::cout << "[RETURN] " << stackDump(L) << std::endl;
    }
    lua_close(L);
}