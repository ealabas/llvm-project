// MyISAMCInstLower.h
#ifndef LLVM_LIB_TARGET_MYISA_MYISAMCINSTLOWER_H
#define LLVM_LIB_TARGET_MYISA_MYISAMCINSTLOWER_H

#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/Support/Compiler.h"

namespace llvm {

class MCContext;
class MCInst;
class MCOperand;
class MachineInstr;
class MachineFunction;
class MyISAAsmPrinter;

class MyISAMCInstLower {
  MCContext &Ctx;
  MyISAAsmPrinter &Printer;

public:
  MyISAMCInstLower(MCContext &Ctx, MyISAAsmPrinter &Printer);

  void lower(const MachineInstr *MI, MCInst &OutMI) const;
  MCOperand lowerOperand(const MachineOperand &MO) const;

private:
  MCOperand lowerSymbolOperand(const MachineOperand &MO, MCSymbol *Sym) const;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_MYISA_MYISAMCINSTLOWER_H