#include "include/cpusim/CPU.h"

auto main(int argc, char** argv) -> int {
  std::unique_ptr<CPU> atom = std::make_unique<CPU>();
  atom->decode(atom->fetch(atom->pc));
  return 0;
}

// #include "src/tests2.cpp"

// int main() {
//   std::cout << "Running RV32I Immediate Instructions Test Suite" << std::endl;
//   std::cout << "================================================" << std::endl;

//   test_addi();
//   test_xori();
//   test_ori();
//   test_andi();
//   test_slli();
//   test_slti();
//   test_sltiu();
//   test_srli();
//   test_srai();
//   test_edge_cases();

//   std::cout << "\n=== Test Suite Complete ===" << std::endl;
//   std::cout << "Review any FAIL results above." << std::endl;
//   std::cout << "Note: This assumes x0 is hardwired to 0 and SLLI uses only lower 5 bits."
//             << std::endl;

//   return 0;
// }