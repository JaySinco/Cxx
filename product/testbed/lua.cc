#include "common/utility/logging.h"
#include "common/utility/string_helper.h"
#include <fstream>
#include <iostream>
#include <lua.hpp>
#include <sstream>

using namespace cxx;

static int l_log(lua_State* L)
{
    std::ostringstream ss;
    int top = lua_gettop(L);
    for (int i = 1; i <= top; i++) {
        if (i > 1)
            ss << " ";
        std::string str(lua_tostring(L, i));
        ss << encodeAnsi(decodeUtf8(str));
    }

    LOG(INFO) << ss.str();
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
    if (luaL_loadstring(L, code.c_str()) || lua_pcall(L, 0, 0, 0)) {
        std::cerr << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }
    lua_close(L);
}