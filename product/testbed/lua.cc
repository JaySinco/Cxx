#include "lua.h"
#include "common/utility/base.h"
#include "common/utility/string_helper.h"
#include "lauxlib.h"
#include "lualib.h"
#include <sstream>
#include <string>

using namespace cxx;

std::string luaStringfy(lua_State* L, int stackIndex, bool quoted = true)
{
    if (stackIndex <= 0)
        luaL_error(L, "%s %d", "wrong stack index", stackIndex);
    std::ostringstream ss;
    int t = lua_type(L, stackIndex);
    switch (t) {
    case LUA_TSTRING: {
        std::string sep = quoted ? "\"" : "";
        ss << sep << encodeAnsi(decodeUtf8(lua_tostring(L, stackIndex))) << sep;
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
            ss << luaStringfy(L, top + 1) << ": " << luaStringfy(L, top + 2);
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

std::string luaStackDump(lua_State* L, bool quoted = true)
{
    std::ostringstream ss;
    int top = lua_gettop(L);
    for (int i = 1; i <= top; i++) {
        ss << luaStringfy(L, i, quoted);
        if (i != top)
            ss << ", ";
    }
    return ss.str();
}

void callSimpleLuaFunc(lua_State* L, const char* func, const char* sig, ...)
{
    va_list vl;
    va_start(vl, sig);
    lua_getglobal(L, func);
    int narg = 0;
    while (*sig) {
        switch (*sig++) {
        case 'd':
            lua_pushnumber(L, va_arg(vl, double));
            break;
        case 'i':
            lua_pushnumber(L, va_arg(vl, int));
            break;
        case 's':
            lua_pushstring(L, va_arg(vl, char*));
            break;
        case '>':
            goto endwhile;
        default:
            luaL_error(L, "invalid option (%c)", *(sig - 1));
        }
        narg++;
        luaL_checkstack(L, 1, "too many arguments");
    }
endwhile:
    int nres = (int)strlen(sig);
    if (lua_pcall(L, narg, nres, 0) != LUA_OK)
        luaL_error(L, "error running function `%s': %s",
            func, lua_tostring(L, -1));
    nres = -nres;
    while (*sig) {
        switch (*sig++) {
        case 'd':
            if (!lua_isnumber(L, nres))
                luaL_error(L, "wrong result type");
            *va_arg(vl, double*) = lua_tonumber(L, nres);
            break;
        case 'i':
            if (!lua_isnumber(L, nres))
                luaL_error(L, "wrong result type");
            *va_arg(vl, int*) = (int)lua_tonumber(L, nres);
            break;
        case 's':
            if (!lua_isstring(L, nres))
                luaL_error(L, "wrong result type");
            *va_arg(vl, const char**) = lua_tostring(L, nres);
            break;
        default:
            luaL_error(L, "invalid option (%c)", *(sig - 1));
        }
        nres++;
    }
    va_end(vl);
}

int log(lua_State* L)
{
    LOG(INFO) << luaStackDump(L, false);
    lua_pushnil(L);
    return 1;
}

int test(lua_State* L)
{
    std::string source = getResAbsPath("testbed", "demo.lua");
    if (luaL_loadfile(L, source.c_str()) || lua_pcall(L, 0, LUA_MULTRET, 0)) {
        luaL_error(L, lua_tostring(L, -1));
    } else {
        if (lua_gettop(L) > 0)
            LOG(INFO) << "[RETURN] " << luaStackDump(L) << std::endl;
        int factRes;
        callSimpleLuaFunc(L, "fact", "i>i", 8, &factRes);
        LOG(INFO) << "from cpp: fact(8) = " << factRes;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_pushcfunction(L, log);
    lua_setglobal(L, "log");
    lua_pushcfunction(L, test);
    if (lua_pcall(L, 0, 0, 0))
        LOG(ERROR) << lua_tostring(L, -1) << std::endl;
    lua_close(L);
}
