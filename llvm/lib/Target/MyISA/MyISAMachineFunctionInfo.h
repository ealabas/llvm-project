// MyISAMachineFunctionInfo.h
#ifndef LLVM_LIB_TARGET_MYISA_MYISAMACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_MYISA_MYISAMACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {

class MyISAMachineFunctionInfo : public MachineFunctionInfo {
private:
  /// Frame pointer save slot. Used for handling variable sized objects.
  int FramePointerSaveIndex = 0;

  /// Return address save slot. Used for handling return address.
  int ReturnAddressSaveIndex = 0;

  /// VarArgsFrameIndex - Frame index for start of varargs area.
  int VarArgsFrameIndex = 0;

  /// SRetReturnReg - Holds the virtual register into which the sret
  /// argument is passed.
  unsigned SRetReturnReg = 0;

public:
  MyISAMachineFunctionInfo(const MachineFunction &MF) {}

  int getFramePointerSaveIndex() const { return FramePointerSaveIndex; }
  void setFramePointerSaveIndex(int Index) { FramePointerSaveIndex = Index; }

  int getReturnAddressSaveIndex() const { return ReturnAddressSaveIndex; }
  void setReturnAddressSaveIndex(int Index) { ReturnAddressSaveIndex = Index; }

  int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }
  void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }

  unsigned getSRetReturnReg() const { return SRetReturnReg; }
  void setSRetReturnReg(unsigned Reg) { SRetReturnReg = Reg; }
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_MYISA_MYISAMACHINEFUNCTIONINFO_H