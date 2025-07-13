#include "include/cpusim/CPU.h"

auto main(int argc, char** argv) -> int {
  std::unique_ptr<CPU> atom = std::make_unique<CPU>();
  while (true) atom->decode(atom->fetch(atom->pc));
  return 0;
}