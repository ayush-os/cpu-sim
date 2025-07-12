#include "../include/cpusim/CPU.h"

#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "tests.cpp"

const int NUM_REGS = 32;
const int MEM_SIZE = 16 * 1024 * 1024;  // 1024 bytes = 1 kb -> 1024 KB = 1 MB -> 16 * 1 MB = 16 MB

const uint8_t OPCODE_MASK = 0x7f;
const uint8_t FUNC7_SHIFT = 0x19;
const uint8_t RS_SHIFT = 0x1B;
const uint8_t FUNC3_SHIFT = 0xC;
const uint8_t FUNC3_MASK = 0x7;
const int SYS_MASK = 0x00100000;

const uint8_t LB_MASK = 0xFF;
const int LH_MASK = 0xFFFF;

const uint8_t R_RD_MASK = 0x1F;
const uint8_t R_RS1_MASK = 0x1F;
const uint8_t R_RS2_MASK = 0x1F;
const uint8_t LOWER_5_BITS_MASK = 0x1F;

const uint8_t R_RD_SHIFT = 7;
const uint8_t R_RS1_SHIFT = 15;
const uint8_t R_RS2_SHIFT = 20;

const uint8_t OFFSET_SHIFT = 0x14;
const uint8_t IMM_SHIFT = 0x14;
const uint8_t U_IMM_SHIFT = 0xC;

enum class InstrType { R, I1, I2, I3, S, B, U1, U2, J, SYS };
enum class RType { add, sub, sll, slt, sltu, xor_, srl, sra, or_, and_ };
enum class I1Type { addi, slti, sltiu, xori, ori, andi, slli, srli, srai };
enum class I2Type { lb, lh, lw, lbu, lhu };
enum class SType { sb, sh, sw };
enum class BType { beq, bne, blt, bge, bltu, bgeu };
enum class SYSType { ecall, ebreak };

const std::unordered_map<uint8_t, InstrType> opcode_to_instr_type
    = {{0x33, InstrType::R}, {0x13, InstrType::I1}, {0x3, InstrType::I2},  {0x67, InstrType::I3},
       {0x23, InstrType::S}, {0x63, InstrType::B},  {0x37, InstrType::U1}, {0x17, InstrType::U2},
       {0x6F, InstrType::J}, {0x73, InstrType::SYS}};

const std::unordered_map<uint8_t, std::unordered_set<RType>> funct7_to_instr
    = {{0x0,
        {
            RType::add,
            RType::sll,
            RType::slt,
            RType::sltu,
            RType::xor_,
            RType::srl,
            RType::or_,
            RType::and_,
        }},
       {0x20, {RType::sub, RType::sra}}};

std::unordered_map<uint8_t, std::unordered_set<RType>> r_funct3_to_instr = {
    {0x0, {RType::add, RType::sub}},
    {0x1, {RType::sll}},
    {0x2, {RType::slt}},
    {0x3, {RType::sltu}},
    {0x4, {RType::xor_}},
    {0x5, {RType::sra, RType::srl}},
    {0x6, {RType::or_}},
    {0x7, {RType::and_}},
};

std::unordered_map<uint8_t, std::unordered_set<I1Type>> i_funct3_to_instr_one = {
    {0x0, {I1Type::addi}},  {0x1, {I1Type::slli}}, {0x2, {I1Type::slti}},
    {0x3, {I1Type::sltiu}}, {0x4, {I1Type::xori}}, {0x5, {I1Type::srli, I1Type::srai}},
    {0x6, {I1Type::ori}},   {0x7, {I1Type::andi}},
};

std::unordered_map<uint8_t, I2Type> i_funct3_to_instr_two = {
    {0x0, I2Type::lb}, {0x1, I2Type::lh}, {0x2, I2Type::lw}, {0x4, I2Type::lbu}, {0x5, I2Type::lhu},
};

std::unordered_map<uint8_t, SType> s_funct3_to_instr = {
    {0x0, SType::sb},
    {0x1, SType::sh},
    {0x2, SType::sw},
};

std::unordered_map<uint8_t, BType> b_funct3_to_instr = {
    {0x0, BType::beq}, {0x1, BType::bne},  {0x4, BType::blt},
    {0x5, BType::bge}, {0x6, BType::bltu}, {0x7, BType::bgeu},
};

std::unordered_map<uint8_t, SYSType> sys_to_instr = {
    {0x0, SYSType::ecall},
    {0x1, SYSType::ebreak},
};

CPU::CPU()
    : pc(0),
      regs(std::make_unique<uint32_t[]>(NUM_REGS)),
      mem(std::make_unique<unsigned char[]>(MEM_SIZE)) {
  mem.get()[3] = 0x00;
  mem.get()[2] = 0x00;
  mem.get()[1] = 0x82;
  mem.get()[0] = 0x03;

  // regs[0] = 4;
  regs[1] = 0x00100004;
  mem.get()[0x00100004] = 0x80;

  // test_all();
}

void CPU::decode(const uint32_t& instr) {
  switch (opcode_to_instr_type.at(instr & OPCODE_MASK)) {
    case InstrType::R:
      decodeR(instr);
      break;
    case InstrType::I1:
      decodeI1(instr);
      break;
    case InstrType::I2:
      decodeI2(instr);
      break;
    case InstrType::I3:
      decodeI3(instr);
      break;
    case InstrType::S:
      decodeS(instr);
      break;
    case InstrType::B:
      decodeB(instr);
      break;
    case InstrType::U1:
      decodeU1(instr);
      break;
    case InstrType::U2:
      decodeU2(instr);
      break;
    case InstrType::J:
      decodeJ(instr);
      break;
    case InstrType::SYS:
      decodeSYS(instr);
      break;
    default:
      std::cerr << "Unknown instruction type!\n";
  }
}

void CPU::decodeR(const uint32_t& instr) {
  Instr i;

  i.rd = (instr >> R_RD_SHIFT) & R_RD_MASK;
  i.rs1 = (instr >> R_RS1_SHIFT) & R_RS1_MASK;
  i.rs2 = (instr >> R_RS2_SHIFT) & R_RS2_MASK;
  i.funct7 = (instr >> FUNC7_SHIFT) & OPCODE_MASK;
  i.funct3 = (instr >> FUNC3_SHIFT) & FUNC3_MASK;

  std::unordered_set<RType>& funct3_set = r_funct3_to_instr.at(i.funct3);

  if (funct3_set.size() > 1) {
    for (RType rtype : funct3_set) {
      if (funct7_to_instr.at(i.funct7).count(rtype)) {
        switch (rtype) {
          case RType::sub:
            execSUB(i);
            break;
          case RType::add:
            execADD(i);
            break;
          case RType::sra:
            execSRA(i);
            break;
          case RType::srl:
            execSRL(i);
            break;
          default:
            std::cerr << "ERROR: This branch should not have executed.\n";
        }
      }
    }
  } else {
    switch (*funct3_set.begin()) {
      case RType::sll:
        execSLL(i);
        break;
      case RType::slt:
        execSLT(i);
        break;
      case RType::sltu:
        execSLTU(i);
        break;
      case RType::xor_:
        execXOR(i);
        break;
      case RType::or_:
        execOR(i);
        break;
      case RType::and_:
        execAND(i);
        break;
      default:
        std::cerr << "ERROR: This branch should not have executed.\n";
    }
  }
}

void CPU::decodeI1(const uint32_t& instr) {
  Instr i;

  i.rd = (instr >> R_RD_SHIFT) & R_RD_MASK;
  i.rs1 = (instr >> R_RS1_SHIFT) & R_RS1_MASK;
  i.funct3 = (instr >> FUNC3_SHIFT) & FUNC3_MASK;
  i.imm = (int32_t)(instr) >> IMM_SHIFT;

  std::unordered_set<I1Type>& funct3_set = i_funct3_to_instr_one.at(i.funct3);

  if (funct3_set.size() > 1) {
    if (instr >> RS_SHIFT) {
      execSRAI(i);
    } else {
      execSRLI(i);
    }
  } else {
    switch (*funct3_set.begin()) {
      case I1Type::addi:
        execADDI(i);
        break;
      case I1Type::slli:
        execSLLI(i);
        break;
      case I1Type::slti:
        execSLTI(i);
        break;
      case I1Type::xori:
        execXORI(i);
        break;
      case I1Type::ori:
        execORI(i);
        break;
      case I1Type::sltiu:
        execSLTIU(i);
        break;
      case I1Type::andi:
        execANDI(i);
        break;
      default:
        std::cerr << "ERROR: This branch should not have executed.\n";
    }
  }
}

void CPU::decodeI2(const uint32_t& instr) {
  Instr i;

  i.rd = (instr >> R_RD_SHIFT) & R_RD_MASK;
  i.rs1 = (instr >> R_RS1_SHIFT) & R_RS1_MASK;
  i.offset = static_cast<int32_t>(static_cast<int32_t>(instr) >> static_cast<int8_t>(OFFSET_SHIFT));
  i.funct3 = (instr >> FUNC3_SHIFT) & FUNC3_MASK;

  switch (i_funct3_to_instr_two.at(i.funct3)) {
    case I2Type::lb:
      execLB(i);
      break;
    case I2Type::lh:
      execLH(i);
      break;
    case I2Type::lw:
      execLW(i);
      break;
    case I2Type::lbu:
      execLBU(i);
      break;
    case I2Type::lhu:
      execLHU(i);
      break;
    default:
      std::cerr << "ERROR: This branch should not have executed.\n";
  }
}

void CPU::decodeS(const uint32_t& instr) {
  uint8_t funct3 = (instr >> FUNC3_SHIFT) & FUNC3_MASK;

  switch (s_funct3_to_instr.at(funct3)) {
    case SType::sb:
      decodeSB(instr);
      break;
    case SType::sh:
      decodeSH(instr);
      break;
    case SType::sw:
      decodeSW(instr);
      break;
    default:
      std::cerr << "ERROR: This branch should not have executed.\n";
  }
}

void CPU::decodeB(const uint32_t& instr) {
  uint8_t funct3 = (instr >> FUNC3_SHIFT) & FUNC3_MASK;

  switch (b_funct3_to_instr.at(funct3)) {
    case BType::beq:
      decodeBEQ(instr);
      break;
    case BType::bne:
      decodeBNE(instr);
      break;
    case BType::blt:
      decodeBLT(instr);
      break;
    case BType::bge:
      decodeBGE(instr);
      break;
    case BType::bltu:
      decodeBLTU(instr);
      break;
    case BType::bgeu:
      decodeBGEU(instr);
      break;
    default:
      std::cerr << "ERROR: This branch should not have executed.\n";
  }
}

void CPU::decodeSYS(const uint32_t& instr) {
  (instr & SYS_MASK) ? decodeEBREAK(instr) : decodeECALL(instr);
}

void CPU::decodeI3(const uint32_t& instr) { decodeJALR(instr); }

void CPU::decodeU1(const uint32_t& instr) {
  Instr i;

  i.rd = (instr >> R_RD_SHIFT) & R_RD_MASK;
  i.imm = static_cast<int32_t>(instr) >> U_IMM_SHIFT;

  execLUI(i);
}

void CPU::decodeU2(const uint32_t& instr) {
  Instr i;

  i.rd = (instr >> R_RD_SHIFT) & R_RD_MASK;
  i.imm = (int32_t)(instr) >> U_IMM_SHIFT;

  execAUIPC(i);
}

void CPU::decodeJ(const uint32_t& instr) { decodeJAL(instr); }

// TODO MAKE SURE ALL EXEC INSTRUCTIONS TO UPDATE PC
void CPU::execSLL(const Instr& instr) {
  regs[instr.rd] = regs[instr.rs1] << (regs[instr.rs2] & R_RD_MASK);
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
                                         >> static_cast<int32_t>(regs[instr.rs2] & R_RD_MASK));
}
void CPU::execSRL(const Instr& instr) {
  regs[instr.rd] = regs[instr.rs1] >> (regs[instr.rs2] & R_RD_MASK);
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
void CPU::execSRLI(const Instr& instr) {
  regs[instr.rd] = regs[instr.rs1] >> (instr.imm & LOWER_5_BITS_MASK);
}
void CPU::execSRAI(const Instr& instr) {
  regs[instr.rd] = static_cast<int32_t>(regs[instr.rs1]) >> (instr.imm & LOWER_5_BITS_MASK);
}

void CPU::execLB(const Instr& instr) {
  // std::cout << std::hex << regs[instr.rs1] << std::endl;
  // std::cout << std::hex << instr.offset << std::endl;
  // std::cout << std::hex << regs[instr.rs1] + instr.offset << std::endl;
  // std::cout << std::hex << static_cast<int32_t>(mem[regs[instr.rs1] + instr.offset]) <<
  // std::endl; std::cout << std::hex << (mem[regs[instr.rs1] + instr.offset] & LB_MASK) <<
  // std::endl;

  regs[instr.rd] = static_cast<int32_t>(
      static_cast<int32_t>(mem[static_cast<int32_t>(regs[instr.rs1]) + instr.offset])
      & static_cast<int32_t>(LB_MASK));
  std::cout << std::hex << static_cast<int32_t>(regs[instr.rd]) << std::endl;
}

void CPU::execLH(const Instr& instr) {
  regs[instr.rd] = static_cast<uint32_t>(mem[regs[instr.rs1] + instr.offset] & LH_MASK);
  std::cout << regs[instr.rd] << std::endl;
}

void CPU::execLW(const Instr& instr) {
  regs[instr.rd] = static_cast<uint32_t>(mem[regs[instr.rs1] + instr.offset]);
  std::cout << regs[instr.rd] << std::endl;
}

void CPU::execLBU(const Instr& instr) { std::cout << "LBU" << std::endl; }
void CPU::execLHU(const Instr& instr) { std::cout << "LHU" << std::endl; }

void CPU::decodeSB(const uint32_t& instr) { std::cout << "SB" << std::endl; }
void CPU::decodeSH(const uint32_t& instr) { std::cout << "SH" << std::endl; }
void CPU::decodeSW(const uint32_t& instr) { std::cout << "SW" << std::endl; }

void CPU::decodeBEQ(const uint32_t& instr) { std::cout << "BEQ" << std::endl; }
void CPU::decodeBNE(const uint32_t& instr) { std::cout << "BNE" << std::endl; }
void CPU::decodeBLT(const uint32_t& instr) { std::cout << "BLT" << std::endl; }
void CPU::decodeBGE(const uint32_t& instr) { std::cout << "BGE" << std::endl; }
void CPU::decodeBLTU(const uint32_t& instr) { std::cout << "BLTU" << std::endl; }
void CPU::decodeBGEU(const uint32_t& instr) { std::cout << "BGEU" << std::endl; }

void CPU::execLUI(const Instr& instr) { regs[instr.rd] = instr.imm << 12; }
void CPU::execAUIPC(const Instr& instr) { regs[instr.rd] = pc + (instr.imm << 12); }

void CPU::decodeJAL(const uint32_t& instr) { std::cout << "JAL" << std::endl; }
void CPU::decodeJALR(const uint32_t& instr) { std::cout << "JALR" << std::endl; }

void CPU::decodeECALL(const uint32_t& instr) { std::cout << "ECALL" << std::endl; }
void CPU::decodeEBREAK(const uint32_t& instr) { std::cout << "EBREAK" << std::endl; }