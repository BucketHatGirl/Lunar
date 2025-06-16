#include <thread>
#include "../Headers/Methods.hpp"

int Methods::Timer::ELAPSED = 0;
bool Methods::Timer::RUNNING = false;

void Methods::Timer::X() {
  ELAPSED = 0;
  RUNNING = true;
  while (RUNNING) {
    ++ELAPSED;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

std::thread Methods::Timer::Start() {
  std::thread THREAD(Timer::X);
  return THREAD;
}

void Methods::Timer::Stop() {
  RUNNING = false;
}
