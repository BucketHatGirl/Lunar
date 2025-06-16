#pragma once

#include <string>


namespace Source {
  #if defined(_WIN32)
    #include <windows.h>
    inline std::string Path() {
      char BUFFER[MAX_PATH];
      DWORD LENGTH = GetModuleFileNameA(NULL, BUFFER, MAX_PATH);
      return std::string(BUFFER, LENGTH);
    }
  #elif defined(__APPLE__)
    #include <mach-o/dyld.h>
    inline std::string Path() {
        char BUFFER[1024];
        uint32_t size = sizeof(BUFFER);
        if (_NSGetExecutablePath(BUFFER, &size) == 0)
          return std::string(BUFFER);
        else
          return std::string();
    }
  #elif defined(__linux__)
    #include <unistd.h>
    inline std::string Path() {
        char BUFFER[1024];
        ssize_t LENGTH = readlink("/proc/self/exe", BUFFER, sizeof(BUFFER) - 1);
        if (LENGTH != -1) {
            BUFFER[LENGTH] = '\0';
            return std::string(BUFFER);
        }
        return std::string();
    }
  #else
    inline std::string Path() {
      return "";
    }

#endif
}
