// MyISA.td
include "llvm/Target/Target.td"
include "MyISARegisterInfo.td"
include "MyISAInstrInfo.td"
include "MyISACallingConv.td"

def MyISA : Target {
  let InstructionSet = MyISAInstrInfo;
  let PointerWidth = 64;
  let PointerType = i64;
  let LittleEndian = false; // check
  let DataLayout = "e-m:e-p:64:64-p10:8:8-p20:8:8-i64:64-v128:64-v256:64-v512:64-v1024:64-n8:16:32:64-S128-A512-G1";
}

// def MyISAAsmParser : AsmParser;
def MyISAAsmPrinter : AsmPrinter;

def MyISATarget : Target {
  let InstructionSet = MyISAInstrInfo;
//  let AssemblyParsers = [MyISAAsmParser];
  let AssemblyPrinters = [MyISAAsmPrinter];
//  let CalleeSavedRegisters = CSR_MyISA;
}