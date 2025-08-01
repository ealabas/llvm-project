// MyISARegisterInfo.h
#ifndef LLVM_LIB_TARGET_MYISA_MYISAREGISTERINFO_H
#define LLVM_LIB_TARGET_MYISA_MYISAREGISTERINFO_H

#include "MyISAGenRegisterInfo.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

namespace llvm {

class MyISASubtarget;
class TargetInstrInfo;

class MyISARegisterInfo : public MyISAGenRegisterInfo {
public:
  explicit MyISARegisterInfo(const MyISASubtarget &STI);

  // Required virtual functions
  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;
  BitVector getReservedRegs(const MachineFunction &MF) const override;
  bool eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj,
                          unsigned FIOperandNum, RegScavenger *RS) const override;
  Register getFrameRegister(const MachineFunction &MF) const override;
  
  // Register information functions
  bool is512BitRegister(unsigned Reg) const;
  unsigned getRegisterSize(unsigned Reg) const;
  
  // Special register accessors
  unsigned getRARegister() const { return MyISA::R15; } // Return address
  unsigned getSPRegister() const { return MyISA::R14; } // Stack pointer
  unsigned getFPRegister() const { return MyISA::R13; } // Frame pointer
  unsigned getZeroRegister() const { return MyISA::R0; } // Zero register
};

} // namespace llvm

#endif