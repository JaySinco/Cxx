#include "lua_helper.h"
#include "string_helper.h"
#include <sstream>

namespace cxx {

std::string luaStringfy(lua_State* L, int stackIndex, bool quoted)
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

std::string luaStackDump(lua_State* L, bool quoted)
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

} // namespace cxx