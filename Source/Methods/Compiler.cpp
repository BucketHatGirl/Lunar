#include "../Headers/Methods.hpp"
#include "../Headers/Source.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

void Methods::Compiler::Build(const char* PATH) {
  std::string SELF = Source::Path();
  std::filesystem::path TARGET(PATH);
  if (std::filesystem::exists(TARGET)) {
    if (std::filesystem::is_directory(TARGET)) {
      Methods::Zip::Directory(TARGET.c_str());
    } else {
      std::cout << "[Lunar] -> [Error] -> Directory must be passed to be compiled." << std::endl;
    }
    std::ifstream LUNAR(SELF, std::ios::binary);
    std::ifstream DATA("output.zip", std::ios::binary);
    std::ofstream BINARY("Output");
    BINARY << LUNAR.rdbuf() << DATA.rdbuf();
    BINARY.close();
    DATA.close();
    LUNAR.close();
    std::filesystem::remove("output.zip");
  }
}
