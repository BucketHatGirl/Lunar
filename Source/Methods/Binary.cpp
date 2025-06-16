#include "../Headers/Methods.hpp"
#include "../Headers/Source.hpp"
#include <cstddef>
#include <fstream>
#include <lua5.4/lua.hpp>
#include <iostream>
#include "../Headers/Miniz.h"

std::pair<bool, std::vector<unsigned char>> Methods::Binary::CheckFused() {
  std::ifstream PROGRAM(Source::Path(), std::ios::binary);
  const unsigned char SIGNATURE[] = {0x50, 0x4B, 0x03, 0x04};
  std::vector<unsigned char> BUFFER(
    (std::istreambuf_iterator<char>(PROGRAM)),
    std::istreambuf_iterator<char>()
  );
  for (size_t X = 0; X <= BUFFER.size() - sizeof(SIGNATURE); ++X) {
    if (std::equal(SIGNATURE, SIGNATURE + sizeof(SIGNATURE), BUFFER.begin() + X)) {
      std::vector<unsigned char> FILE(BUFFER.begin() + X, BUFFER.end());
      return {true, FILE};
    }
  }
  return {false, std::vector<unsigned char>()};
}

void Methods::Binary::Dump(lua_State *L, const char *FILE) {
  std::vector<uint8_t> BYTECODE;
  std::filesystem::path LUA_FILE(FILE);
  if (std::filesystem::exists(LUA_FILE) && std::filesystem::absolute(LUA_FILE).extension() == ".lua") {
    std::string IN_FILE = std::filesystem::absolute(LUA_FILE).string();
    std::string OUT_FILE = IN_FILE + "c";
    if (luaL_loadfile(L, IN_FILE.c_str()) != LUA_OK) {
      std::cout << "\n[Lunar] -> [Error] -> " << lua_tostring(L, -1) << std::endl;
      lua_close(L);
      exit(1);
    }
    lua_dump(L, Methods::Binary::Writer, &BYTECODE, 1);
    std::ofstream X(OUT_FILE, std::ios::binary);
    X.write(reinterpret_cast<const char*>(BYTECODE.data()), BYTECODE.size());
    X.close();
  }
}

int Methods::Binary::Writer(lua_State* L, const void* POINTER, size_t SIZE, void* Z) {
  auto* BUFFER = static_cast<std::vector<uint8_t>*>(Z);
  const uint8_t* BYTES = static_cast<const uint8_t*>(POINTER);
  BUFFER->insert(BUFFER->end(), BYTES, BYTES + SIZE);
  return 0;
}

void Methods::Binary::InitFused(mz_zip_archive* ARCHIVE, std::vector<unsigned char> DATA) {
  memset(ARCHIVE, 0, sizeof(*ARCHIVE));
  if (!mz_zip_reader_init_mem(ARCHIVE, DATA.data(), DATA.size(), 0)) {
    std::cout << "[Lunar] -> [Error] -> Unable to load fused zip." << std::endl;
    exit(0);
  }
}

void* Methods::Binary::FetchFused(mz_zip_archive* ARCHIVE, const char* FILE, size_t* SIZE) {
  return mz_zip_reader_extract_file_to_heap(ARCHIVE, FILE, SIZE, 0);
}
