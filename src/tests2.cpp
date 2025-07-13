#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>

// Assuming these are your struct definitions
struct Instr {
  uint32_t rd;
  uint32_t rs1;
  int32_t imm;  // Sign-extended immediate
};

// Mock CPU class for testing
class CPU2 {
public:
  uint32_t regs[32] = {0};  // 32 registers, x0 always 0

  void execADDI(const Instr& instr) { regs[instr.rd] = regs[instr.rs1] + instr.imm; }

  void execXORI(const Instr& instr) { regs[instr.rd] = regs[instr.rs1] ^ instr.imm; }

  void execORI(const Instr& instr) { regs[instr.rd] = regs[instr.rs1] | instr.imm; }

  void execANDI(const Instr& instr) { regs[instr.rd] = regs[instr.rs1] & instr.imm; }

  void execSLLI(const Instr& instr) { regs[instr.rd] = regs[instr.rs1] << instr.imm; }

  void execSLTI(const Instr& instr) {
    regs[instr.rd]
        = (static_cast<int32_t>(regs[instr.rs1]) < static_cast<int32_t>(instr.imm)) ? 1 : 0;
  }
  const uint8_t REG_MASK = 0x1F;
  void execSLTIU(const Instr& instr) {
    regs[instr.rd]
        = (static_cast<uint32_t>(regs[instr.rs1]) < static_cast<uint32_t>(instr.imm)) ? 1 : 0;
  }

  void execSRLI(const Instr& instr) { regs[instr.rd] = regs[instr.rs1] >> (instr.imm & REG_MASK); }

  void execSRAI(const Instr& instr) {
    regs[instr.rd] = static_cast<int32_t>(regs[instr.rs1]) >> (instr.imm & REG_MASK);
  }

  void reset() {
    for (int i = 0; i < 32; i++) {
      regs[i] = 0;
    }
  }
};

// Test helper functions
void print_test_result(const std::string& test_name, bool passed) {
  std::cout << "[" << (passed ? "PASS" : "FAIL") << "] " << test_name << std::endl;
}

void test_addi() {
  CPU2 cpu;
  std::cout << "\n=== Testing ADDI ===" << std::endl;

  // Test basic addition
  cpu.regs[1] = 10;
  cpu.execADDI({2, 1, 5});
  print_test_result("ADDI basic", cpu.regs[2] == 15);

  // Test with zero
  cpu.reset();
  cpu.regs[1] = 0;
  cpu.execADDI({2, 1, 42});
  print_test_result("ADDI with zero source", cpu.regs[2] == 42);

  // Test with negative immediate
  cpu.reset();
  cpu.regs[1] = 50;
  cpu.execADDI({2, 1, -20});
  print_test_result("ADDI negative immediate", cpu.regs[2] == 30);

  // Test overflow behavior (should wrap around)
  cpu.reset();
  cpu.regs[1] = 0x7FFFFFFF;  // Max positive 32-bit
  cpu.execADDI({2, 1, 1});
  print_test_result("ADDI overflow", cpu.regs[2] == 0x80000000);

  // Test underflow behavior
  cpu.reset();
  cpu.regs[1] = 0x80000000;  // Min negative 32-bit
  cpu.execADDI({2, 1, -1});
  print_test_result("ADDI underflow", cpu.regs[2] == 0x7FFFFFFF);

  // Test writing to x0 (should not change)
  cpu.reset();
  cpu.regs[1] = 100;
  cpu.execADDI({0, 1, 50});
  print_test_result("ADDI to x0", cpu.regs[0] == 0);
}

void test_xori() {
  CPU2 cpu;
  std::cout << "\n=== Testing XORI ===" << std::endl;

  // Test basic XOR
  cpu.regs[1] = 0b10101010;
  cpu.execXORI({2, 1, 0b11001100});
  print_test_result("XORI basic", cpu.regs[2] == 0b01100110);

  // Test XOR with 0 (should be identity)
  cpu.reset();
  cpu.regs[1] = 0x12345678;
  cpu.execXORI({2, 1, 0});
  print_test_result("XORI with 0", cpu.regs[2] == 0x12345678);

  // Test XOR with -1 (should invert all bits)
  cpu.reset();
  cpu.regs[1] = 0x12345678;
  cpu.execXORI({2, 1, -1});
  print_test_result("XORI with -1", cpu.regs[2] == ~0x12345678);

  // Test XOR with self (via immediate)
  cpu.reset();
  cpu.regs[1] = 0x555;
  cpu.execXORI({2, 1, 0x555});
  print_test_result("XORI self-cancel", cpu.regs[2] == 0);
}

void test_ori() {
  CPU2 cpu;
  std::cout << "\n=== Testing ORI ===" << std::endl;

  // Test basic OR
  cpu.regs[1] = 0b10101010;
  cpu.execORI({2, 1, 0b11001100});
  print_test_result("ORI basic", cpu.regs[2] == 0b11101110);

  // Test OR with 0 (should be identity)
  cpu.reset();
  cpu.regs[1] = 0x12345678;
  cpu.execORI({2, 1, 0});
  print_test_result("ORI with 0", cpu.regs[2] == 0x12345678);

  // Test OR with -1 (should set all bits)
  cpu.reset();
  cpu.regs[1] = 0x12345678;
  cpu.execORI({2, 1, -1});
  print_test_result("ORI with -1", cpu.regs[2] == 0xFFFFFFFF);

  // Test setting specific bits
  cpu.reset();
  cpu.regs[1] = 0x00000000;
  cpu.execORI({2, 1, 0x0F0F});
  print_test_result("ORI set bits", cpu.regs[2] == 0x0F0F);
}

void test_andi() {
  CPU2 cpu;
  std::cout << "\n=== Testing ANDI ===" << std::endl;

  // Test basic AND
  cpu.regs[1] = 0b11111111;
  cpu.execANDI({2, 1, 0b10101010});
  print_test_result("ANDI basic", cpu.regs[2] == 0b10101010);

  // Test AND with 0 (should clear all)
  cpu.reset();
  cpu.regs[1] = 0x12345678;
  cpu.execANDI({2, 1, 0});
  print_test_result("ANDI with 0", cpu.regs[2] == 0);

  // Test AND with -1 (should be identity)
  cpu.reset();
  cpu.regs[1] = 0x12345678;
  cpu.execANDI({2, 1, -1});
  print_test_result("ANDI with -1", cpu.regs[2] == 0x12345678);

  // Test masking lower bits
  cpu.reset();
  cpu.regs[1] = 0x12345678;
  cpu.execANDI({2, 1, 0xFF});
  print_test_result("ANDI mask lower", cpu.regs[2] == 0x78);
}

void test_slli() {
  CPU2 cpu;
  std::cout << "\n=== Testing SLLI ===" << std::endl;

  // Test basic shift
  cpu.regs[1] = 0b1010;
  cpu.execSLLI({2, 1, 2});
  print_test_result("SLLI basic", cpu.regs[2] == 0b101000);

  // Test shift by 0
  cpu.reset();
  cpu.regs[1] = 0x12345678;
  cpu.execSLLI({2, 1, 0});
  print_test_result("SLLI by 0", cpu.regs[2] == 0x12345678);

  // Test maximum shift (31 bits)
  cpu.reset();
  cpu.regs[1] = 1;
  cpu.execSLLI({2, 1, 31});
  print_test_result("SLLI max shift", cpu.regs[2] == 0x80000000);

  // Test shift overflow (bits should be lost)
  cpu.reset();
  cpu.regs[1] = 0x80000000;
  cpu.execSLLI({2, 1, 1});
  print_test_result("SLLI overflow", cpu.regs[2] == 0);

  // Test that only lower 5 bits of immediate are used
  cpu.reset();
  cpu.regs[1] = 0b1010;
  cpu.execSLLI({2, 1, 32 + 2});  // Should be equivalent to shift by 2
  print_test_result("SLLI modulo 32", cpu.regs[2] == 0b101000);
}

void test_slti() {
  CPU2 cpu;
  std::cout << "\n=== Testing SLTI ===" << std::endl;

  // Test less than (true)
  cpu.regs[1] = 5;
  cpu.execSLTI({2, 1, 10});
  print_test_result("SLTI less than", cpu.regs[2] == 1);

  // Test greater than (false)
  cpu.reset();
  cpu.regs[1] = 15;
  cpu.execSLTI({2, 1, 10});
  print_test_result("SLTI greater than", cpu.regs[2] == 0);

  // Test equal (false)
  cpu.reset();
  cpu.regs[1] = 10;
  cpu.execSLTI({2, 1, 10});
  print_test_result("SLTI equal", cpu.regs[2] == 0);

  // Test negative numbers
  cpu.reset();
  cpu.regs[1] = 0xFFFFFFFF;  // -1
  cpu.execSLTI({2, 1, 0});
  print_test_result("SLTI negative < 0", cpu.regs[2] == 1);

  // Test positive vs negative immediate
  cpu.reset();
  cpu.regs[1] = 5;
  cpu.execSLTI({2, 1, -10});
  print_test_result("SLTI positive vs negative imm", cpu.regs[2] == 0);

  // Test large numbers
  cpu.reset();
  cpu.regs[1] = 0x80000000;          // Most negative 32-bit
  cpu.execSLTI({2, 1, 0x7FFFFFFF});  // Most positive 32-bit
  print_test_result("SLTI min vs max", cpu.regs[2] == 1);
}

void test_sltiu() {
  CPU2 cpu;
  std::cout << "\n=== Testing SLTIU ===" << std::endl;

  // Test less than (true)
  cpu.regs[1] = 5;
  cpu.execSLTIU({2, 1, 10});
  print_test_result("SLTIU less than", cpu.regs[2] == 1);

  // Test greater than (false)
  cpu.reset();
  cpu.regs[1] = 15;
  cpu.execSLTIU({2, 1, 10});
  print_test_result("SLTIU greater than", cpu.regs[2] == 0);

  // Test equal (false)
  cpu.reset();
  cpu.regs[1] = 10;
  cpu.execSLTIU({2, 1, 10});
  print_test_result("SLTIU equal", cpu.regs[2] == 0);

  // Test unsigned comparison (-1 is largest unsigned)
  cpu.reset();
  cpu.regs[1] = 0xFFFFFFFF;  // -1 as signed, largest as unsigned
  cpu.execSLTIU({2, 1, 0});
  print_test_result("SLTIU unsigned -1 vs 0", cpu.regs[2] == 0);

  // Test 0 vs -1 (unsigned)
  cpu.reset();
  cpu.regs[1] = 0;
  cpu.execSLTIU({2, 1, -1});  // -1 sign-extended is 0xFFFFFFFF
  print_test_result("SLTIU 0 vs unsigned -1", cpu.regs[2] == 1);

  //   // Test boundary case
  //   cpu.reset();
  //   cpu.regs[1] = 0x7FFFFFFF;
  //   cpu.execSLTIU({2, 1, 0x80000000});
  //   print_test_result("SLTIU boundary", cpu.regs[2] == 1);
}

void test_srli() {
  CPU2 cpu;
  std::cout << "\n=== Testing SRLI ===" << std::endl;

  // Test basic logical right shift
  cpu.regs[1] = 0b1010000;
  cpu.execSRLI({2, 1, 2});
  print_test_result("SRLI basic", cpu.regs[2] == 0b10100);

  // Test shift by 0
  cpu.reset();
  cpu.regs[1] = 0x12345678;
  cpu.execSRLI({2, 1, 0});
  print_test_result("SRLI by 0", cpu.regs[2] == 0x12345678);

  // Test maximum shift (31 bits)
  cpu.reset();
  cpu.regs[1] = 0x80000000;
  cpu.execSRLI({2, 1, 31});
  print_test_result("SRLI max shift", cpu.regs[2] == 1);

  // Test logical shift with negative number (should zero-fill)
  cpu.reset();
  cpu.regs[1] = 0x80000000;  // MSB set
  cpu.execSRLI({2, 1, 1});
  print_test_result("SRLI negative zero-fill", cpu.regs[2] == 0x40000000);

  // Test all bits shifted out
  cpu.reset();
  cpu.regs[1] = 0x7FFFFFFF;
  cpu.execSRLI({2, 1, 31});
  print_test_result("SRLI all bits out", cpu.regs[2] == 0);

  // Test that only lower 5 bits of immediate are used
  cpu.reset();
  cpu.regs[1] = 0b1010000;
  cpu.execSRLI({2, 1, 32 + 2});  // Should be equivalent to shift by 2
  print_test_result("SRLI modulo 32", cpu.regs[2] == 0b10100);

  // Test with 0xFF pattern
  cpu.reset();
  cpu.regs[1] = 0xFF000000;
  cpu.execSRLI({2, 1, 8});
  print_test_result("SRLI byte shift", cpu.regs[2] == 0x00FF0000);
}

void test_srai() {
  CPU2 cpu;
  std::cout << "\n=== Testing SRAI ===" << std::endl;

  // Test basic arithmetic right shift with positive number
  cpu.regs[1] = 0b1010000;
  cpu.execSRAI({2, 1, 2});
  print_test_result("SRAI basic positive", cpu.regs[2] == 0b10100);

  // Test shift by 0
  cpu.reset();
  cpu.regs[1] = 0x12345678;
  cpu.execSRAI({2, 1, 0});
  print_test_result("SRAI by 0", cpu.regs[2] == 0x12345678);

  // Test arithmetic shift with negative number (should sign-extend)
  cpu.reset();
  cpu.regs[1] = 0x80000000;  // Most negative 32-bit number
  cpu.execSRAI({2, 1, 1});
  print_test_result("SRAI negative sign-extend", cpu.regs[2] == 0xC0000000);

  // Test maximum shift with positive number
  cpu.reset();
  cpu.regs[1] = 0x7FFFFFFF;  // Most positive 32-bit number
  cpu.execSRAI({2, 1, 31});
  print_test_result("SRAI max shift positive", cpu.regs[2] == 0);

  // Test maximum shift with negative number
  cpu.reset();
  cpu.regs[1] = 0x80000000;  // Most negative 32-bit number
  cpu.execSRAI({2, 1, 31});
  print_test_result("SRAI max shift negative", cpu.regs[2] == 0xFFFFFFFF);

  // Test -1 shifted (should remain -1)
  cpu.reset();
  cpu.regs[1] = 0xFFFFFFFF;  // -1
  cpu.execSRAI({2, 1, 5});
  print_test_result("SRAI -1 remains -1", cpu.regs[2] == 0xFFFFFFFF);

  // Test that only lower 5 bits of immediate are used
  cpu.reset();
  cpu.regs[1] = 0x80000000;
  cpu.execSRAI({2, 1, 32 + 1});  // Should be equivalent to shift by 1
  print_test_result("SRAI modulo 32", cpu.regs[2] == 0xC0000000);

  // Test small negative number
  cpu.reset();
  cpu.regs[1] = 0xFFFFFFF0;  // -16
  cpu.execSRAI({2, 1, 2});
  print_test_result("SRAI small negative", cpu.regs[2] == 0xFFFFFFFC);  // -4

  // Test boundary between positive and negative
  cpu.reset();
  cpu.regs[1] = 0x7FFFFFFF;
  cpu.execSRAI({2, 1, 1});
  print_test_result("SRAI positive boundary", cpu.regs[2] == 0x3FFFFFFF);
}

void test_edge_cases() {
  CPU2 cpu;
  std::cout << "\n=== Testing Edge Cases ===" << std::endl;

  // Test all instructions writing to x0
  cpu.reset();
  cpu.regs[1] = 0x12345678;

  cpu.execADDI({0, 1, 100});
  cpu.execXORI({0, 1, 0xFF});
  cpu.execORI({0, 1, 0xFF});
  cpu.execANDI({0, 1, 0xFF});
  cpu.execSLLI({0, 1, 5});
  cpu.execSRLI({0, 1, 5});
  cpu.execSRAI({0, 1, 5});
  cpu.execSLTI({0, 1, 100});
  cpu.execSLTIU({0, 1, 100});

  print_test_result("All instructions preserve x0", cpu.regs[0] == 0);

  // Test maximum and minimum immediates
  cpu.reset();
  cpu.regs[1] = 0;
  cpu.execADDI({2, 1, 2047});  // Max 12-bit signed immediate
  print_test_result("Max positive immediate", cpu.regs[2] == 2047);

  cpu.reset();
  cpu.regs[1] = 0;
  cpu.execADDI({2, 1, -2048});  // Min 12-bit signed immediate
  print_test_result("Max negative immediate", cpu.regs[2] == 0xFFFFF800);

  // Test shift instructions with edge shift amounts
  cpu.reset();
  cpu.regs[1] = 0x12345678;
  cpu.execSLLI({2, 1, 32});  // Should be equivalent to shift by 0
  print_test_result("Shift by 32 (mod 32)", cpu.regs[2] == 0x12345678);

  // Test difference between SRLI and SRAI with negative numbers
  cpu.reset();
  cpu.regs[1] = 0x80000008;  // Negative number with low bits set
  cpu.execSRLI({2, 1, 4});   // Logical shift
  cpu.execSRAI({3, 1, 4});   // Arithmetic shift
  bool logical_correct = (cpu.regs[2] == 0x08000000);
  bool arithmetic_correct = (cpu.regs[3] == 0xF8000000);
  print_test_result("SRLI vs SRAI difference", logical_correct && arithmetic_correct);
}