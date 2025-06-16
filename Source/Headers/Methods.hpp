#pragma once

#include <cstddef>
#include <lua5.4/lua.hpp>
#include <thread>
#include <utility>
#include <vector>
#include "Miniz.h"

namespace Methods {
  class Timer {
    public:
      static int ELAPSED;
      static std::thread Start();
      static void Stop();
    private:
      static void X();
      static bool RUNNING; 
  };

  class Binary {
    public:
      static std::pair<bool, std::vector<unsigned char>> CheckFused();
      static void Dump(lua_State* L, const char* FILE);
      static void InitFused(mz_zip_archive* ARCHIVE, std::vector<unsigned char> DATA);
      static void* FetchFused(mz_zip_archive* ARCHIVE, const char* FILE, size_t* SIZE);
    private:
      static int Writer(lua_State* L, const void* POINTER, size_t SIZE, void* Z);
  };

  class Interpreter {
    public:
      static void File(lua_State* L, const char* FILE);
      static void String(lua_State* L, const char *CODE);
  };

  class Compiler {
    public: 
      static void Build(const char* PATH);
      static void Inspect(const char* FILE);
  };

  class Zip {
    public:
      static void Directory(const char* PATH);
      static void File(const char* FILE);
  };

  class Memory {
    public:
      static void CollectGarbage(lua_State* L);
  };

  

};
