// MyISAMCTargetDesc.cpp
#include "MyISAMCTargetDesc.h"
#include "MyISAInstPrinter.h"
#include "MyISAMCAsmInfo.h"
#include "TargetInfo/MyISATargetInfo.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "MyISAGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "MyISAGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "MyISAGenRegisterInfo.inc"

static MCInstrInfo *createMyISAMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitMyISAMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createMyISAMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitMyISAMCRegisterInfo(X, MyISA::R15);
  return X;
}

static MCSubtargetInfo *createMyISAMCSubtargetInfo(const Triple &TT,
                                                   StringRef CPU,
                                                   StringRef FS) {
  return createMyISAMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

static MCAsmInfo *createMyISAMCAsmInfo(const MCRegisterInfo &MRI,
                                      const Triple &TT,
                                      const MCTargetOptions &Options) {
  return new MyISAMCAsmInfo(TT);
}

static MCInstPrinter *createMyISAMCInstPrinter(const Triple &T,
                                                unsigned SyntaxVariant,
                                                const MCAsmInfo &MAI,
                                                const MCInstrInfo &MII,
                                                const MCRegisterInfo &MRI) {
  return new MyISAInstPrinter(MAI, MII, MRI);
}

static MCCodeEmitter *createMyISAMCCodeEmitter(const MCInstrInfo &MCII,
                                               MCContext &Ctx) {
  return new MyISAMCCodeEmitter(MCII, Ctx);
}

static MCAsmBackend *createMyISAAsmBackend(const Target &T,
                                          const MCSubtargetInfo &STI,
                                          const MCRegisterInfo &MRI,
                                          const MCTargetOptions &Options) {
  return new MyISAAsmBackend(STI);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeMyISATargetMC() {
  // Register the MCAsmInfo
  TargetRegistry::RegisterMCAsmInfo(getTheMyISATarget(), createMyISAMCAsmInfo);

  // Register the MCInstrInfo
  TargetRegistry::RegisterMCInstrInfo(getTheMyISATarget(), createMyISAMCInstrInfo);

  // Register the MCRegisterInfo
  TargetRegistry::RegisterMCRegInfo(getTheMyISATarget(), createMyISAMCRegisterInfo);

  // Register the MCSubtargetInfo
  TargetRegistry::RegisterMCSubtargetInfo(getTheMyISATarget(),
                                          createMyISAMCSubtargetInfo);

  // Register the MCInstPrinter
  TargetRegistry::RegisterMCInstPrinter(getTheMyISATarget(),
                                        createMyISAMCInstPrinter);

  // Register the MCCodeEmitter
  TargetRegistry::RegisterMCCodeEmitter(getTheMyISATarget(),
                                         createMyISAMCCodeEmitter);

  // Register the MCAsmBackend
  TargetRegistry::RegisterMCAsmBackend(getTheMyISATarget(),
                                        createMyISAAsmBackend);
}