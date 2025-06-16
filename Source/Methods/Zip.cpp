#include "../Headers/Methods.hpp"
#include "../Headers/Miniz.h"
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>

void Methods::Zip::Directory(const char* PATH) {
  mz_zip_archive ZIP;
  memset(&ZIP, 0, sizeof(ZIP));
  mz_zip_writer_init_file(&ZIP, "output.zip", 0);
  for (const auto& ENTRY : std::filesystem::recursive_directory_iterator(PATH)) {
    if (ENTRY.is_regular_file()) {
      std::string FILE = ENTRY.path();
      std::string FILE_NAME = ENTRY.path().filename();
      mz_zip_writer_add_file(&ZIP, FILE_NAME.c_str(), FILE.c_str(), NULL, 0, 0);
    }
  }
  mz_zip_writer_finalize_archive(&ZIP);
  mz_zip_writer_end(&ZIP);
}
