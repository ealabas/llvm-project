// MyISAFrameLowering.h
#ifndef LLVM_LIB_TARGET_MYISA_MYISAFRAMELOWERING_H
#define LLVM_LIB_TARGET_MYISA_MYISAFRAMELOWERING_H

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {

class MyISASubtarget;

class MyISAFrameLowering : public TargetFrameLowering {
public:
  explicit MyISAFrameLowering(const MyISASubtarget &STI);

  // Stack frame size
  bool hasReservedCallFrame(const MachineFunction &MF) const override;
  MachineBasicBlock::iterator
  eliminateCallFramePseudoInstr(MachineFunction &MF,
                                MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I) const override;

  // Stack probing
  bool hasFP(const MachineFunction &MF) const override;
  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS) const override;

  // Emit prologue and epilogue
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  // Stack size
  unsigned getStackAlignment(const MachineFunction &MF) const override;
  bool spillCalleeSavedRegisters(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator MI,
                                 ArrayRef<CalleeSavedInfo> CSI,
                                 const TargetRegisterInfo *TRI) const override;
  bool restoreCalleeSavedRegisters(MachineBasicBlock &MBB,
                                   MachineBasicBlock::iterator MI,
                                   ArrayRef<CalleeSavedInfo> CSI,
                                   const TargetRegisterInfo *TRI) const override;

private:
  const MyISASubtarget &STI;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_MYISA_MYISAFRAMELOWERING_H