#include "common/utility/logging.h"
#include "common/utility/lua_helper.h"

using namespace cxx;

int log(lua_State* L)
{
    LOG(INFO) << luaStackDump(L, false);
    lua_pushnil(L);
    return 1;
}

int test(lua_State* L)
{
    std::string source = fromRoot("product\\testbed\\resources\\demo.lua");
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
