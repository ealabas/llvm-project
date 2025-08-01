// MyISAInstPrinter.cpp
#include "MyISAInstPrinter.h"
#include "MyISA.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define GET_INSTRUCTION_NAME
#define PRINT_ALIAS_INSTR
#include "MyISAGenAsmWriter.inc"

void MyISAInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                 StringRef Annot, const MCSubtargetInfo &STI,
                                 raw_ostream &O) {
  printInstruction(MI, Address, O);
  printAnnotation(O, Annot);
}

void MyISAInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                    raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    printRegister(MI, OpNo, O);
  } else if (Op.isImm()) {
    printImmediate(MI, OpNo, O);
  } else {
    assert(Op.isExpr() && "Unknown operand type in printOperand");
    Op.getExpr()->print(O, &MAI);
  }
}

void MyISAInstPrinter::printImmediate(const MCInst *MI, unsigned OpNo,
                                      raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isImm())
    O << formatImm(Op.getImm());
  else
    printOperand(MI, OpNo, O);
}

void MyISAInstPrinter::printRegister(const MCInst *MI, unsigned OpNo,
                                      raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  assert(Op.isReg() && "printRegister called with non-register operand");
  O << getRegisterName(Op.getReg());
}