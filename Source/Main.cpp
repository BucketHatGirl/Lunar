#include <algorithm>
#include <csignal>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include "Headers/Miniz.h"
#include "Headers/Methods.hpp"
#include <lua5.4/lua.hpp>

mz_zip_archive ARCHIVE;
size_t SIZE = 0;

static int Require(lua_State* L) {
  const char* NAME = luaL_checkstring(L, 1);
  std::string PATH = NAME;
  std::replace(PATH.begin(), PATH.end(), '.', '/');
  PATH += ".lua";
  size_t X = 0;
  void* DATA = Methods::Binary::FetchFused(&ARCHIVE, PATH.c_str(), &X);
  if (!DATA) return 0;

  if (luaL_loadbuffer(L, (const char*)DATA, X, "PROGRAM_REQUIRE") == LUA_OK) {
    mz_free(DATA);
    return 1;
  } else {
    std::cout << "[Lunar] -> [Error] -> Could not require file ~> " << lua_tostring(L, -1) << std::endl;
    exit(0);
  }
}

int main(int NUM, const char *ARGV[]) {
  std::thread T = Methods::Timer::Start();
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  auto [FUSED, DATA] = Methods::Binary::CheckFused();
  if (FUSED) {
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "searchers");
    lua_pushcfunction(L, Require);
    lua_rawseti(L, -2, 2);
    lua_pop(L, 2);
    Methods::Binary::InitFused(&ARCHIVE, DATA);
    void* CONFIG = Methods::Binary::FetchFused(&ARCHIVE, "Config.lua", &SIZE);
    if (CONFIG) {
      if (luaL_loadbuffer(L, (const char*)CONFIG, SIZE, "PROGRAM_CONFIG") == LUA_OK) {
        if (lua_pcall(L, 0, LUA_MULTRET, 0) != LUA_OK) {
          std::cout << "[Lunar] -> [Error] -> There is an error in your 'Config.lua' ~> " << lua_tostring(L, -1) << std::endl;
          exit(0);
        } else {
          lua_getglobal(L, "MAIN");
          mz_free(CONFIG);
          Methods::Memory::CollectGarbage(L);
          void* MAIN = Methods::Binary::FetchFused(&ARCHIVE, lua_tostring(L, -1), &SIZE);
          lua_pop(L, 1);
          if (MAIN) {
            if (luaL_loadbuffer(L, (const char*)MAIN, SIZE, "PROGRAM") == LUA_OK) {
              if (lua_pcall(L, 0, LUA_MULTRET, 0) != LUA_OK) {
                std::cout << "[Lunar] -> [Error] -> Error running program ~> " << lua_tostring(L, -1) << std::endl;
                exit(0);
              }
            }
          } else {
            std::cout << "[Lunar] -> [Error] -> Error loading buffer from memory ~> " << lua_tostring(L, -1) << std::endl;
            exit(0);
          } 
        }
      }
    } else {
      std::cout << "[Lunar] -> [Error] -> 'Config.lua' not found." << std::endl;
      exit(0);
    }
    Methods::Timer::Stop();
    T.join();
    std::cout << "\n[Lunar] -> Program ran for ~> " << Methods::Timer::ELAPSED << "ms." << std::endl;
    exit(0);
  }

  std::map<std::string, std::string> ARGS;
  for (int X = 1; X < NUM - 1; X += 2) {
    ARGS[ARGV[X]] = ARGV[X + 1];
  }

  if (!ARGS.count("help")) {
    if (ARGS.count("run")) {
      Methods::Interpreter::File(L, ARGS.at("run").c_str());
      Methods::Timer::Stop();
      T.join();
      std::cout << "[Lunar] -> Program ran for ~> " << Methods::Timer::ELAPSED << "ms." << std::endl;
      exit(0);
    }

    if (ARGS.count("string")) {
      Methods::Interpreter::String(L, ARGS.at("string").c_str());
      exit(0);
    }

    if (ARGS.count("build")) {
      Methods::Compiler::Build(ARGS.at("build").c_str());
      Methods::Timer::Stop();
      T.join();
      std::cout << "[Lunar] -> Took ~> " << Methods::Timer::ELAPSED << "ms to build." << std::endl;
      exit(0);
    }

    if (ARGS.count("dump")) {
      Methods::Binary::Dump(L, ARGS.at("dump").c_str());
      Methods::Timer::Stop();
      T.join();
      std::cout << "[Lunar] -> Took ~> " << Methods::Timer::ELAPSED << "ms to dump." << std::endl;
      exit(0);
    }
    exit(0);
  } else {

  }
  return 0;
}
