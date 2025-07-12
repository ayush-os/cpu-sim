// RISC-V SLL (Shift Left Logical) Test Cases
// Format: rs1, rs2, expected_result, description

#include <cstdint>
#include <cstdio>
struct SLLTestCase {
  uint32_t rs1;
  uint32_t rs2;
  uint32_t expected;
  const char* description;
};

SLLTestCase sll_tests[] = {
    // Basic functionality
    {0x00000001, 0x00000000, 0x00000001, "No shift (shift by 0)"},
    {0x00000001, 0x00000001, 0x00000002, "Shift 1 by 1 position"},
    {0x00000001, 0x00000004, 0x00000010, "Shift 1 by 4 positions"},
    {0x00000004, 0x00000002, 0x00000010, "Shift 4 by 2 positions"},

    // Shift amount masking (only lower 5 bits used)
    {0x00000001, 0x00000020, 0x00000001, "Shift by 32 (masked to 0)"},
    {0x00000001, 0x00000021, 0x00000002, "Shift by 33 (masked to 1)"},
    {0x00000004, 0x0000007F, 0x00000000, "Shift by 127 (masked to 31) - overflow"},
    {0x00000001, 0xFFFFFFFF, 0x80000000, "Shift by -1 (masked to 31)"},

    // Overflow cases
    {0x00000001, 0x0000001F, 0x80000000, "Shift 1 by 31 (max valid shift)"},
    {0x00000002, 0x0000001E, 0x80000000, "Shift 2 by 30 - no overflow"},
    {0x00000002, 0x0000001F, 0x00000000, "Shift 2 by 31 - overflow to 0"},
    {0x00000004, 0x0000001E, 0x00000000, "Shift 4 by 30 - overflow to 0"},
    {0x00000004, 0x0000001D, 0x80000000, "Shift 4 by 29 - no overflow"},

    // Edge cases with large numbers
    {0x80000000, 0x00000001, 0x00000000, "Shift MSB by 1 - overflow"},
    {0x40000000, 0x00000001, 0x80000000, "Shift 0x40000000 by 1"},
    {0xFFFFFFFF, 0x00000001, 0xFFFFFFFE, "Shift all 1s by 1"},
    {0xFFFFFFFF, 0x00000008, 0xFFFFFF00, "Shift all 1s by 8"},

    // Zero cases
    {0x00000000, 0x00000000, 0x00000000, "Shift 0 by 0"},
    {0x00000000, 0x0000001F, 0x00000000, "Shift 0 by 31"},
    {0x00000000, 0xFFFFFFFF, 0x00000000, "Shift 0 by any amount"},
};

// RISC-V SLT (Set Less Than) Test Cases
// Format: rs1, rs2, expected_result, description

struct SLTTestCase {
  uint32_t rs1;
  uint32_t rs2;
  uint32_t expected;
  const char* description;
};

SLTTestCase slt_tests[] = {
    // Basic comparisons
    {0x00000000, 0x00000001, 0x00000001, "0 < 1 (true)"},
    {0x00000001, 0x00000000, 0x00000000, "1 < 0 (false)"},
    {0x00000001, 0x00000001, 0x00000000, "1 < 1 (false - equal)"},
    {0x00000005, 0x0000000A, 0x00000001, "5 < 10 (true)"},
    {0x0000000A, 0x00000005, 0x00000000, "10 < 5 (false)"},

    // Signed comparisons (SLT treats values as signed)
    {0x00000000, 0x80000000, 0x00000000, "0 < -2147483648 (false)"},
    {0x80000000, 0x00000000, 0x00000001, "-2147483648 < 0 (true)"},
    {0x80000000, 0x7FFFFFFF, 0x00000001, "-2147483648 < 2147483647 (true)"},
    {0x7FFFFFFF, 0x80000000, 0x00000000, "2147483647 < -2147483648 (false)"},

    // Negative number comparisons
    {0xFFFFFFFF, 0x00000000, 0x00000001, "-1 < 0 (true)"},
    {0x00000000, 0xFFFFFFFF, 0x00000000, "0 < -1 (false)"},
    {0xFFFFFFFF, 0xFFFFFFFE, 0x00000000, "-1 < -2 (false)"},
    {0xFFFFFFFE, 0xFFFFFFFF, 0x00000001, "-2 < -1 (true)"},
    {0x80000001, 0x80000000, 0x00000000, "-2147483647 < -2147483648 (false)"},

    // Edge cases
    {0x7FFFFFFF, 0x7FFFFFFF, 0x00000000, "MAX_INT < MAX_INT (false - equal)"},
    {0x80000000, 0x80000000, 0x00000000, "MIN_INT < MIN_INT (false - equal)"},
    {0x7FFFFFFE, 0x7FFFFFFF, 0x00000001, "MAX_INT-1 < MAX_INT (true)"},
    {0x80000000, 0x80000001, 0x00000001, "MIN_INT < MIN_INT+1 (true)"},

    // Mixed positive/negative
    {0x00000001, 0xFFFFFFFF, 0x00000000, "1 < -1 (false)"},
    {0xFFFFFFFF, 0x00000001, 0x00000001, "-1 < 1 (true)"},
    {0x12345678, 0x87654321, 0x00000000, "pos < neg (false)"},
    {0x87654321, 0x12345678, 0x00000001, "neg < pos (true)"},
};

// RISC-V SLTU (Set Less Than Unsigned) Test Cases
// Format: rs1, rs2, expected_result, description

struct SLTUTestCase {
  uint32_t rs1;
  uint32_t rs2;
  uint32_t expected;
  const char* description;
};

SLTUTestCase sltu_tests[] = {
    // Basic unsigned comparisons
    {0x00000000, 0x00000001, 0x00000001, "0 < 1 (true)"},
    {0x00000001, 0x00000000, 0x00000000, "1 < 0 (false)"},
    {0x00000001, 0x00000001, 0x00000000, "1 < 1 (false - equal)"},
    {0x00000005, 0x0000000A, 0x00000001, "5 < 10 (true)"},
    {0x0000000A, 0x00000005, 0x00000000, "10 < 5 (false)"},

    // Unsigned vs signed difference (key difference from SLT)
    {0x00000000, 0x80000000, 0x00000001, "0 < 0x80000000 (true - unsigned)"},
    {0x80000000, 0x00000000, 0x00000000, "0x80000000 < 0 (false - unsigned)"},
    {0x80000000, 0x7FFFFFFF, 0x00000000, "0x80000000 < 0x7FFFFFFF (false - unsigned)"},
    {0x7FFFFFFF, 0x80000000, 0x00000001, "0x7FFFFFFF < 0x80000000 (true - unsigned)"},

    // Large unsigned numbers (would be negative in signed)
    {0xFFFFFFFF, 0x00000000, 0x00000000, "0xFFFFFFFF < 0 (false - max unsigned)"},
    {0x00000000, 0xFFFFFFFF, 0x00000001, "0 < 0xFFFFFFFF (true - max unsigned)"},
    {0xFFFFFFFF, 0xFFFFFFFE, 0x00000000, "0xFFFFFFFF < 0xFFFFFFFE (false)"},
    {0xFFFFFFFE, 0xFFFFFFFF, 0x00000001, "0xFFFFFFFE < 0xFFFFFFFF (true)"},
    {0x80000001, 0x80000000, 0x00000000, "0x80000001 < 0x80000000 (false)"},

    // Edge cases
    {0x7FFFFFFF, 0x7FFFFFFF, 0x00000000, "0x7FFFFFFF < 0x7FFFFFFF (false - equal)"},
    {0x80000000, 0x80000000, 0x00000000, "0x80000000 < 0x80000000 (false - equal)"},
    {0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, "0xFFFFFFFF < 0xFFFFFFFF (false - equal)"},
    {0x7FFFFFFE, 0x7FFFFFFF, 0x00000001, "0x7FFFFFFE < 0x7FFFFFFF (true)"},
    {0x80000000, 0x80000001, 0x00000001, "0x80000000 < 0x80000001 (true)"},

    // Compare with SLT results (these should be different)
    {0xFFFFFFFF, 0x00000001, 0x00000000, "0xFFFFFFFF < 1 (false - unsigned, but true in signed)"},
    {0x00000001, 0xFFFFFFFF, 0x00000001, "1 < 0xFFFFFFFF (true - unsigned, but false in signed)"},
    {0x12345678, 0x87654321, 0x00000001, "0x12345678 < 0x87654321 (true - unsigned)"},
    {0x87654321, 0x12345678, 0x00000000, "0x87654321 < 0x12345678 (false - unsigned)"},

    // Sequential comparisons
    {0x00000000, 0x00000001, 0x00000001, "0 < 1"},
    {0x00000001, 0x00000002, 0x00000001, "1 < 2"},
    {0x7FFFFFFE, 0x7FFFFFFF, 0x00000001, "0x7FFFFFFE < 0x7FFFFFFF"},
    {0x7FFFFFFF, 0x80000000, 0x00000001, "0x7FFFFFFF < 0x80000000 (crosses sign boundary)"},
    {0x80000000, 0x80000001, 0x00000001, "0x80000000 < 0x80000001"},
    {0xFFFFFFFE, 0xFFFFFFFF, 0x00000001, "0xFFFFFFFE < 0xFFFFFFFF"},

    // Zero comparisons
    {0x00000000, 0x00000000, 0x00000000, "0 < 0 (false)"},
    {0x00000000, 0x80000000, 0x00000001, "0 < 0x80000000 (true)"},
    {0x00000000, 0xFFFFFFFF, 0x00000001, "0 < 0xFFFFFFFF (true)"},
};

// RISC-V XOR (Exclusive OR) Test Cases
// Format: rs1, rs2, expected_result, description

struct XORTestCase {
  uint32_t rs1;
  uint32_t rs2;
  uint32_t expected;
  const char* description;
};

XORTestCase xor_tests[] = {
    // Basic XOR operations
    {0x00000000, 0x00000000, 0x00000000, "0 XOR 0 = 0"},
    {0x00000001, 0x00000000, 0x00000001, "1 XOR 0 = 1"},
    {0x00000000, 0x00000001, 0x00000001, "0 XOR 1 = 1"},
    {0x00000001, 0x00000001, 0x00000000, "1 XOR 1 = 0"},

    // XOR with same value (always 0)
    {0x12345678, 0x12345678, 0x00000000, "Same value XOR = 0"},
    {0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, "All 1s XOR all 1s = 0"},
    {0x80000000, 0x80000000, 0x00000000, "MSB XOR MSB = 0"},
    {0x7FFFFFFF, 0x7FFFFFFF, 0x00000000, "Max positive XOR itself = 0"},

    // XOR with 0 (identity)
    {0x12345678, 0x00000000, 0x12345678, "Value XOR 0 = value"},
    {0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, "All 1s XOR 0 = all 1s"},
    {0x80000000, 0x00000000, 0x80000000, "MSB XOR 0 = MSB"},
    {0x00000000, 0x12345678, 0x12345678, "0 XOR value = value"},

    // XOR with all 1s (bitwise NOT)
    {0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, "0 XOR all 1s = all 1s"},
    {0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, "All 1s XOR all 1s = 0"},
    {0x12345678, 0xFFFFFFFF, 0xEDCBA987, "Value XOR all 1s = NOT value"},
    {0x80000000, 0xFFFFFFFF, 0x7FFFFFFF, "MSB XOR all 1s = max positive"},
    {0x7FFFFFFF, 0xFFFFFFFF, 0x80000000, "Max positive XOR all 1s = MSB"},

    // Bit manipulation patterns
    {0x0000FFFF, 0xFFFF0000, 0xFFFFFFFF, "Lower 16 bits XOR upper 16 bits"},
    {0xAAAAAAAA, 0x55555555, 0xFFFFFFFF, "Alternating bits XOR = all 1s"},
    {0x55555555, 0xAAAAAAAA, 0xFFFFFFFF, "Alternating bits XOR = all 1s"},
    {0xF0F0F0F0, 0x0F0F0F0F, 0xFFFFFFFF, "Nibble pattern XOR = all 1s"},

    // Single bit operations
    {0x00000001, 0x00000002, 0x00000003, "Bit 0 XOR bit 1"},
    {0x00000001, 0x00000003, 0x00000002, "Bit 0 XOR (bit 0 + bit 1)"},
    {0x80000000, 0x40000000, 0xC0000000, "MSB XOR bit 30"},
    {0x00000001, 0x80000000, 0x80000001, "LSB XOR MSB"},

    // Powers of 2 XOR
    {0x00000001, 0x00000004, 0x00000005, "2^0 XOR 2^2"},
    {0x00000010, 0x00000100, 0x00000110, "2^4 XOR 2^8"},
    {0x00001000, 0x00010000, 0x00011000, "2^12 XOR 2^16"},
    {0x80000000, 0x00000001, 0x80000001, "2^31 XOR 2^0"},

    // Mask operations
    {0x12345678, 0x0000FFFF, 0x1234A987, "Mask upper 16 bits"},
    {0x12345678, 0xFFFF0000, 0xEDCB5678, "Mask lower 16 bits"},
    {0x12345678, 0x00FF00FF, 0x12CB5687, "Mask alternating bytes"},
    {0x12345678, 0x0F0F0F0F, 0x1D3B5977, "Mask alternating nibbles"},

    // Sequential bit patterns
    {0x00000001, 0x00000002, 0x00000003, "1 XOR 2 = 3"},
    {0x00000003, 0x00000002, 0x00000001, "3 XOR 2 = 1"},
    {0x00000005, 0x00000003, 0x00000006, "5 XOR 3 = 6"},
    {0x00000006, 0x00000003, 0x00000005, "6 XOR 3 = 5"},

    // Large number XOR
    {0x12345678, 0x9ABCDEF0, 0x88888888, "Large number XOR pattern"},
    {0x87654321, 0xFEDCBA98, 0x79B9F9B9, "Large number XOR pattern 2"},
    {0xDEADBEEF, 0xCAFEBABE, 0x14530451, "Hex patterns XOR"},

    // Edge cases
    {0x00000000, 0x80000000, 0x80000000, "0 XOR min signed"},
    {0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, "Max positive XOR min signed"},
    {0x00000001, 0xFFFFFFFE, 0xFFFFFFFF, "1 XOR (-2 in 2s complement)"},
    {0xFFFFFFFF, 0x00000001, 0xFFFFFFFE, "All 1s XOR 1"},
};

// RISC-V OR (Bitwise OR) Test Cases
// Format: rs1, rs2, expected_result, description

struct ORTestCase {
  uint32_t rs1;
  uint32_t rs2;
  uint32_t expected;
  const char* description;
};

ORTestCase or_tests[] = {
    // Basic OR operations
    {0x00000000, 0x00000000, 0x00000000, "0 OR 0 = 0"},
    {0x00000001, 0x00000000, 0x00000001, "1 OR 0 = 1"},
    {0x00000000, 0x00000001, 0x00000001, "0 OR 1 = 1"},
    {0x00000001, 0x00000001, 0x00000001, "1 OR 1 = 1"},

    // OR with same value (idempotent)
    {0x12345678, 0x12345678, 0x12345678, "Same value OR = same value"},
    {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, "All 1s OR all 1s = all 1s"},
    {0x80000000, 0x80000000, 0x80000000, "MSB OR MSB = MSB"},
    {0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, "Max positive OR itself = same"},

    // OR with 0 (identity)
    {0x12345678, 0x00000000, 0x12345678, "Value OR 0 = value"},
    {0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, "All 1s OR 0 = all 1s"},
    {0x80000000, 0x00000000, 0x80000000, "MSB OR 0 = MSB"},
    {0x00000000, 0x12345678, 0x12345678, "0 OR value = value"},

    // OR with all 1s (absorption)
    {0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, "0 OR all 1s = all 1s"},
    {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, "All 1s OR all 1s = all 1s"},
    {0x12345678, 0xFFFFFFFF, 0xFFFFFFFF, "Value OR all 1s = all 1s"},
    {0x80000000, 0xFFFFFFFF, 0xFFFFFFFF, "MSB OR all 1s = all 1s"},
    {0x7FFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, "Max positive OR all 1s = all 1s"},

    // Bit combination patterns
    {0x0000FFFF, 0xFFFF0000, 0xFFFFFFFF, "Lower 16 bits OR upper 16 bits"},
    {0xAAAAAAAA, 0x55555555, 0xFFFFFFFF, "Alternating bits OR = all 1s"},
    {0x55555555, 0xAAAAAAAA, 0xFFFFFFFF, "Alternating bits OR = all 1s"},
    {0xF0F0F0F0, 0x0F0F0F0F, 0xFFFFFFFF, "Nibble pattern OR = all 1s"},

    // Single bit operations
    {0x00000001, 0x00000002, 0x00000003, "Bit 0 OR bit 1"},
    {0x00000001, 0x00000003, 0x00000003, "Bit 0 OR (bit 0 + bit 1)"},
    {0x80000000, 0x40000000, 0xC0000000, "MSB OR bit 30"},
    {0x00000001, 0x80000000, 0x80000001, "LSB OR MSB"},

    // Powers of 2 OR
    {0x00000001, 0x00000004, 0x00000005, "2^0 OR 2^2"},
    {0x00000010, 0x00000100, 0x00000110, "2^4 OR 2^8"},
    {0x00001000, 0x00010000, 0x00011000, "2^12 OR 2^16"},
    {0x80000000, 0x00000001, 0x80000001, "2^31 OR 2^0"},

    // Mask operations (bit setting)
    {0x12340000, 0x0000FFFF, 0x1234FFFF, "Set lower 16 bits"},
    {0x00005678, 0xFFFF0000, 0xFFFF5678, "Set upper 16 bits"},
    {0x12045678, 0x00FF00FF, 0x12FF56FF, "Set alternating bytes"},
    {0x12345678, 0x0F0F0F0F, 0x1F3F5F7F, "Set alternating nibbles"},

    // Sequential bit patterns
    {0x00000001, 0x00000002, 0x00000003, "1 OR 2 = 3"},
    {0x00000003, 0x00000002, 0x00000003, "3 OR 2 = 3"},
    {0x00000005, 0x00000003, 0x00000007, "5 OR 3 = 7"},
    {0x00000006, 0x00000003, 0x00000007, "6 OR 3 = 7"},

    // Large number OR
    {0x12345678, 0x9ABCDEF0, 0x9ABCDEF8, "Large number OR pattern"},
    {0x87654321, 0xFEDCBA98, 0xFFFDFBB9, "Large number OR pattern 2"},
    {0xDEADBEEF, 0xCAFEBABE, 0xDEFFBEFF, "Hex patterns OR"},

    // Edge cases
    {0x00000000, 0x80000000, 0x80000000, "0 OR min signed"},
    {0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, "Max positive OR min signed"},
    {0x00000001, 0xFFFFFFFE, 0xFFFFFFFF, "1 OR (-2 in 2s complement)"},
    {0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, "All 1s OR 1"},
};

// RISC-V AND (Bitwise AND) Test Cases
// Format: rs1, rs2, expected_result, description

struct ANDTestCase {
  uint32_t rs1;
  uint32_t rs2;
  uint32_t expected;
  const char* description;
};

ANDTestCase and_tests[] = {
    // Basic AND operations
    {0x00000000, 0x00000000, 0x00000000, "0 AND 0 = 0"},
    {0x00000001, 0x00000000, 0x00000000, "1 AND 0 = 0"},
    {0x00000000, 0x00000001, 0x00000000, "0 AND 1 = 0"},
    {0x00000001, 0x00000001, 0x00000001, "1 AND 1 = 1"},

    // AND with same value (idempotent)
    {0x12345678, 0x12345678, 0x12345678, "Same value AND = same value"},
    {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, "All 1s AND all 1s = all 1s"},
    {0x80000000, 0x80000000, 0x80000000, "MSB AND MSB = MSB"},
    {0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, "Max positive AND itself = same"},

    // AND with 0 (annihilation)
    {0x12345678, 0x00000000, 0x00000000, "Value AND 0 = 0"},
    {0xFFFFFFFF, 0x00000000, 0x00000000, "All 1s AND 0 = 0"},
    {0x80000000, 0x00000000, 0x00000000, "MSB AND 0 = 0"},
    {0x00000000, 0x12345678, 0x00000000, "0 AND value = 0"},

    // AND with all 1s (identity)
    {0x00000000, 0xFFFFFFFF, 0x00000000, "0 AND all 1s = 0"},
    {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, "All 1s AND all 1s = all 1s"},
    {0x12345678, 0xFFFFFFFF, 0x12345678, "Value AND all 1s = value"},
    {0x80000000, 0xFFFFFFFF, 0x80000000, "MSB AND all 1s = MSB"},
    {0x7FFFFFFF, 0xFFFFFFFF, 0x7FFFFFFF, "Max positive AND all 1s = same"},

    // Bit masking patterns
    {0x12345678, 0x0000FFFF, 0x00005678, "Mask to lower 16 bits"},
    {0x12345678, 0xFFFF0000, 0x12340000, "Mask to upper 16 bits"},
    {0x12345678, 0x00FF00FF, 0x00340078, "Mask alternating bytes"},
    {0x12345678, 0x0F0F0F0F, 0x02040608, "Mask alternating nibbles"},

    // Single bit operations
    {0x00000001, 0x00000002, 0x00000000, "Bit 0 AND bit 1 = 0"},
    {0x00000001, 0x00000003, 0x00000001, "Bit 0 AND (bit 0 + bit 1)"},
    {0x80000000, 0x40000000, 0x00000000, "MSB AND bit 30 = 0"},
    {0x00000001, 0x80000000, 0x00000000, "LSB AND MSB = 0"},

    // Powers of 2 AND
    {0x00000001, 0x00000004, 0x00000000, "2^0 AND 2^2 = 0"},
    {0x00000010, 0x00000100, 0x00000000, "2^4 AND 2^8 = 0"},
    {0x00001000, 0x00010000, 0x00000000, "2^12 AND 2^16 = 0"},
    {0x80000000, 0x00000001, 0x00000000, "2^31 AND 2^0 = 0"},

    // Bit extraction patterns
    {0x12345678, 0x000000FF, 0x00000078, "Extract lowest byte"},
    {0x12345678, 0x0000FF00, 0x00005600, "Extract second byte"},
    {0x12345678, 0x00FF0000, 0x00340000, "Extract third byte"},
    {0x12345678, 0xFF000000, 0x12000000, "Extract highest byte"},

    // Sequential bit patterns
    {0x00000001, 0x00000002, 0x00000000, "1 AND 2 = 0"},
    {0x00000003, 0x00000002, 0x00000002, "3 AND 2 = 2"},
    {0x00000005, 0x00000003, 0x00000001, "5 AND 3 = 1"},
    {0x00000006, 0x00000003, 0x00000002, "6 AND 3 = 2"},

    // Large number AND
    {0x12345678, 0x9ABCDEF0, 0x12345670, "Large number AND pattern"},
    {0x87654321, 0xFEDCBA98, 0x86440200, "Large number AND pattern 2"},
    {0xDEADBEEF, 0xCAFEBABE, 0xCAACBAAE, "Hex patterns AND"},

    // Edge cases
    {0x00000000, 0x80000000, 0x00000000, "0 AND min signed"},
    {0x7FFFFFFF, 0x80000000, 0x00000000, "Max positive AND min signed"},
    {0x00000001, 0xFFFFFFFE, 0x00000000, "1 AND (-2 in 2s complement)"},
    {0xFFFFFFFF, 0x00000001, 0x00000001, "All 1s AND 1"},

    // Complementary patterns
    {0xAAAAAAAA, 0x55555555, 0x00000000, "Alternating bits AND = 0"},
    {0x55555555, 0xAAAAAAAA, 0x00000000, "Alternating bits AND = 0"},
    {0xF0F0F0F0, 0x0F0F0F0F, 0x00000000, "Nibble pattern AND = 0"},
    {0xFFFF0000, 0x0000FFFF, 0x00000000, "Upper/lower 16 bits AND = 0"},
};

void run_or_tests() {
  int num_tests = sizeof(or_tests) / sizeof(or_tests[0]);
  for (int i = 0; i < num_tests; i++) {
    uint32_t result = or_tests[i].rs1 | or_tests[i].rs2;
    bool passed = (result == or_tests[i].expected);
    printf("OR Test %d: %s - %s\n", i + 1, or_tests[i].description, passed ? "PASS" : "FAIL");
    if (!passed) {
      printf("  Expected: 0x%08X, Got: 0x%08X\n", or_tests[i].expected, result);
    }
  }
}

void run_and_tests() {
  int num_tests = sizeof(and_tests) / sizeof(and_tests[0]);
  for (int i = 0; i < num_tests; i++) {
    uint32_t result = and_tests[i].rs1 & and_tests[i].rs2;
    bool passed = (result == and_tests[i].expected);
    printf("AND Test %d: %s - %s\n", i + 1, and_tests[i].description, passed ? "PASS" : "FAIL");
    if (!passed) {
      printf("  Expected: 0x%08X, Got: 0x%08X\n", and_tests[i].expected, result);
    }
  }
}

void run_xor_tests() {
  int num_tests = sizeof(xor_tests) / sizeof(xor_tests[0]);
  for (int i = 0; i < num_tests; i++) {
    uint32_t result = xor_tests[i].rs1 ^ xor_tests[i].rs2;
    bool passed = (result == xor_tests[i].expected);
    printf("XOR Test %d: %s - %s\n", i + 1, xor_tests[i].description, passed ? "PASS" : "FAIL");
    if (!passed) {
      printf("  Expected: 0x%08X, Got: 0x%08X\n", xor_tests[i].expected, result);
    }
  }
}

void run_sltu_tests() {
  int num_tests = sizeof(sltu_tests) / sizeof(sltu_tests[0]);
  for (int i = 0; i < num_tests; i++) {
    uint32_t result = (sltu_tests[i].rs1 < sltu_tests[i].rs2) ? 1 : 0;
    bool passed = (result == sltu_tests[i].expected);
    printf("SLTU Test %d: %s - %s\n", i + 1, sltu_tests[i].description, passed ? "PASS" : "FAIL");
    if (!passed) {
      printf("  Expected: 0x%08X, Got: 0x%08X\n", sltu_tests[i].expected, result);
    }
  }
}

// Test runner example
void run_sll_tests() {
  int num_tests = sizeof(sll_tests) / sizeof(sll_tests[0]);
  for (int i = 0; i < num_tests; i++) {
    uint32_t result = sll_tests[i].rs1 << (sll_tests[i].rs2 & 0x1F);
    bool passed = (result == sll_tests[i].expected);
    printf("SLL Test %d: %s - %s\n", i + 1, sll_tests[i].description, passed ? "PASS" : "FAIL");
    if (!passed) {
      printf("  Expected: 0x%08X, Got: 0x%08X\n", sll_tests[i].expected, result);
    }
  }
}

void run_slt_tests() {
  int num_tests = sizeof(slt_tests) / sizeof(slt_tests[0]);
  for (int i = 0; i < num_tests; i++) {
    uint32_t result = ((int32_t)slt_tests[i].rs1 < (int32_t)slt_tests[i].rs2) ? 1 : 0;
    bool passed = (result == slt_tests[i].expected);
    printf("SLT Test %d: %s - %s\n", i + 1, slt_tests[i].description, passed ? "PASS" : "FAIL");
    if (!passed) {
      printf("  Expected: 0x%08X, Got: 0x%08X\n", slt_tests[i].expected, result);
    }
  }
}

struct ADDTestCase {
  uint32_t rs1;
  uint32_t rs2;
  uint32_t expected;
  const char* description;
};

ADDTestCase add_tests[] = {
    // Basic addition
    {0x00000000, 0x00000000, 0x00000000, "0 + 0 = 0"},
    {0x00000001, 0x00000000, 0x00000001, "1 + 0 = 1"},
    {0x00000000, 0x00000001, 0x00000001, "0 + 1 = 1"},
    {0x00000001, 0x00000001, 0x00000002, "1 + 1 = 2"},
    {0x00000005, 0x00000003, 0x00000008, "5 + 3 = 8"},
    {0x0000000A, 0x0000000F, 0x00000019, "10 + 15 = 25"},

    // Addition with zero (identity)
    {0x12345678, 0x00000000, 0x12345678, "Value + 0 = value"},
    {0x00000000, 0x12345678, 0x12345678, "0 + value = value"},
    {0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, "Max value + 0 = max value"},
    {0x80000000, 0x00000000, 0x80000000, "Min signed + 0 = min signed"},

    // Powers of 2 addition
    {0x00000001, 0x00000001, 0x00000002, "2^0 + 2^0 = 2^1"},
    {0x00000002, 0x00000002, 0x00000004, "2^1 + 2^1 = 2^2"},
    {0x00000004, 0x00000004, 0x00000008, "2^2 + 2^2 = 2^3"},
    {0x00000008, 0x00000008, 0x00000010, "2^3 + 2^3 = 2^4"},
    {0x40000000, 0x40000000, 0x80000000, "2^30 + 2^30 = 2^31"},

    // Overflow cases (32-bit wraparound)
    {0xFFFFFFFF, 0x00000001, 0x00000000, "Max + 1 = 0 (overflow)"},
    {0x80000000, 0x80000000, 0x00000000, "Min signed + min signed = 0 (overflow)"},
    {0x7FFFFFFF, 0x00000001, 0x80000000, "Max positive + 1 = min signed (overflow)"},
    {0x7FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFE, "Max positive + max positive (overflow)"},
    {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE, "Max + max = max-1 (overflow)"},

    // Large number addition
    {0x12345678, 0x9ABCDEF0, 0xACF13568, "Large number addition"},
    {0x87654321, 0xFEDCBA98, 0x8641FDB9, "Large number addition 2 (overflow)"},
    {0xDEADBEEF, 0xCAFEBABE, 0xA9AC79AD, "Hex patterns addition (overflow)"},

    // Signed positive addition
    {0x00000001, 0x00000002, 0x00000003, "1 + 2 = 3"},
    {0x00000064, 0x00000064, 0x000000C8, "100 + 100 = 200"},
    {0x000003E8, 0x000003E8, 0x000007D0, "1000 + 1000 = 2000"},
    {0x00000000, 0x7FFFFFFF, 0x7FFFFFFF, "0 + max positive = max positive"},

    // Adding negative numbers (in 2's complement)
    {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE, "(-1) + (-1) = (-2)"},
    {0xFFFFFFFF, 0xFFFFFFFE, 0xFFFFFFFD, "(-1) + (-2) = (-3)"},
    {0x80000000, 0xFFFFFFFF, 0x7FFFFFFF, "(-2147483648) + (-1) = 2147483647 (overflow)"},
    {0x80000001, 0xFFFFFFFF, 0x80000000, "(-2147483647) + (-1) = (-2147483648)"},

    // Mixed positive/negative
    {0x00000001, 0xFFFFFFFF, 0x00000000, "1 + (-1) = 0"},
    {0x00000005, 0xFFFFFFFC, 0x00000001, "5 + (-4) = 1"},
    {0x0000000A, 0xFFFFFFF6, 0x00000000, "10 + (-10) = 0"},
    {0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, "Max positive + min signed = -1"},

    // Carry propagation
    {0x000000FF, 0x00000001, 0x00000100, "255 + 1 = 256 (carry)"},
    {0x0000FFFF, 0x00000001, 0x00010000, "65535 + 1 = 65536 (carry)"},
    {0x00FFFFFF, 0x00000001, 0x01000000, "16777215 + 1 = 16777216 (carry)"},
    {0x7FFFFFFF, 0x00000001, 0x80000000, "Max positive + 1 (carry into sign bit)"},

    // Sequential additions
    {0x00000000, 0x00000001, 0x00000001, "0 + 1"},
    {0x00000001, 0x00000001, 0x00000002, "1 + 1"},
    {0x00000002, 0x00000001, 0x00000003, "2 + 1"},
    {0x00000003, 0x00000001, 0x00000004, "3 + 1"},

    // Edge cases
    {0x00000001, 0x80000000, 0x80000001, "1 + min signed"},
    {0x80000000, 0x00000001, 0x80000001, "Min signed + 1"},
    {0x7FFFFFFE, 0x00000001, 0x7FFFFFFF, "Max positive - 1 + 1"},
    {0x00000000, 0x80000000, 0x80000000, "0 + min signed"},

    // Nibble and byte boundaries
    {0x0000000F, 0x00000001, 0x00000010, "15 + 1 = 16 (nibble carry)"},
    {0x000000FF, 0x00000001, 0x00000100, "255 + 1 = 256 (byte carry)"},
    {0x00000FFF, 0x00000001, 0x00001000, "4095 + 1 = 4096 (3 nibble carry)"},
    {0x0000FFFF, 0x00000001, 0x00010000, "65535 + 1 = 65536 (2 byte carry)"},
};

// RISC-V SUB (Subtraction) Test Cases
// Format: rs1, rs2, expected_result, description

struct SUBTestCase {
  uint32_t rs1;
  uint32_t rs2;
  uint32_t expected;
  const char* description;
};

SUBTestCase sub_tests[] = {
    // Basic subtraction
    {0x00000000, 0x00000000, 0x00000000, "0 - 0 = 0"},
    {0x00000001, 0x00000000, 0x00000001, "1 - 0 = 1"},
    {0x00000001, 0x00000001, 0x00000000, "1 - 1 = 0"},
    {0x00000005, 0x00000003, 0x00000002, "5 - 3 = 2"},
    {0x0000000A, 0x00000005, 0x00000005, "10 - 5 = 5"},
    {0x00000019, 0x0000000F, 0x0000000A, "25 - 15 = 10"},

    // Subtraction with zero
    {0x12345678, 0x00000000, 0x12345678, "Value - 0 = value"},
    {0x00000000, 0x12345678, 0xEDCBA988, "0 - value = -value (2's complement)"},
    {0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, "Max value - 0 = max value"},
    {0x80000000, 0x00000000, 0x80000000, "Min signed - 0 = min signed"},

    // Self subtraction
    {0x12345678, 0x12345678, 0x00000000, "Value - value = 0"},
    {0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, "Max - max = 0"},
    {0x80000000, 0x80000000, 0x00000000, "Min signed - min signed = 0"},
    {0x7FFFFFFF, 0x7FFFFFFF, 0x00000000, "Max positive - max positive = 0"},

    // Powers of 2 subtraction
    {0x00000002, 0x00000001, 0x00000001, "2^1 - 2^0 = 2^0"},
    {0x00000004, 0x00000002, 0x00000002, "2^2 - 2^1 = 2^1"},
    {0x00000008, 0x00000004, 0x00000004, "2^3 - 2^2 = 2^2"},
    {0x00000010, 0x00000008, 0x00000008, "2^4 - 2^3 = 2^3"},
    {0x80000000, 0x40000000, 0x40000000, "2^31 - 2^30 = 2^30"},

    // Underflow cases (32-bit wraparound)
    {0x00000000, 0x00000001, 0xFFFFFFFF, "0 - 1 = -1 (underflow)"},
    {0x00000001, 0x00000002, 0xFFFFFFFF, "1 - 2 = -1 (underflow)"},
    {0x80000000, 0x00000001, 0x7FFFFFFF, "Min signed - 1 = max positive (underflow)"},
    {0x00000000, 0x80000000, 0x80000000, "0 - min signed = min signed (overflow)"},
    {0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, "Max positive - min signed = -1 (overflow)"},

    // Large number subtraction
    {0x9ABCDEF0, 0x12345678, 0x88888878, "Large number subtraction"},
    {0xFEDCBA98, 0x87654321, 0x77777777, "Large number subtraction 2"},
    {0xDEADBEEF, 0xCAFEBABE, 0x13AF0431, "Hex patterns subtraction"},

    // Signed positive subtraction
    {0x00000003, 0x00000002, 0x00000001, "3 - 2 = 1"},
    {0x000000C8, 0x00000064, 0x00000064, "200 - 100 = 100"},
    {0x000007D0, 0x000003E8, 0x000003E8, "2000 - 1000 = 1000"},
    {0x7FFFFFFF, 0x00000000, 0x7FFFFFFF, "Max positive - 0 = max positive"},

    // Subtracting negative numbers (in 2's complement)
    {0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, "(-1) - (-1) = 0"},
    {0xFFFFFFFF, 0xFFFFFFFE, 0x00000001, "(-1) - (-2) = 1"},
    {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, "(-2) - (-1) = (-1)"},
    {0x80000000, 0xFFFFFFFF, 0x80000001, "(-2147483648) - (-1) = (-2147483647)"},

    // Mixed positive/negative subtraction
    {0x00000001, 0xFFFFFFFF, 0x00000002, "1 - (-1) = 2"},
    {0x00000005, 0xFFFFFFFC, 0x00000009, "5 - (-4) = 9"},
    {0x0000000A, 0xFFFFFFF6, 0x00000014, "10 - (-10) = 20"},
    {0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, "Max positive - min signed = -1"},

    // Borrow propagation
    {0x00000100, 0x00000001, 0x000000FF, "256 - 1 = 255 (borrow)"},
    {0x00010000, 0x00000001, 0x0000FFFF, "65536 - 1 = 65535 (borrow)"},
    {0x01000000, 0x00000001, 0x00FFFFFF, "16777216 - 1 = 16777215 (borrow)"},
    {0x80000000, 0x00000001, 0x7FFFFFFF, "Min signed - 1 (borrow from sign bit)"},

    // Sequential subtractions
    {0x00000004, 0x00000001, 0x00000003, "4 - 1"},
    {0x00000003, 0x00000001, 0x00000002, "3 - 1"},
    {0x00000002, 0x00000001, 0x00000001, "2 - 1"},
    {0x00000001, 0x00000001, 0x00000000, "1 - 1"},

    // Edge cases
    {0x80000001, 0x00000001, 0x80000000, "Min signed + 1 - 1"},
    {0x7FFFFFFF, 0x00000001, 0x7FFFFFFE, "Max positive - 1"},
    {0x00000000, 0x7FFFFFFF, 0x80000001, "0 - max positive"},
    {0x00000001, 0x80000000, 0x80000001, "1 - min signed"},

    // Nibble and byte boundaries
    {0x00000010, 0x00000001, 0x0000000F, "16 - 1 = 15 (nibble borrow)"},
    {0x00000100, 0x00000001, 0x000000FF, "256 - 1 = 255 (byte borrow)"},
    {0x00001000, 0x00000001, 0x00000FFF, "4096 - 1 = 4095 (3 nibble borrow)"},
    {0x00010000, 0x00000001, 0x0000FFFF, "65536 - 1 = 65535 (2 byte borrow)"},

    // Two's complement negation equivalence
    {0x00000000, 0x00000001, 0xFFFFFFFF, "0 - 1 = ~1 + 1 = -1"},
    {0x00000000, 0x00000005, 0xFFFFFFFB, "0 - 5 = ~5 + 1 = -5"},
    {0x00000000, 0x7FFFFFFF, 0x80000001, "0 - max positive = ~max positive + 1"},
    {0x00000000, 0x80000000, 0x80000000, "0 - min signed = min signed (special case)"},
};

void run_add_tests() {
  int num_tests = sizeof(add_tests) / sizeof(add_tests[0]);
  for (int i = 0; i < num_tests; i++) {
    uint32_t result = add_tests[i].rs1 + add_tests[i].rs2;
    bool passed = (result == add_tests[i].expected);
    printf("ADD Test %d: %s - %s\n", i + 1, add_tests[i].description, passed ? "PASS" : "FAIL");
    if (!passed) {
      printf("  Expected: 0x%08X, Got: 0x%08X\n", add_tests[i].expected, result);
    }
  }
}

void run_sub_tests() {
  int num_tests = sizeof(sub_tests) / sizeof(sub_tests[0]);
  for (int i = 0; i < num_tests; i++) {
    uint32_t result = sub_tests[i].rs1 - sub_tests[i].rs2;
    bool passed = (result == sub_tests[i].expected);
    printf("SUB Test %d: %s - %s\n", i + 1, sub_tests[i].description, passed ? "PASS" : "FAIL");
    if (!passed) {
      printf("  Expected: 0x%08X, Got: 0x%08X\n", sub_tests[i].expected, result);
    }
  }
}
// RISC-V SRA (Shift Right Arithmetic) Test Cases
// Format: rs1, rs2, expected_result, description

struct SRATestCase {
  uint32_t rs1;
  uint32_t rs2;
  uint32_t expected;
  const char* description;
};

SRATestCase sra_tests[] = {
    // Basic shift right arithmetic with positive numbers
    {0x00000000, 0x00000000, 0x00000000, "0 >>> 0 = 0"},
    {0x00000001, 0x00000000, 0x00000001, "1 >>> 0 = 1"},
    {0x00000001, 0x00000001, 0x00000000, "1 >>> 1 = 0"},
    {0x00000002, 0x00000001, 0x00000001, "2 >>> 1 = 1"},
    {0x00000004, 0x00000001, 0x00000002, "4 >>> 1 = 2"},
    {0x00000008, 0x00000001, 0x00000004, "8 >>> 1 = 4"},
    {0x00000010, 0x00000001, 0x00000008, "16 >>> 1 = 8"},

    // Multiple bit shifts with positive numbers
    {0x00000004, 0x00000002, 0x00000001, "4 >>> 2 = 1"},
    {0x00000008, 0x00000003, 0x00000001, "8 >>> 3 = 1"},
    {0x00000010, 0x00000004, 0x00000001, "16 >>> 4 = 1"},
    {0x00000020, 0x00000005, 0x00000001, "32 >>> 5 = 1"},
    {0x00000040, 0x00000006, 0x00000001, "64 >>> 6 = 1"},
    {0x00000080, 0x00000007, 0x00000001, "128 >>> 7 = 1"},

    // Positive numbers with large shifts
    {0x12345678, 0x00000001, 0x091A2B3C, "0x12345678 >>> 1"},
    {0x12345678, 0x00000002, 0x048D159E, "0x12345678 >>> 2"},
    {0x12345678, 0x00000004, 0x01234567, "0x12345678 >>> 4"},
    {0x12345678, 0x00000008, 0x00123456, "0x12345678 >>> 8"},
    {0x12345678, 0x00000010, 0x00001234, "0x12345678 >>> 16"},

    // Negative numbers (sign extension)
    {0x80000000, 0x00000001, 0xC0000000, "MIN_INT >>> 1 = sign extended"},
    {0x80000000, 0x00000002, 0xE0000000, "MIN_INT >>> 2 = sign extended"},
    {0x80000000, 0x00000004, 0xF8000000, "MIN_INT >>> 4 = sign extended"},
    {0x80000000, 0x00000008, 0xFF800000, "MIN_INT >>> 8 = sign extended"},
    {0x80000000, 0x00000010, 0xFFFF8000, "MIN_INT >>> 16 = sign extended"},
    {0x80000000, 0x0000001F, 0xFFFFFFFF, "MIN_INT >>> 31 = all 1s"},

    // Negative numbers with various patterns
    {0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, "(-1) >>> 1 = (-1)"},
    {0xFFFFFFFF, 0x00000002, 0xFFFFFFFF, "(-1) >>> 2 = (-1)"},
    {0xFFFFFFFF, 0x0000001F, 0xFFFFFFFF, "(-1) >>> 31 = (-1)"},
    {0xFFFFFFFE, 0x00000001, 0xFFFFFFFF, "(-2) >>> 1 = (-1)"},
    {0xFFFFFFFC, 0x00000001, 0xFFFFFFFE, "(-4) >>> 1 = (-2)"},
    {0xFFFFFFF8, 0x00000001, 0xFFFFFFFC, "(-8) >>> 1 = (-4)"},

    // Edge cases with negative numbers
    {0x80000001, 0x00000001, 0xC0000000, "(-2147483647) >>> 1"},
    {0x80000001, 0x0000001F, 0xFFFFFFFF, "(-2147483647) >>> 31"},
    {0x87654321, 0x00000001, 0xC3B2A190, "Negative pattern >>> 1"},
    {0x87654321, 0x00000004, 0xF8765432, "Negative pattern >>> 4"},
    {0x87654321, 0x00000008, 0xFF876543, "Negative pattern >>> 8"},

    // Shift amount modulo 32 (only lower 5 bits matter)
    {0x12345678, 0x00000020, 0x12345678, "Shift by 32 = shift by 0"},
    {0x12345678, 0x00000021, 0x091A2B3C, "Shift by 33 = shift by 1"},
    {0x12345678, 0x00000022, 0x048D159E, "Shift by 34 = shift by 2"},
    {0x80000000, 0x00000020, 0x80000000, "Negative shift by 32 = shift by 0"},
    {0x80000000, 0x00000021, 0xC0000000, "Negative shift by 33 = shift by 1"},
    {0xFFFFFFFF, 0x00000020, 0xFFFFFFFF, "(-1) shift by 32 = shift by 0"},

    // High shift amounts (testing 5-bit masking)
    {0x12345678, 0x000000A1, 0x091A2B3C, "Shift by 0xA1 = shift by 1"},
    {0x12345678, 0x000000E4, 0x01234567, "Shift by 0xE4 = shift by 4"},
    {0x80000000, 0x000000FF, 0xFFFFFFFF, "Negative shift by 0xFF = shift by 31"},
    {0x87654321, 0x00000088, 0xFF876543, "Negative shift by 0x88 = shift by 8"},

    // Powers of 2 with arithmetic shift
    {0x00000001, 0x00000001, 0x00000000, "1 >>> 1 = 0"},
    {0x00000002, 0x00000001, 0x00000001, "2 >>> 1 = 1"},
    {0x00000004, 0x00000001, 0x00000002, "4 >>> 1 = 2"},
    {0x00000008, 0x00000001, 0x00000004, "8 >>> 1 = 4"},
    {0x80000000, 0x00000001, 0xC0000000, "Negative power of 2 >>> 1"},

    // Max positive number
    {0x7FFFFFFF, 0x00000001, 0x3FFFFFFF, "MAX_INT >>> 1"},
    {0x7FFFFFFF, 0x00000002, 0x1FFFFFFF, "MAX_INT >>> 2"},
    {0x7FFFFFFF, 0x00000004, 0x07FFFFFF, "MAX_INT >>> 4"},
    {0x7FFFFFFF, 0x00000008, 0x007FFFFF, "MAX_INT >>> 8"},
    {0x7FFFFFFF, 0x0000001E, 0x00000001, "MAX_INT >>> 30"},
    {0x7FFFFFFF, 0x0000001F, 0x00000000, "MAX_INT >>> 31"},

    // Zero with various shift amounts
    {0x00000000, 0x00000001, 0x00000000, "0 >>> 1 = 0"},
    {0x00000000, 0x0000000F, 0x00000000, "0 >>> 15 = 0"},
    {0x00000000, 0x0000001F, 0x00000000, "0 >>> 31 = 0"},

    // Special bit patterns
    {0x55555555, 0x00000001, 0x2AAAAAAA, "Alternating 1s >>> 1"},
    {0xAAAAAAAA, 0x00000001, 0xD5555555, "Alternating 1s (negative) >>> 1"},
    {0x0F0F0F0F, 0x00000004, 0x00F0F0F0, "Nibble pattern >>> 4"},
    {0xF0F0F0F0, 0x00000004, 0xFF0F0F0F, "Nibble pattern (negative) >>> 4"},
};

// RISC-V SRL (Shift Right Logical) Test Cases
// Format: rs1, rs2, expected_result, description

struct SRLTestCase {
  uint32_t rs1;
  uint32_t rs2;
  uint32_t expected;
  const char* description;
};

SRLTestCase srl_tests[] = {
    // Basic shift right logical
    {0x00000000, 0x00000000, 0x00000000, "0 >> 0 = 0"},
    {0x00000001, 0x00000000, 0x00000001, "1 >> 0 = 1"},
    {0x00000001, 0x00000001, 0x00000000, "1 >> 1 = 0"},
    {0x00000002, 0x00000001, 0x00000001, "2 >> 1 = 1"},
    {0x00000004, 0x00000001, 0x00000002, "4 >> 1 = 2"},
    {0x00000008, 0x00000001, 0x00000004, "8 >> 1 = 4"},
    {0x00000010, 0x00000001, 0x00000008, "16 >> 1 = 8"},

    // Multiple bit shifts
    {0x00000004, 0x00000002, 0x00000001, "4 >> 2 = 1"},
    {0x00000008, 0x00000003, 0x00000001, "8 >> 3 = 1"},
    {0x00000010, 0x00000004, 0x00000001, "16 >> 4 = 1"},
    {0x00000020, 0x00000005, 0x00000001, "32 >> 5 = 1"},
    {0x00000040, 0x00000006, 0x00000001, "64 >> 6 = 1"},
    {0x00000080, 0x00000007, 0x00000001, "128 >> 7 = 1"},

    // Positive numbers with large shifts
    {0x12345678, 0x00000001, 0x091A2B3C, "0x12345678 >> 1"},
    {0x12345678, 0x00000002, 0x048D159E, "0x12345678 >> 2"},
    {0x12345678, 0x00000004, 0x01234567, "0x12345678 >> 4"},
    {0x12345678, 0x00000008, 0x00123456, "0x12345678 >> 8"},
    {0x12345678, 0x00000010, 0x00001234, "0x12345678 >> 16"},

    // Negative numbers (NO sign extension - zeros fill from left)
    {0x80000000, 0x00000001, 0x40000000, "MIN_INT >> 1 = 0x40000000"},
    {0x80000000, 0x00000002, 0x20000000, "MIN_INT >> 2 = 0x20000000"},
    {0x80000000, 0x00000004, 0x08000000, "MIN_INT >> 4 = 0x08000000"},
    {0x80000000, 0x00000008, 0x00800000, "MIN_INT >> 8 = 0x00800000"},
    {0x80000000, 0x00000010, 0x00008000, "MIN_INT >> 16 = 0x00008000"},
    {0x80000000, 0x0000001F, 0x00000001, "MIN_INT >> 31 = 1"},

    // Negative numbers with various patterns
    {0xFFFFFFFF, 0x00000001, 0x7FFFFFFF, "(-1) >> 1 = 0x7FFFFFFF"},
    {0xFFFFFFFF, 0x00000002, 0x3FFFFFFF, "(-1) >> 2 = 0x3FFFFFFF"},
    {0xFFFFFFFF, 0x0000001F, 0x00000001, "(-1) >> 31 = 1"},
    {0xFFFFFFFE, 0x00000001, 0x7FFFFFFF, "(-2) >> 1 = 0x7FFFFFFF"},
    {0xFFFFFFFC, 0x00000001, 0x7FFFFFFE, "(-4) >> 1 = 0x7FFFFFFE"},
    {0xFFFFFFF8, 0x00000001, 0x7FFFFFFC, "(-8) >> 1 = 0x7FFFFFFC"},

    // Edge cases with negative numbers
    {0x80000001, 0x00000001, 0x40000000, "(-2147483647) >> 1"},
    {0x80000001, 0x0000001F, 0x00000001, "(-2147483647) >> 31"},
    {0x87654321, 0x00000001, 0x43B2A190, "Negative pattern >> 1"},
    {0x87654321, 0x00000004, 0x08765432, "Negative pattern >> 4"},
    {0x87654321, 0x00000008, 0x00876543, "Negative pattern >> 8"},

    // Shift amount modulo 32 (only lower 5 bits matter)
    {0x12345678, 0x00000020, 0x12345678, "Shift by 32 = shift by 0"},
    {0x12345678, 0x00000021, 0x091A2B3C, "Shift by 33 = shift by 1"},
    {0x12345678, 0x00000022, 0x048D159E, "Shift by 34 = shift by 2"},
    {0x80000000, 0x00000020, 0x80000000, "Negative shift by 32 = shift by 0"},
    {0x80000000, 0x00000021, 0x40000000, "Negative shift by 33 = shift by 1"},
    {0xFFFFFFFF, 0x00000020, 0xFFFFFFFF, "(-1) shift by 32 = shift by 0"},

    // High shift amounts (testing 5-bit masking)
    {0x12345678, 0x000000A1, 0x091A2B3C, "Shift by 0xA1 = shift by 1"},
    {0x12345678, 0x000000E4, 0x01234567, "Shift by 0xE4 = shift by 4"},
    {0x80000000, 0x000000FF, 0x00000001, "Negative shift by 0xFF = shift by 31"},
    {0x87654321, 0x00000088, 0x00876543, "Negative shift by 0x88 = shift by 8"},

    // Powers of 2 with logical shift
    {0x00000001, 0x00000001, 0x00000000, "1 >> 1 = 0"},
    {0x00000002, 0x00000001, 0x00000001, "2 >> 1 = 1"},
    {0x00000004, 0x00000001, 0x00000002, "4 >> 1 = 2"},
    {0x00000008, 0x00000001, 0x00000004, "8 >> 1 = 4"},
    {0x80000000, 0x00000001, 0x40000000, "Negative power of 2 >> 1"},

    // Max positive number
    {0x7FFFFFFF, 0x00000001, 0x3FFFFFFF, "MAX_INT >> 1"},
    {0x7FFFFFFF, 0x00000002, 0x1FFFFFFF, "MAX_INT >> 2"},
    {0x7FFFFFFF, 0x00000004, 0x07FFFFFF, "MAX_INT >> 4"},
    {0x7FFFFFFF, 0x00000008, 0x007FFFFF, "MAX_INT >> 8"},
    {0x7FFFFFFF, 0x0000001E, 0x00000001, "MAX_INT >> 30"},
    {0x7FFFFFFF, 0x0000001F, 0x00000000, "MAX_INT >> 31"},

    // Zero with various shift amounts
    {0x00000000, 0x00000001, 0x00000000, "0 >> 1 = 0"},
    {0x00000000, 0x0000000F, 0x00000000, "0 >> 15 = 0"},
    {0x00000000, 0x0000001F, 0x00000000, "0 >> 31 = 0"},

    // Special bit patterns
    {0x55555555, 0x00000001, 0x2AAAAAAA, "Alternating 1s >> 1"},
    {0xAAAAAAAA, 0x00000001, 0x55555555, "Alternating 1s (negative) >> 1"},
    {0x0F0F0F0F, 0x00000004, 0x00F0F0F0, "Nibble pattern >> 4"},
    {0xF0F0F0F0, 0x00000004, 0x0F0F0F0F, "Nibble pattern (negative) >> 4"},

    // All bits set patterns
    {0xFFFFFFFF, 0x00000001, 0x7FFFFFFF, "All 1s >> 1"},
    {0xFFFFFFFF, 0x00000008, 0x00FFFFFF, "All 1s >> 8"},
    {0xFFFFFFFF, 0x00000010, 0x0000FFFF, "All 1s >> 16"},
    {0xFFFFFFFF, 0x00000018, 0x000000FF, "All 1s >> 24"},
    {0xFFFFFFFF, 0x0000001F, 0x00000001, "All 1s >> 31"},

    // Byte boundaries
    {0xFF000000, 0x00000008, 0x00FF0000, "Byte in MSB >> 8"},
    {0x00FF0000, 0x00000008, 0x0000FF00, "Byte in 2nd >> 8"},
    {0x0000FF00, 0x00000008, 0x000000FF, "Byte in 3rd >> 8"},
    {0x000000FF, 0x00000008, 0x00000000, "Byte in LSB >> 8"},
};

void run_sra_tests() {
  int num_tests = sizeof(sra_tests) / sizeof(sra_tests[0]);
  for (int i = 0; i < num_tests; i++) {
    // For SRA, we need to perform arithmetic right shift
    // In C, right shift of signed integers is implementation-defined
    // but typically arithmetic. For unsigned, it's always logical.
    // We'll cast to signed for arithmetic shift
    int32_t rs1_signed = (int32_t)sra_tests[i].rs1;
    int32_t shift_amount = (int32_t)sra_tests[i].rs2 & 0x1F;  // Only lower 5 bits
    uint32_t result = (uint32_t)(rs1_signed >> shift_amount);

    bool passed = (result == sra_tests[i].expected);
    printf("SRA Test %d: %s - %s\n", i + 1, sra_tests[i].description, passed ? "PASS" : "FAIL");
    if (!passed) {
      printf("  Expected: 0x%08X, Got: 0x%08X\n", sra_tests[i].expected, result);
    }
  }
}

void run_srl_tests() {
  int num_tests = sizeof(srl_tests) / sizeof(srl_tests[0]);
  for (int i = 0; i < num_tests; i++) {
    // For SRL, we perform logical right shift (always fill with zeros)
    uint32_t shift_amount = srl_tests[i].rs2 & 0x1F;  // Only lower 5 bits
    uint32_t result = srl_tests[i].rs1 >> shift_amount;

    bool passed = (result == srl_tests[i].expected);
    printf("SRL Test %d: %s - %s\n", i + 1, srl_tests[i].description, passed ? "PASS" : "FAIL");
    if (!passed) {
      printf("  Expected: 0x%08X, Got: 0x%08X\n", srl_tests[i].expected, result);
    }
  }
}

void test_all() {
  run_or_tests();
  run_and_tests();
  run_xor_tests();
  run_sll_tests();
  run_sltu_tests();
  run_slt_tests();
  run_add_tests();
  run_sub_tests();
  run_sra_tests();
  run_srl_tests();
}