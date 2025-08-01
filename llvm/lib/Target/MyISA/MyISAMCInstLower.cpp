// MyISAMCInstLower.cpp
#include "MyISAMCInstLower.h"
#include "MyISAAsmPrinter.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

MyISAMCInstLower::MyISAMCInstLower(MCContext &Ctx, MyISAAsmPrinter &Printer)
    : Ctx(Ctx), Printer(Printer) {}

MCOperand MyISAMCInstLower::lowerSymbolOperand(const MachineOperand &MO,
                                               MCSymbol *Sym) const {
  const MCExpr *Expr = MCSymbolRefExpr::create(Sym, Ctx);
  
  if (!MO.isJTI() && !MO.isMBB() && MO.getOffset())
    Expr = MCBinaryExpr::createAdd(Expr, MCConstantExpr::create(MO.getOffset(), Ctx), Ctx);
  
  return MCOperand::createExpr(Expr);
}

MCOperand MyISAMCInstLower::lowerOperand(const MachineOperand &MO) const {
  switch (MO.getType()) {
  default:
    llvm_unreachable("unknown operand type");
  case MachineOperand::MO_Register:
    if (MO.isImplicit())
      return MCOperand();
    return MCOperand::createReg(MO.getReg());
  case MachineOperand::MO_Immediate:
    return MCOperand::createImm(MO.getImm());
  case MachineOperand::MO_MachineBasicBlock:
    return lowerSymbolOperand(MO, MO.getMBB()->getSymbol());
  case MachineOperand::MO_GlobalAddress:
    return lowerSymbolOperand(MO, Printer.getSymbol(MO.getGlobal()));
  case MachineOperand::MO_ExternalSymbol:
    return lowerSymbolOperand(MO, Printer.GetExternalSymbolSymbol(MO.getSymbolName()));
  case MachineOperand::MO_BlockAddress:
    return lowerSymbolOperand(MO, Printer.GetBlockAddressSymbol(MO.getBlockAddress()));
  case MachineOperand::MO_RegisterMask:
    break;
  case MachineOperand::MO_ConstantPoolIndex:
    return lowerSymbolOperand(MO, Printer.GetCPISymbol(MO.getIndex()));
  }
  return MCOperand();
}

void MyISAMCInstLower::lower(const MachineInstr *MI, MCInst &OutMI) const {
  OutMI.setOpcode(MI->getOpcode());
  
  for (const MachineOperand &MO : MI->operands()) {
    MCOperand MCOp = lowerOperand(MO);
    if (MCOp.isValid())
      OutMI.addOperand(MCOp);
  }
}