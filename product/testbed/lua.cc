#include "common/utility/logging.h"
#include <iostream>
#include <lua.hpp>

static int l_log(lua_State* L)
{
    LOG(INFO) << lua_tostring(L, 1);
    lua_pushnil(L);
    return 1;
}

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_pushcfunction(L, l_log);
    lua_setglobal(L, "log");
    std::string cmd;
    std::cout << ">>> " << std::flush;
    while (std::getline(std::cin, cmd)) {
        if (luaL_loadstring(L, cmd.c_str()) || lua_pcall(L, 0, 0, 0)) {
            std::cerr << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1);
        }
        std::cout << ">>> " << std::flush;
    }
    lua_close(L);
}