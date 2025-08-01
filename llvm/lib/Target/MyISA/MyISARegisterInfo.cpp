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
    AddrReg = 0; // Direct addressing, no base register
    Offset = MI.getOperand(1).getImm();
    return true;
  }
  return false;
}

bool MyISAInstrInfo::isStoreInstr(const MachineInstr &MI, 
                                  unsigned &AddrReg, 
                                  int64_t &Offset) const {
  if (MI.getOpcode() == MyISA::STORE) {
    AddrReg = 0; // Direct addressing, no base register
    Offset = MI.getOperand(2).getImm();
    return true;
  }
  return false;
}

void MyISAInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator MI,
                                 const DebugLoc &DL, MCRegister DestReg,
                                 MCRegister SrcReg, bool KillSrc) const {
  // Use ADD for 512-bit register copies (ADD R0, Src, Dest)
  // R0 is our zero register
  BuildMI(MBB, MI, DL, get(MyISA::ADD), DestReg)
      .addReg(MyISA::R0)
      .addReg(SrcReg, getKillRegState(KillSrc));
}

// ... rest of the file remains the same ...