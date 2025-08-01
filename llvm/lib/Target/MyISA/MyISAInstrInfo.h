// MyISAInstrInfo.h
#ifndef LLVM_LIB_TARGET_MYISA_MYISAINSTRINFO_H
#define LLVM_LIB_TARGET_MYISA_MYISAINSTRINFO_H

#include "MyISARegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

namespace llvm {

class MyISAInstrInfo : public TargetInstrInfo {
  const MyISASubtarget &Subtarget;

public:
  explicit MyISAInstrInfo(const MyISASubtarget &STI);

  // Required virtual functions
  bool isLoadInstr(const MachineInstr &MI, unsigned &AddrReg, 
                   int64_t &Offset) const override;
  bool isStoreInstr(const MachineInstr &MI, unsigned &AddrReg, 
                    int64_t &Offset) const override;
  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
                   const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                   bool KillSrc) const override;
  
  // Instruction encoding
  std::pair<unsigned, unsigned> 
  decomposeMachineOperandsTargetFlags(unsigned TF) const override;
  
  // Additional functions for vector operations
  bool isReductionOp(unsigned Opcode) const;
  bool isCompareOp(unsigned Opcode) const;
  bool isLogicalOp(unsigned Opcode) const;
  
  // Direct address handling
  bool isDirectAddressAccess(unsigned Opcode) const;
  uint64_t getDirectAddress(const MachineInstr &MI) const;
};

} // namespace llvm

#endif