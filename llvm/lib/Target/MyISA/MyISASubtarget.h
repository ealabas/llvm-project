// MyISASubtarget.h
#ifndef LLVM_LIB_TARGET_MYISA_MYISASUBTARGET_H
#define LLVM_LIB_TARGET_MYISA_MYISASUBTARGET_H

#include "MyISAGenSubtargetInfo.inc"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

#define GET_SUBTARGETINFO_HEADER
#include "MyISAGenSubtargetInfo.inc"

namespace llvm {

class StringRef;

class MyISASubtarget : public MyISAGenSubtargetInfo {
public:
  MyISASubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                 const TargetMachine &TM);

  // Feature accessors
  bool hasVectorOperations() const { return HasVector; }
  bool hasSIMDOperations() const { return HasSIMD; }
  bool has512BitRegisters() const { return Has512Bit; }

  // Override TargetSubtargetInfo functions
  const MyISAInstrInfo *getInstrInfo() const override { return &InstrInfo; }
  const MyISAFrameLowering *getFrameLowering() const override { return &FrameLowering; }
  const MyISATargetLowering *getTargetLowering() const override { return &TLInfo; }
  const MyISARegisterInfo *getRegisterInfo() const override { return &RegInfo; }

private:
  // Feature flags
  bool HasVector;
  bool HasSIMD;
  bool Has512Bit;

  // Components
  MyISAInstrInfo InstrInfo;
  MyISAFrameLowering FrameLowering;
  MyISATargetLowering TLInfo;
  MyISARegisterInfo RegInfo;
};

} // namespace llvm

#endif