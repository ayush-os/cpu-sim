#pragma once
#include <cstdint>
#include <memory>

const int NUM_BITS = 32;

struct Instr {
  uint8_t rs1;
  uint8_t rs2;
  uint8_t rd;
  uint8_t funct7;
  uint8_t funct3;
  int32_t imm;
  int32_t offset;
};

class CPU {
  // TODO update accessibility of fns and vars
public:
  uint32_t pc;
  // TODO: Block write ops on x0
  std::unique_ptr<uint32_t[]> regs;
  std::unique_ptr<unsigned char[]> mem;  // 16 MB: 0x000000 to 0xFFFFFF

  explicit CPU();
  inline uint32_t fetch(const uint32_t& pc) {
    return *reinterpret_cast<const uint32_t*>(mem.get() + pc);
  };

  void decode(const uint32_t& instr);
  // TODO maybe turn these into a namespace?
  void decodeR(const uint32_t& instr);
  void decodeI1(const uint32_t& instr);
  void decodeI2(const uint32_t& instr);
  void decodeI3(const uint32_t& instr);
  void decodeS(const uint32_t& instr);
  void decodeB(const uint32_t& instr);
  void decodeU1(const uint32_t& instr);
  void decodeU2(const uint32_t& instr);
  void decodeJ(const uint32_t& instr);
  void decodeSYS(const uint32_t& instr);

  void execSLL(const Instr& instr);
  void execSLT(const Instr& instr);
  void execSLTU(const Instr& instr);
  void execXOR(const Instr& instr);
  void execSRA(const Instr& instr);
  void execOR(const Instr& instr);
  void execAND(const Instr& instr);
  void execADD(const Instr& instr);
  void execSRL(const Instr& instr);
  void execSUB(const Instr& instr);

  void execADDI(const Instr& instr);
  void execSLLI(const Instr& instr);
  void execSLTI(const Instr& instr);
  void execXORI(const Instr& instr);
  void execORI(const Instr& instr);
  void execSRLI(const Instr& instr);
  void execSRAI(const Instr& instr);
  void execSLTIU(const Instr& instr);
  void execANDI(const Instr& instr);

  void execLB(const Instr& instr);
  void execLH(const Instr& instr);
  void execLW(const Instr& instr);
  void execLBU(const Instr& instr);
  void execLHU(const Instr& instr);

  void decodeSB(const uint32_t& instr);
  void decodeSH(const uint32_t& instr);
  void decodeSW(const uint32_t& instr);

  void decodeBEQ(const uint32_t& instr);
  void decodeBNE(const uint32_t& instr);
  void decodeBLT(const uint32_t& instr);
  void decodeBGE(const uint32_t& instr);
  void decodeBLTU(const uint32_t& instr);
  void decodeBGEU(const uint32_t& instr);

  void execLUI(const Instr& instr);
  void execAUIPC(const Instr& instr);

  void decodeJAL(const uint32_t& instr);
  void decodeJALR(const uint32_t& instr);

  void decodeECALL(const uint32_t& instr);
  void decodeEBREAK(const uint32_t& instr);
};