#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <cstdarg>
#include <cstdint>
#include <string>
#include <vector>

#include "simulator/memory_manager.h"

const int REGNUM = 32;

enum Inst {
  LUI = 0,
  AUIPC = 1,
  JAL = 2,
  JALR = 3,
  BEQ = 4,
  BNE = 5,
  BLT = 6,
  BGE = 7,
  BLTU = 8,
  BGEU = 9,
  LB = 10,
  LH = 11,
  LW = 12,
  LD = 13,
  LBU = 14,
  LHU = 15,
  SB = 16,
  SH = 17,
  SW = 18,
  SD = 19,
  ADDI = 20,
  SLTI = 21,
  SLTIU = 22,
  XORI = 23,
  ORI = 24,
  ANDI = 25,
  SLLI = 26,
  SRLI = 27,
  SRAI = 28,
  ADD = 29,
  SUB = 30,
  SLL = 31,
  SLT = 32,
  SLTU = 33,
  XOR = 34,
  SRL = 35,
  SRA = 36,
  OR = 37,
  AND = 38,
  ECALL = 39,
  ADDIW = 40,
  MUL = 41,
  MULH = 42,
  DIV = 43,
  REM = 44,
  LWU = 45,
  SLLIW = 46,
  SRLIW = 47,
  SRAIW = 48,
  ADDW = 49,
  SUBW = 50,
  SLLW = 51,
  SRLW = 52,
  SRAW = 53,
  UNKNOWN = -1,
};

// Opcode field
const int OP_REG = 0x33;
const int OP_IMM = 0x13;
const int OP_LUI = 0x37;
const int OP_BRANCH = 0x63;
const int OP_STORE = 0x23;
const int OP_LOAD = 0x03;
const int OP_SYSTEM = 0x73;
const int OP_AUIPC = 0x17;
const int OP_JAL = 0x6F;
const int OP_JALR = 0x67;
const int OP_IMM32 = 0x1B;
const int OP_32 = 0x3B;

inline bool isBranch(Inst inst) {
  if (inst == BEQ || inst == BNE || inst == BLT || inst == BGE ||
      inst == BLTU || inst == BGEU) {
    return true;
  }
  return false;
}

inline bool isJump(Inst inst) {
  if (inst == JAL || inst == JALR) {
    return true;
  }
  return false;
}

inline bool isReadMem(Inst inst) {
  if (inst == LB || inst == LH || inst == LW || inst == LD || inst == LBU ||
      inst == LHU || inst == LWU) {
    return true;
  }
  return false;
}


class Simulator {
public:
  bool use_cache;
  uint64_t pc;
  uint64_t reg[32];
  uint32_t stackBase;
  uint32_t maximumStackSize;
  MemoryManager *memory;

  Simulator(MemoryManager *memory);
  ~Simulator();

  void initStack(uint32_t baseaddr, uint32_t maxSize);

  void simulate();

  void print_history();
  void read_char(int64_t& oprand1);
  void read_num(int64_t& oprand1);

private:
  struct FReg {
    bool bubble;
    uint32_t stall;

    uint64_t pc;
    uint32_t inst;
    uint32_t len;
  } fReg, fRegNew;
  struct DReg {
    bool bubble;
    uint32_t stall;
    uint32_t rs1, rs2;

    uint64_t pc;
    Inst inst;
    int64_t op1;
    int64_t op2;
    uint32_t dest;
    int64_t offset;
    bool predictedBranch;
    uint64_t predictedPC; 
    uint64_t anotherPC; 
  } dReg, dRegNew;
  struct EReg {
    bool bubble;
    uint32_t stall;

    uint64_t pc;
    Inst inst;
    int64_t op1;
    int64_t op2;
    bool writeReg;
    uint32_t destReg;
    int64_t out;
    bool writeMem;
    bool readMem;
    bool readSignExt;
    uint32_t memLen;
    bool branch;
  } eReg, eRegNew;
  struct MReg {
    bool bubble;
    uint32_t stall;

    uint64_t pc;
    Inst inst;
    int64_t op1;
    int64_t op2;
    int64_t out;
    bool writeReg;
    uint32_t destReg;
  } mReg, mRegNew;

  bool executeWriteBack;
  uint32_t executeWBReg;
  bool memoryWriteBack;
  uint32_t memoryWBReg;

  struct History {
    uint32_t instCount;
    uint32_t cycleCount;
    uint32_t stalledCycleCount;
  } history;

  void fetch();
  void decode();
  void excecute();
  void memoryAccess();
  void writeBack();

  int64_t handleSystemCall(int64_t op1, int64_t op2);
};

#endif