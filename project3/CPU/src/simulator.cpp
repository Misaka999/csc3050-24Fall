#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include "simulator/simulator.h"

Simulator::Simulator(MemoryManager *memory) {
  this->memory = memory;
  this->pc = 0;
  for (int i = 0; i < REGNUM; ++i) {
    this->reg[i] = 0;
  }
}

Simulator::~Simulator() {}

void Simulator::initStack(uint32_t baseaddr, uint32_t maxSize) {
  this->reg[2] = baseaddr;
  this->stackBase = baseaddr;
  this->maximumStackSize = maxSize;
  for (uint32_t addr = baseaddr; addr > baseaddr - maxSize; addr--) {
    if (!this->memory->isPageExist(addr)) {
      this->memory->addPage(addr);
    }
    this->memory->setByte(addr, 0);
  }
}

void Simulator::simulate() {
  // Initialize pipeline registers
  memset(&this->fReg, 0, sizeof(this->fReg));
  memset(&this->fRegNew, 0, sizeof(this->fRegNew));
  memset(&this->dReg, 0, sizeof(this->dReg));
  memset(&this->dRegNew, 0, sizeof(this->dReg));
  memset(&this->eReg, 0, sizeof(this->eReg));
  memset(&this->eRegNew, 0, sizeof(this->eRegNew));
  memset(&this->mReg, 0, sizeof(this->mReg));
  memset(&this->mRegNew, 0, sizeof(this->mRegNew));

  // Insert Bubble to later pipeline stages
  fReg.bubble = true;
  dReg.bubble = true;
  eReg.bubble = true;
  mReg.bubble = true;

  // Main Simulation Loop
  while (true) {
    this->executeWriteBack = false;
    this->executeWBReg = -1;
    this->memoryWriteBack = false;
    this->memoryWBReg = -1;

    // THE EXECUTION ORDER of these functions are important!!!
    // Changing them will introduce strange bugs
    this->fetch();
    this->decode();
    this->excecute();
    this->memoryAccess();
    this->writeBack();

    if (!this->fReg.stall) this->fReg = this->fRegNew;
    else this->fReg.stall--;
    if (!this->dReg.stall) this->dReg = this->dRegNew;
    else this->dReg.stall--;
    this->eReg = this->eRegNew;
    this->mReg = this->mRegNew;
    memset(&this->fRegNew, 0, sizeof(this->fRegNew));
    memset(&this->dRegNew, 0, sizeof(this->dRegNew));
    memset(&this->eRegNew, 0, sizeof(this->eRegNew));
    memset(&this->mRegNew, 0, sizeof(this->mRegNew));

    if (!this->dReg.bubble && !this->dReg.stall && !this->fReg.stall && this->dReg.predictedBranch) {
      this->pc = this->dReg.predictedPC;
    }

    this->history.cycleCount++;
  }
}

void Simulator::fetch() {
  uint32_t inst = this->memory->getInt(this->pc);
  uint32_t len = 4;

  this->fRegNew.bubble = false;
  this->fRegNew.stall = false;
  this->fRegNew.inst = inst;
  this->fRegNew.len = len;
  this->fRegNew.pc = this->pc;
  this->pc = this->pc + len;
}

void Simulator::decode() {
  if (this->fReg.stall) {
    this->pc = this->pc - 4;
    return;
  }
  if (this->fReg.bubble || this->fReg.inst == 0) {
    this->dRegNew.bubble = true;
    return;
  }

  Inst insttype;
  uint32_t inst = this->fReg.inst;
  int64_t op1 = 0, op2 = 0, offset = 0; // op1, op2 and offset are values
  uint32_t dest = 0, reg1 = -1, reg2 = -1; // reg1 and reg2 are operands

  // Reg for 32bit instructions
  if (this->fReg.len == 4) // 32 bit instruction
  {
    uint32_t opcode = inst & 0x7F;
    uint32_t funct3 = (inst >> 12) & 0x7;
    uint32_t funct7 = (inst >> 25) & 0x7F;
    uint32_t rd = (inst >> 7) & 0x1F;
    uint32_t rs1 = (inst >> 15) & 0x1F;
    uint32_t rs2 = (inst >> 20) & 0x1F;
    int32_t imm_i = int32_t(inst) >> 20;
    int32_t imm_s =
        int32_t(((inst >> 7) & 0x1F) | ((inst >> 20) & 0xFE0)) << 20 >> 20;
    int32_t imm_sb = int32_t(((inst >> 7) & 0x1E) | ((inst >> 20) & 0x7E0) |
                             ((inst << 4) & 0x800) | ((inst >> 19) & 0x1000))
                         << 19 >>
                     19;
    int32_t imm_u = int32_t(inst) >> 12;
    int32_t imm_uj = int32_t(((inst >> 21) & 0x3FF) | ((inst >> 10) & 0x400) |
                             ((inst >> 1) & 0x7F800) | ((inst >> 12) & 0x80000))
                         << 12 >>
                     11;

    switch (opcode) {
    case OP_REG:
      op1 = this->reg[rs1];
      op2 = this->reg[rs2];
      reg1 = rs1;
      reg2 = rs2;
      dest = rd;
      switch (funct3) {
      case 0x0: // add, mul, sub
        if (funct7 == 0x00) {
          insttype = ADD;
        } else if (funct7 == 0x01) {
          insttype = MUL;
        } else if (funct7 == 0x20) {
          insttype = SUB;
        } else {
           fprintf(stderr,"Unknown funct7 0x%x for funct3 0x%x\n", funct7, funct3);
         exit(-1);
          
        }
        break;
      case 0x1: // sll, mulh
        if (funct7 == 0x00) {
          insttype = SLL;
        } else if (funct7 == 0x01) {
          insttype = MULH;
        } else {
           fprintf(stderr,"Unknown funct7 0x%x for funct3 0x%x\n", funct7, funct3);
         exit(-1);
        }
        break;
      case 0x2: // slt
        if (funct7 == 0x00) {
          insttype = SLT;
        } else {
           fprintf(stderr,"Unknown funct7 0x%x for funct3 0x%x\n", funct7, funct3);
         exit(-1);
        }
        break;
      case 0x3: // sltu
        if (funct7 == 0x00)
        {
          insttype = SLTU;
        }
        else
        {
         fprintf(stderr,"Unknown funct7 0x%x for funct3 0x%x\n", funct7, funct3);
         exit(-1);
        }
        break;
      case 0x4: // xor div
        if (funct7 == 0x00) {
          insttype = XOR;
        } else if (funct7 == 0x01) {
          insttype = DIV;
        } else {
          fprintf(stderr,"Unknown funct7 0x%x for funct3 0x%x\n", funct7, funct3);
         exit(-1);
        }
        break;
      case 0x5: // srl, sra
        if (funct7 == 0x00) {
          insttype = SRL;
        } else if (funct7 == 0x20) {
          insttype = SRA;
        } else {
          fprintf(stderr,"Unknown funct7 0x%x for funct3 0x%x\n", funct7, funct3);
          exit(-1);
        }
        break;
      case 0x6: // or, rem
        if (funct7 == 0x00) {
          insttype = OR;
        } else if (funct7 == 0x01) {
          insttype = REM;
        } else {
          fprintf(stderr,"Unknown funct7 0x%x for funct3 0x%x\n", funct7, funct3);
          exit(-1);
        }
        break;
      case 0x7: // and
        if (funct7 == 0x00) {
          insttype = AND;
        } else {
            fprintf(stderr,"Unknown funct7 0x%x for funct3 0x%x\n", funct7, funct3);
         exit(-1);
        }
        break;
      default:
          fprintf(stderr,"Unknown Funct3 field %x\n", funct3);
         exit(-1);
      }
      break;
    case OP_IMM:
      op1 = this->reg[rs1];
      reg1 = rs1;
      op2 = imm_i;
      dest = rd;
      switch (funct3) {
      case 0x0:
        insttype = ADDI;
        break;
      case 0x2:
        insttype = SLTI;
        break;
      case 0x3:
        insttype = SLTIU;
        break;
      case 0x4:
        insttype = XORI;
        break;
      case 0x6:
        insttype = ORI;
        break;
      case 0x7:
        insttype = ANDI;
        break;
      case 0x1:
        insttype = SLLI;
        op2 = op2 & 0x3F;
        break;
      case 0x5:
        if (((inst >> 26) & 0x3F) == 0x0) {
          insttype = SRLI;
          op2 = op2 & 0x3F;
        } else if (((inst >> 26) & 0x3F) == 0x10) {
          insttype = SRAI;
          op2 = op2 & 0x3F;
        } else {
         fprintf(stderr,"Unknown funct7 0x%x for OP_IMM\n", (inst >> 26) & 0x3F);
         exit(-1);
          
        }
        break;
      default:
         fprintf(stderr,"Unknown Funct3 field %x\n", funct3);
         exit(-1);      
      }
      break;
    case OP_LUI:
      op1 = imm_u;
      op2 = 0;
      offset = imm_u;
      dest = rd;
      insttype = LUI;
      break;
    case OP_AUIPC:
      op1 = imm_u;
      op2 = 0;
      offset = imm_u;
      dest = rd;
      insttype = AUIPC;
      break;
    case OP_JAL:
      op1 = imm_uj;
      op2 = 0;
      offset = imm_uj;
      dest = rd;
      insttype = JAL;
      break;
    case OP_JALR:
      op1 = this->reg[rs1];
      reg1 = rs1;
      op2 = imm_i;
      dest = rd;
      insttype = JALR;
      break;
    case OP_BRANCH:
      op1 = this->reg[rs1];
      op2 = this->reg[rs2];
      reg1 = rs1;
      reg2 = rs2;
      offset = imm_sb;
      switch (funct3) {
      case 0x0:
        insttype = BEQ;
        break;
      case 0x1:
        insttype = BNE;
        break;
      case 0x4:
        insttype = BLT;
        break;
      case 0x5:
        insttype = BGE;
        break;
      case 0x6:
        insttype = BLTU;
        break;
      case 0x7:
        insttype = BGEU;
        break;
      default:
         fprintf(stderr,"Unknown funct3 0x%x at OP_BRANCH\n", funct3);
         exit(-1);  
      }
      break;
    case OP_STORE:
      op1 = this->reg[rs1];
      op2 = this->reg[rs2];
      reg1 = rs1;
      reg2 = rs2;
      offset = imm_s;
      switch (funct3) {
      case 0x0:
        insttype = SB;
        break;
      case 0x1:
        insttype = SH;
        break;
      case 0x2:
        insttype = SW;
        break;
      case 0x3:
        insttype = SD;
        break;
      default:
         fprintf(stderr,"Unknown funct3 0x%x for OP_STORE\n", funct3);
         exit(-1);          
      }
      break;
    case OP_LOAD:
      op1 = this->reg[rs1];
      reg1 = rs1;
      op2 = imm_i;
      offset = imm_i;
      dest = rd;
      switch (funct3) {
      case 0x0:
        insttype = LB;
        break;
      case 0x1:
        insttype = LH;
        break;
      case 0x2:
        insttype = LW;
        break;
      case 0x3:
        insttype = LD;
        break;
      case 0x4:
        insttype = LBU;
        break;
      case 0x5:
        insttype = LHU;
        break;
      case 0x6:
        insttype = LWU;
      default:
        fprintf(stderr,"Unknown funct3 0x%x for OP_LOAD\n", funct3);
         exit(-1);   
      }
      break;
    case OP_SYSTEM:
      if (funct3 == 0x0 && funct7 == 0x000) {
        op1 = this->reg[10];
        op2 = this->reg[17];
        reg1 = 10;
        reg2 = 17;
        dest = 10;
        insttype = ECALL;
      } else {
         fprintf(stderr,"Unknown OP_SYSTEM inst with funct3 0x%x and funct7 0x%x\n",
                    funct3, funct7);
         exit(-1);  
      }
      break;
    case OP_IMM32:
      op1 = this->reg[rs1];
      reg1 = rs1;
      op2 = imm_i;
      dest = rd;
      switch (funct3) {
      case 0x0:
        insttype = ADDIW;
        break;
      case 0x1:
        insttype = SLLIW;
        break;
      case 0x5:
        if (((inst >> 25) & 0x7F) == 0x0) {
          insttype = SRLIW;
        } else if (((inst >> 25) & 0x7F) == 0x20) {
          insttype = SRAIW;
        } else {
          fprintf(stderr,"Unknown shift inst type 0x%x\n", ((inst >> 25) & 0x7F));
         exit(-1);
        }
        break;
      default:
        fprintf(stderr,"Unknown funct3 0x%x for OP_ADDIW\n", funct3);
         exit(-1);
      }
      break;
    case OP_32: {
      op1 = this->reg[rs1];
      op2 = this->reg[rs2];
      reg1 = rs1;
      reg2 = rs2;
      dest = rd;

      uint32_t temp = (inst >> 25) & 0x7F; // 32bit funct7 field
      switch (funct3) {
      case 0x0:
        if (temp == 0x0) {
          insttype = ADDW;
        } else if (temp == 0x20) {
          insttype = SUBW;
        } else {
          fprintf(stderr,"Unknown 32bit funct7 0x%x\n", temp);
         exit(-1);
          
        }
        break;
      case 0x1:
        if (temp == 0x0) {
          insttype = SLLW;
        } else {
         fprintf(stderr,"Unknown 32bit funct7 0x%x\n", temp);
         exit(-1);
        }
        break;
      case 0x5:
        if (temp == 0x0) {
          insttype = SRLW;
        } else if (temp == 0x20) {
          insttype = SRAW;
        } else {
          fprintf(stderr,"Unknown 32bit funct7 0x%x\n", temp);
         exit(-1);
        }
        break;
      default:
         fprintf(stderr,"Unknown 32bit funct3 0x%x\n", funct3);
         exit(-1);
      }
    } break;
    default:
         fprintf(stderr,"Unsupported opcode 0x%x!\n", opcode);
         exit(-1);
    }
  } 
  bool predictedBranch = false;
  if (isBranch(insttype)) {
      predictedBranch = true;
      this->dRegNew.predictedPC = this->fReg.pc + offset;
      this->dRegNew.anotherPC = this->fReg.pc + 4;
      this->fRegNew.bubble = true;
  }

  this->dRegNew.stall = false;
  this->dRegNew.bubble = false;
  this->dRegNew.rs1 = reg1;
  this->dRegNew.rs2 = reg2;
  this->dRegNew.pc = this->fReg.pc;
  this->dRegNew.inst = insttype;
  this->dRegNew.predictedBranch = predictedBranch;
  this->dRegNew.dest = dest;
  this->dRegNew.op1 = op1;
  this->dRegNew.op2 = op2;
  this->dRegNew.offset = offset;
}

void Simulator::excecute() {
  if (this->dReg.stall) {
    this->eRegNew.bubble = true;
    return;
  }
  if (this->dReg.bubble) {
    this->eRegNew.bubble = true;
    return;
  }

  this->history.instCount++;

  Inst inst = this->dReg.inst;
  int64_t op1 = this->dReg.op1;
  int64_t op2 = this->dReg.op2;
  int64_t offset = this->dReg.offset;
  bool predictedBranch = this->dReg.predictedBranch;

  uint64_t dRegPC = this->dReg.pc;
  bool writeReg = false;
  uint32_t destReg = this->dReg.dest;
  int64_t out = 0;
  bool writeMem = false;
  bool readMem = false;
  bool readSignExt = false;
  uint32_t memLen = 0;
  bool branch = false;

  switch (inst) {
  case LUI:
    writeReg = true;
    out = offset << 12;
    break;
  case AUIPC:
    writeReg = true;
    out = dRegPC + (offset << 12);
    break;
  case JAL:
    writeReg = true;
    out = dRegPC + 4;
    dRegPC = dRegPC + op1;
    branch = true;
    break;
  case JALR:
    writeReg = true;
    out = dRegPC + 4;
    dRegPC = (op1 + op2) & (~(uint64_t)1);
    branch = true;
    break;
  case BEQ:
    if (op1 == op2) {
      branch = true;
      dRegPC = dRegPC + offset;
    }
    break;
  case BNE:
    if (op1 != op2) {
      branch = true;
      dRegPC = dRegPC + offset;
    }
    break;
  case BLT:
    if (op1 < op2) {
      branch = true;
      dRegPC = dRegPC + offset;
    }
    break;
  case BGE:
    if (op1 >= op2) {
      branch = true;
      dRegPC = dRegPC + offset;
    }
    break;
  case BLTU:
    if ((uint64_t)op1 < (uint64_t)op2) {
      branch = true;
      dRegPC = dRegPC + offset;
    }
    break;
  case BGEU:
    if ((uint64_t)op1 >= (uint64_t)op2) {
      branch = true;
      dRegPC = dRegPC + offset;
    }
    break;
  case LB:
    readMem = true;
    writeReg = true;
    memLen = 1;
    out = op1 + offset;
    readSignExt = true;
    break;
  case LH:
    readMem = true;
    writeReg = true;
    memLen = 2;
    out = op1 + offset;
    readSignExt = true;
    break;
  case LW:
    readMem = true;
    writeReg = true;
    memLen = 4;
    out = op1 + offset;
    readSignExt = true;
    break;
  case LD:
    readMem = true;
    writeReg = true;
    memLen = 8;
    out = op1 + offset;
    readSignExt = true;
    break;
  case LBU:
    readMem = true;
    writeReg = true;
    memLen = 1;
    out = op1 + offset;
    break;
  case LHU:
    readMem = true;
    writeReg = true;
    memLen = 2;
    out = op1 + offset;
    break;
  case LWU:
    readMem = true;
    writeReg = true;
    memLen = 4;
    out = op1 + offset;
    break;
  case SB:
    writeMem = true;
    memLen = 1;
    out = op1 + offset;
    op2 = op2 & 0xFF;
    break;
  case SH:
    writeMem = true;
    memLen = 2;
    out = op1 + offset;
    op2 = op2 & 0xFFFF;
    break;
  case SW:
    writeMem = true;
    memLen = 4;
    out = op1 + offset;
    op2 = op2 & 0xFFFFFFFF;
    break;
  case SD:
    writeMem = true;
    memLen = 8;
    out = op1 + offset;
    break;
  case ADDI:
  case ADD:
    writeReg = true;
    out = op1 + op2;
    break;
  case ADDIW:
  case ADDW:
    writeReg = true;
    out = (int64_t)((int32_t)op1 + (int32_t)op2);
    break;
  case SUB:
    writeReg = true;
    out = op1 - op2;
    break;
  case SUBW:
    writeReg = true;
    out = (int64_t)((int32_t)op1 - (int32_t)op2);
    break;
  case MUL:
    writeReg = true;
    out = op1 * op2;
    break;
  case DIV:
    writeReg = true;
    out = op1 / op2;
    break;
  case SLTI:
  case SLT:
    writeReg = true;
    out = op1 < op2 ? 1 : 0;
    break;
  case SLTIU:
  case SLTU:
    writeReg = true;
    out = (uint64_t)op1 < (uint64_t)op2 ? 1 : 0;
    break;
  case XORI:
  case XOR:
    writeReg = true;
    out = op1 ^ op2;
    break;
  case ORI:
  case OR:
    writeReg = true;
    out = op1 | op2;
    break;
  case ANDI:
  case AND:
    writeReg = true;
    out = op1 & op2;
    break;
  case SLLI:
  case SLL:
    writeReg = true;
    out = op1 << op2;
    break;
  case SLLIW:
  case SLLW:
    writeReg = true;
    out = int64_t(int32_t(op1 << op2));
    break;
    break;
  case SRLI:
  case SRL:
    writeReg = true;
    out = (uint64_t)op1 >> (uint64_t)op2;
    break;
  case SRLIW:
  case SRLW:
    writeReg = true;
    out = uint64_t(uint32_t((uint32_t)op1 >> (uint32_t)op2));
    break;
  case SRAI:
  case SRA:
    writeReg = true;
    out = op1 >> op2;
    break;
  case SRAW:
  case SRAIW:
    writeReg = true;
    out = int64_t(int32_t((int32_t)op1 >> (int32_t)op2));
    break;
  case ECALL:
    out = handleSystemCall(op1, op2);
    writeReg = true;
    break;
  default:
        fprintf(stderr,"Unknown instruction type %d\n", inst);
    exit(-1);
  }

  if (isBranch(inst)) {
    if (predictedBranch =! branch) {
      this->pc = this->dReg.anotherPC;
      this->fRegNew.bubble = true;
      this->dRegNew.bubble = true;
    }
  }
  if (isJump(inst)) {
    this->pc = dRegPC;
    this->fRegNew.bubble = true;
    this->dRegNew.bubble = true;
  }
  if (isReadMem(inst)) {
    if (this->dRegNew.rs1 == destReg || this->dRegNew.rs2 == destReg) {
      this->fRegNew.stall = 2;
      this->dRegNew.stall = 2;
      this->eRegNew.bubble = true;
      this->history.cycleCount--;
    }
  }

  if (writeReg && destReg != 0 && !isReadMem(inst)) {
    if (this->dRegNew.rs1 == destReg) {
      this->dRegNew.op1 = out;
      this->executeWBReg = destReg;
      this->executeWriteBack = true;
    }
    if (this->dRegNew.rs2 == destReg) {
      this->dRegNew.op2 = out;
      this->executeWBReg = destReg;
      this->executeWriteBack = true;
    }
  }

  this->eRegNew.bubble = false;
  this->eRegNew.stall = false;
  this->eRegNew.pc = dRegPC;
  this->eRegNew.inst = inst;
  this->eRegNew.op1 = op1; 
  this->eRegNew.op2 = op2; 
  this->eRegNew.writeReg = writeReg;
  this->eRegNew.destReg = destReg;
  this->eRegNew.out = out;
  this->eRegNew.writeMem = writeMem;
  this->eRegNew.readMem = readMem;
  this->eRegNew.readSignExt = readSignExt;
  this->eRegNew.memLen = memLen;
  this->eRegNew.branch = branch;
}

void Simulator::memoryAccess() {
  if (this->eReg.stall) {
    return;
  }
  if (this->eReg.bubble) {
    this->mRegNew.bubble = true;
    return;
  }

  uint64_t eRegPC = this->eReg.pc;
  Inst inst = this->eReg.inst;
  bool writeReg = this->eReg.writeReg;
  uint32_t destReg = this->eReg.destReg;
  int64_t op1 = this->eReg.op1; // for jalr
  int64_t op2 = this->eReg.op2; // for store
  int64_t out = this->eReg.out;
  bool writeMem = this->eReg.writeMem;
  bool readMem = this->eReg.readMem;
  bool readSignExt = this->eReg.readSignExt;
  uint32_t memLen = this->eReg.memLen;

  bool good = true;
  uint32_t cycles = 0;

  if (writeMem) {
    switch (memLen) {
    case 1:
      good = this->memory->setByte(out, op2, &cycles);
      break;
    case 2:
      good = this->memory->setShort(out, op2, &cycles);
      break;
    case 4:
      good = this->memory->setInt(out, op2, &cycles);
      break;
    case 8:
      good = this->memory->setLong(out, op2, &cycles);
      break;
    default:
    fprintf(stderr,"Unknown memLen %d\n", memLen);
    exit(-1);
    }
  }

  if (!good) {
    fprintf(stderr,"Invalid Mem Access!\n");
    exit(-1);
  }

  if (readMem) {
    switch (memLen) {
    case 1:
      if (readSignExt) {
        out = (int64_t)this->memory->getByte(out, &cycles);
      } else {
        out = (uint64_t)this->memory->getByte(out, &cycles);
      }
      break;
    case 2:
      if (readSignExt) {
        out = (int64_t)this->memory->getShort(out, &cycles);
      } else {
        out = (uint64_t)this->memory->getShort(out, &cycles);
      }
      break;
    case 4:
      if (readSignExt) {
        out = (int64_t)this->memory->getInt(out, &cycles);
      } else {
        out = (uint64_t)this->memory->getInt(out, &cycles);
      }
      break;
    case 8:
      if (readSignExt) {
        out = (int64_t)this->memory->getLong(out, &cycles);
      } else {
        out = (uint64_t)this->memory->getLong(out, &cycles);
      }
      break;
    default:
        fprintf(stderr,"Unknown memLen %d\n", memLen);
        exit(-1);
    }
  }

  this->history.cycleCount += cycles;

  // Check for data hazard and forward data
  if (writeReg && destReg != 0) {
    if (this->dRegNew.rs1 == destReg) {
      // Avoid overwriting recent values
      if (this->executeWriteBack == false ||
          (this->executeWriteBack && this->executeWBReg != destReg)) {
        this->dRegNew.op1 = out;
        this->memoryWriteBack = true;
        this->memoryWBReg = destReg;
      }
    }
    if (this->dRegNew.rs2 == destReg) {
      // Avoid overwriting recent values
      if (this->executeWriteBack == false ||
          (this->executeWriteBack && this->executeWBReg != destReg)) {
        this->dRegNew.op2 = out;
        this->memoryWriteBack = true;
        this->memoryWBReg = destReg;
      }
    }
    // Corner case of forwarding mem load data to stalled decode reg
    if (this->dReg.stall) {
      if (this->dReg.rs1 == destReg) this->dReg.op1 = out;
      if (this->dReg.rs2 == destReg) this->dReg.op2 = out;
      this->memoryWriteBack = true;
      this->memoryWBReg = destReg;
    }
  }

  this->mRegNew.bubble = false;
  this->mRegNew.stall = false;
  this->mRegNew.pc = eRegPC;
  this->mRegNew.inst = inst;
  this->mRegNew.op1 = op1;
  this->mRegNew.op2 = op2;
  this->mRegNew.destReg = destReg;
  this->mRegNew.writeReg = writeReg;
  this->mRegNew.out = out;
}

void Simulator::writeBack() {
  if (this->mReg.stall) {
    return;
  }
  if (this->mReg.bubble) {
    return;
  }

  if (this->mReg.writeReg && this->mReg.destReg != 0) {
    // Check for data hazard and forward data
    if (this->dRegNew.rs1 == this->mReg.destReg) {
      // Avoid overwriting recent data
      if (!this->executeWriteBack ||
          (this->executeWriteBack &&
           this->executeWBReg != this->mReg.destReg)) {
        if (!this->memoryWriteBack ||
            (this->memoryWriteBack &&
             this->memoryWBReg != this->mReg.destReg)) {
          this->dRegNew.op1 = this->mReg.out;
        }
      }
    }
    if (this->dRegNew.rs2 == this->mReg.destReg) {
      // Avoid overwriting recent data
      if (!this->executeWriteBack ||
          (this->executeWriteBack &&
           this->executeWBReg != this->mReg.destReg)) {
        if (!this->memoryWriteBack ||
            (this->memoryWriteBack &&
             this->memoryWBReg != this->mReg.destReg)) {
          this->dRegNew.op2 = this->mReg.out;
        }
      }
    }
    this->reg[this->mReg.destReg] = this->mReg.out;
  }
}

int64_t Simulator::handleSystemCall(int64_t op1, int64_t op2) {
  int64_t type = op2; // reg a7
  int64_t arg1 = op1; // reg a0
  switch (type) {
  case 0: { // print string
    uint32_t addr = arg1;
    char ch = this->memory->getByte(addr);
    while (ch != '\0') {
      printf("%c", ch);
      ch = this->memory->getByte(++addr);
    }
    break;
  }
  case 1: // print char
    printf("%c", (char)arg1);
    break;
  case 2: // print num
    printf("%d", (int32_t)arg1);
    break;
  case 3:
  case 93: // exit
    this->print_history();
    exit(0);
    exit(0);
  case 4: 
    read_char(op1);
    break;
  case 5: 
    read_num(op1);
    break;
  default:
    fprintf(stderr,"Unknown syscall type %d\n", type);
        exit(-1);
  }
  return op1;
}


void Simulator::print_history() {
  printf("\n");
  if (this->use_cache){
     printf("With Cache\n");
  }
  else{
     printf("Without Cache\n");
  }
  printf("Number of Instructions: %u\n", this->history.instCount);
  printf("Number of Cycles: %u\n", this->history.cycleCount);
  printf("Avg Cycles per Instrcution: %.4f\n",
         (float)this->history.cycleCount / this->history.instCount);
}


void Simulator::read_char(int64_t& oprand1){
    char inputBuffer[256];
    if (fgets(inputBuffer, sizeof(inputBuffer), stdin)) {
        char ch;
        // Use sscanf to read the first non-whitespace character.
        if (sscanf(inputBuffer, " %c", &ch) == 1) {
            oprand1 = ch;
            return;
        }
        else {
            std::cerr << "Invalid char input!\n";
            exit(-1);
        }
    }
    else {
        std::cerr << "Char read failed!\n";
        exit(-1);
    }
}


void Simulator::read_num(int64_t& oprand1){
    char inputBuffer2[256];
    if (fgets(inputBuffer2, sizeof(inputBuffer2), stdin)) {
        int num;
        // Use sscanf to read the first integer from the buffer.
        if (sscanf(inputBuffer2, " %d", &num) == 1) {
            oprand1 = num;
            return;
        }
        else {
            std::cerr << "Invalid number input!\n";
            exit(-1);
        }
    }
    else {
        std::cerr << "Number read failed!\n";
        exit(-1);
    }
}