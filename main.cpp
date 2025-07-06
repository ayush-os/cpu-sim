#include <cstdint>
#include <iostream>

const int MEM_SIZE = 16 * 1024 * 1024;

auto main(int argc, char** argv) -> int {
  int* regs = new int[32]();
  uint32_t pc = 0;

  // 16 MB: 0x000000 to 0xFFFFFF
  unsigned char* mem = new unsigned char[MEM_SIZE];
  strcpy(reinterpret_cast<char*>(mem), "testing 123");
  for (int i = 0; i < 11; i++) {
    std::cout << mem[pc++];
  }
  std::cout << std::endl;
}

// inline unsigned char* fetch(uint32_t& pc) { return mem[pc]; }