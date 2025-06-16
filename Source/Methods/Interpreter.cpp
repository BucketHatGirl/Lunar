#include "../Headers/Methods.hpp"
#include <cstddef>
#include <iostream>
#include <sys/types.h>
#include <cstring>
#include <lua5.4/lua.hpp>


void Methods::Interpreter::File(lua_State* L, const char* FILE) {
  if (luaL_loadfile(L, FILE) != LUA_OK || lua_pcall(L, 0, 0, 0) != LUA_OK) {
    std::cout << "\n[Lunar] -> [Fatal] -> " << lua_tostring(L, -1) << std::endl;
    lua_close(L);
  }
}

void Methods::Interpreter::String(lua_State* L, const char *CODE) {
  if (luaL_dostring(L, CODE) != LUA_OK) {
    std::cout << "\n[Lunar] -> [Fatal] -> " << lua_tostring(L, -1) << std::endl;
    lua_close(L);
  }
} 
