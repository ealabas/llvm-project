// MyISAInstrInfo.cpp
#include "MyISAInstrInfo.h"
#include "MyISATargetMachine.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "MyISAGenInstrInfo.inc"

MyISAInstrInfo::MyISAInstrInfo(const MyISASubtarget &STI)
    : MyISAGenInstrInfo(MyISA::ADJCALLSTACKDOWN, MyISA::ADJCALLSTACKUP),
      Subtarget(STI) {}

bool MyISAInstrInfo::isLoadInstr(const MachineInstr &MI, 
                                 unsigned &AddrReg, 
                                 int64_t &Offset) const {
  if (MI.getOpcode() == MyISA::LOAD) {
    // For direct addressing, there's no base register
    AddrReg = 0; // Indicate no base register
    Offset = MI.getOperand(1).getImm();
    return true;
  }
  return false;
}

bool MyISAInstrInfo::isStoreInstr(const MachineInstr &MI, 
                                  unsigned &AddrReg, 
                                  int64_t &Offset) const {
  if (MI.getOpcode() == MyISA::STORE) {
    // For direct addressing, there's no base register
    AddrReg = 0; // Indicate no base register
    Offset = MI.getOperand(2).getImm();
    return true;
  }
  return false;
}

void MyISAInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator MI,
                                 const DebugLoc &DL, MCRegister DestReg,
                                 MCRegister SrcReg, bool KillSrc) const {
  // Use ADD for register copies (ADD R0, Src, Dest)
  BuildMI(MBB, MI, DL, get(MyISA::ADD), DestReg)
      .addReg(MyISA::R0)
      .addReg(SrcReg, getKillRegState(KillSrc));
}

std::pair<unsigned, unsigned>
MyISAInstrInfo::decomposeMachineOperandsTargetFlags(unsigned TF) const {
  return std::make_pair(TF, 0u); // No special flags for now
}

bool MyISAInstrInfo::isReductionOp(unsigned Opcode) const {
  return Opcode == MyISA::REDUCE;
}

bool MyISAInstrInfo::isCompareOp(unsigned Opcode) const {
  switch (Opcode) {
    case MyISA::EQ:
    case MyISA::NE:
    case MyISA::GT:
    case MyISA::GE:
    case MyISA::LT:
    case MyISA::LE:
      return true;
    default:
      return false;
  }
}

bool MyISAInstrInfo::isLogicalOp(unsigned Opcode) const {
  switch (Opcode) {
    case MyISA::AND:
    case MyISA::OR:
    case MyISA::XOR:
      return true;
    default:
      return false;
  }
}

bool MyISAInstrInfo::isDirectAddressAccess(unsigned Opcode) const {
  return Opcode == MyISA::LOAD || Opcode == MyISA::STORE;
}

uint64_t MyISAInstrInfo::getDirectAddress(const MachineInstr &MI) const {
  if (MI.getOpcode() == MyISA::LOAD) {
    return MI.getOperand(1).getImm();
  } else if (MI.getOpcode() == MyISA::STORE) {
    return MI.getOperand(2).getImm();
  }
  llvm_unreachable("Not a direct address instruction");
}