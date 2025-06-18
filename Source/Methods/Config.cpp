#include "../Headers/Methods.hpp"
#include <lua5.4/lua.h>
#include <lua5.4/lua.hpp>
#include <map>

struct CONFIG {
  const char* MAIN;
  std::map<const char*, bool> PRELOAD;
};

auto Methods::Config::Load(lua_State *L) {
  lua_getglobal(L, "CONFIG");
}


