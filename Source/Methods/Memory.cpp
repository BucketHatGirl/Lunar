#include <lua5.4/lua.hpp>
#include "../Headers/Methods.hpp"

void Methods::Memory::CollectGarbage(lua_State* L) {
  lua_gc(L, LUA_GCCOLLECT, 0);
}
