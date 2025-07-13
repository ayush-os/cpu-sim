#include "../include/cpusim/CPU.h"

#include <iostream>

const int NUM_REGS = 32;
const int MEM_SIZE = 16 * 1024 * 1024;  // 1024 bytes = 1 kb -> 1024 KB = 1 MB -> 16 * 1 MB = 16 MB

const uint8_t OPCODE_MASK = 0x7f;
const uint8_t FUNCT7_SHIFT = 0x19;
const uint8_t FUNCT7_MASK = 0x7f;
const uint8_t FUNCT3_SHIFT = 0xC;
const uint8_t FUNCT3_MASK = 0x7;

const uint8_t RTYPE_OPCODE = 0x33;
const uint8_t ITYPE_OPCODE1 = 0x13;
const uint8_t ITYPE_OPCODE2 = 0x3;
const uint8_t STYPE_OPCODE = 0x23;
const uint8_t UTYPE_OPCODE1 = 0x37;
const uint8_t UTYPE_OPCODE2 = 0x17;

const uint8_t REG_MASK = 0x1F;

const uint8_t CONST_SHIFT_1 = 0x14;
const uint8_t CONST_SHIFT_2 = 0xC;
const uint8_t RD_SHIFT = 7;
const uint8_t RS1_SHIFT = 15;
const uint8_t RS2_SHIFT = 20;

const uint8_t LB_MASK = 0xFF;
const int LH_MASK = 0xFFFF;

using ExecFunc = void (CPU::*)(const Instr&);

std::unordered_map<uint32_t, ExecFunc> instruction_map = {
    // R-type
    {0x33000000, &CPU::execADD},
    {0x33000020, &CPU::execSUB},
    {0x33001000, &CPU::execSLL},
    {0x33002000, &CPU::execSLT},
    {0x33005000, &CPU::execSRL},
    {0x33005020, &CPU::execSRA},
    {0x33003000, &CPU::execSLTU},
    {0x33004000, &CPU::execXOR},
    {0x33006000, &CPU::execOR},
    {0x33007000, &CPU::execAND},

    // I-type
    {0x13000000, &CPU::execADDI},
    {0x13001000, &CPU::execSLLI},
    {0x13002000, &CPU::execSLTI},
    {0x13003000, &CPU::execSLTIU},
    {0x13004000, &CPU::execXORI},
    {0x13005000, &CPU::execSRLAI},
    {0x13006000, &CPU::execORI},
    {0x13007000, &CPU::execANDI},
    {0x03004000, &CPU::execLBU},
    {0x03005000, &CPU::execLHU},

    // I-Type
    {0x03000000, &CPU::execLB},
    {0x03001000, &CPU::execLH},
    {0x03002000, &CPU::execLW},

    // S-Type
    {0x23000000, &CPU::execSB},
    {0x23001000, &CPU::execSH},
    {0x23002000, &CPU::execSW},

    // B-Type
    {0x63000000, &CPU::execBEQ},
    {0x63001000, &CPU::execBNE},
    {0x63004000, &CPU::execBLT},
    {0x63005000, &CPU::execBGE},
    {0x63006000, &CPU::execBLTU},
    {0x63007000, &CPU::execBGEU},

    // U-type
    {0x37000000, &CPU::execLUI},
    {0x17000000, &CPU::execAUIPC},

    // J-type
    {0x6F000000, &CPU::execJAL},
    {0x67000000, &CPU::execJALR},

    // SYS
    {0x73000000, &CPU::execE},
};

CPU::CPU()
    : pc(0),
      regs(std::make_unique<uint32_t[]>(NUM_REGS)),
      mem(std::make_unique<unsigned char[]>(MEM_SIZE)) {
  mem.get()[3] = 0x00;
  mem.get()[2] = 0x00;
  mem.get()[1] = 0x82;
  mem.get()[0] = 0x83;

  regs[1] = 0x00100004;

  // mem.get()[0x400803] = 0xFF;
  // mem.get()[0x400802] = 0xFF;
  // mem.get()[0x400801] = 0xFF;
  mem.get()[0x00100004] = 0x80;
}

uint32_t makeInstrKey(const uint32_t& instr) {
  uint8_t opcode = instr & OPCODE_MASK;
  uint8_t funct3 = (instr >> FUNCT3_SHIFT) & FUNCT3_MASK;
  uint8_t funct7 = (instr >> FUNCT7_SHIFT) & FUNCT7_MASK;

  if (opcode == RTYPE_OPCODE) {
    return (opcode << 24) | (funct3 << 12) | (funct7 << 0);
  } else {
    return (opcode << 24) | (funct3 << 12);
  }
}

Instr extractFields(const uint32_t& instr) {
  Instr i;
  i.rd = (instr >> RD_SHIFT) & REG_MASK;
  i.rs1 = (instr >> RS1_SHIFT) & REG_MASK;
  i.rs2 = (instr >> RS2_SHIFT) & REG_MASK;
  i.funct3 = (instr >> FUNCT3_SHIFT) & FUNCT3_MASK;
  i.funct7 = (instr >> FUNCT7_SHIFT) & FUNCT7_MASK;

  uint8_t opcode = instr & OPCODE_MASK;

  if (opcode == ITYPE_OPCODE1 || opcode == ITYPE_OPCODE2) {
    i.imm = static_cast<int32_t>(instr) >> CONST_SHIFT_1;
  } else if (opcode == UTYPE_OPCODE1 || opcode == UTYPE_OPCODE2) {
    i.imm = static_cast<int32_t>(instr) >> CONST_SHIFT_2;
  }

  return i;
}

void CPU::decode(const uint32_t& instr) {
  Instr i = extractFields(instr);
  uint32_t key = makeInstrKey(instr);

  auto it = instruction_map.find(key);
  if (it != instruction_map.end()) {
    std::invoke(it->second, *this, i);
  } else {
    std::cerr << "Unknown instruction: 0x" << std::hex << instr << std::endl;
  }
}

void CPU::execSLL(const Instr& instr) {
  regs[instr.rd] = regs[instr.rs1] << (regs[instr.rs2] & REG_MASK);
}

void CPU::execSLT(const Instr& instr) {
  regs[instr.rd]
      = (static_cast<int32_t>(regs[instr.rs1]) < static_cast<int32_t>(regs[instr.rs2])) ? 1 : 0;
}

void CPU::execSLTU(const Instr& instr) {
  regs[instr.rd] = (regs[instr.rs1] < regs[instr.rs2]) ? 1 : 0;
}

void CPU::execXOR(const Instr& instr) { regs[instr.rd] = regs[instr.rs1] ^ regs[instr.rs2]; }

void CPU::execOR(const Instr& instr) { regs[instr.rd] = regs[instr.rs1] | regs[instr.rs2]; }

void CPU::execAND(const Instr& instr) { regs[instr.rd] = regs[instr.rs1] & regs[instr.rs2]; }

void CPU::execSRA(const Instr& instr) {
  regs[instr.rd] = static_cast<uint32_t>(static_cast<int32_t>(regs[instr.rs1])
                                         >> static_cast<int32_t>(regs[instr.rs2] & REG_MASK));
}

void CPU::execSRL(const Instr& instr) {
  regs[instr.rd] = regs[instr.rs1] >> (regs[instr.rs2] & REG_MASK);
}
void CPU::execADD(const Instr& instr) { regs[instr.rd] = regs[instr.rs1] + regs[instr.rs2]; }

void CPU::execSUB(const Instr& instr) { regs[instr.rd] = regs[instr.rs1] - regs[instr.rs2]; }

void CPU::execADDI(const Instr& instr) { regs[instr.rd] = regs[instr.rs1] + instr.imm; }

void CPU::execXORI(const Instr& instr) { regs[instr.rd] = regs[instr.rs1] ^ instr.imm; }

void CPU::execORI(const Instr& instr) { regs[instr.rd] = regs[instr.rs1] | instr.imm; }

void CPU::execANDI(const Instr& instr) { regs[instr.rd] = regs[instr.rs1] & instr.imm; }

void CPU::execSLLI(const Instr& instr) { regs[instr.rd] = regs[instr.rs1] << instr.imm; }

void CPU::execSLTI(const Instr& instr) {
  regs[instr.rd]
      = (static_cast<int32_t>(regs[instr.rs1]) < static_cast<int32_t>(instr.imm)) ? 1 : 0;
}

void CPU::execSLTIU(const Instr& instr) {
  regs[instr.rd]
      = (static_cast<uint32_t>(regs[instr.rs1]) < static_cast<uint32_t>(instr.imm)) ? 1 : 0;
}

void CPU::execSRLAI(const Instr& instr) { (instr.funct7) ? execSRAI(instr) : execSRLI(instr); }

void CPU::execSRLI(const Instr& instr) {
  regs[instr.rd] = regs[instr.rs1] >> (instr.imm & REG_MASK);
}

void CPU::execSRAI(const Instr& instr) {
  regs[instr.rd] = static_cast<int32_t>(regs[instr.rs1]) >> (instr.imm & REG_MASK);
}

// BUGGY AND UNTESTED
void CPU::execLB(const Instr& instr) {
  uint32_t addr = regs[instr.rs1] + instr.imm;
  regs[instr.rd]
      = (int32_t)((int16_t)(*reinterpret_cast<const uint32_t*>(mem.get() + addr) & LB_MASK));
  std::cout << std::hex << regs[instr.rd] << std::endl;
}

// BUGGY AND UNTESTED
void CPU::execLH(const Instr& instr) {
  uint32_t addr = regs[instr.rs1] + instr.imm;
  regs[instr.rd]
      = (int32_t)((int16_t)(*reinterpret_cast<const uint32_t*>(mem.get() + addr) & LH_MASK));

  std::cout << std::hex << regs[instr.rd] << std::endl;
}

void CPU::execLW(const Instr& instr) {
  uint32_t addr = regs[instr.rs1] + instr.imm;
  regs[instr.rd] = *reinterpret_cast<const uint32_t*>(mem.get() + addr);
}

void CPU::execLBU(const Instr& instr) { std::cout << "LBU" << std::endl; }
void CPU::execLHU(const Instr& instr) { std::cout << "LHU" << std::endl; }

void CPU::execSB(const Instr& instr) { std::cout << "SB" << std::endl; }
void CPU::execSH(const Instr& instr) { std::cout << "SH" << std::endl; }
void CPU::execSW(const Instr& instr) { std::cout << "SW" << std::endl; }

void CPU::execBEQ(const Instr& instr) { std::cout << "BEQ" << std::endl; }
void CPU::execBNE(const Instr& instr) { std::cout << "BNE" << std::endl; }
void CPU::execBLT(const Instr& instr) { std::cout << "BLT" << std::endl; }
void CPU::execBGE(const Instr& instr) { std::cout << "BGE" << std::endl; }
void CPU::execBLTU(const Instr& instr) { std::cout << "BLTU" << std::endl; }
void CPU::execBGEU(const Instr& instr) { std::cout << "BGEU" << std::endl; }

void CPU::execLUI(const Instr& instr) { regs[instr.rd] = instr.imm << 12; }
void CPU::execAUIPC(const Instr& instr) { regs[instr.rd] = pc + (instr.imm << 12); }

void CPU::execJAL(const Instr& instr) { std::cout << "JAL" << std::endl; }

// WHEN YOU GET TO THIS, IMMEDIATE WILL BE OFF
void CPU::execJALR(const Instr& instr) { std::cout << "JALR" << std::endl; }

void CPU::execE(const Instr& instr) { (instr.rs2) ? execEBREAK(instr) : execECALL(instr); }
void CPU::execECALL(const Instr& instr) { std::cout << "ECALL" << std::endl; }
void CPU::execEBREAK(const Instr& instr) { std::cout << "EBREAK" << std::endl; }