// MyISATargetMachine.cpp
#include "MyISATargetMachine.h"
#include "MyISATargetTransformInfo.h"
#include "TargetInfo/MyISATargetInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Transforms/Scalar.h"
#include <optional>

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeMyISATarget() {
  // Register the target
  RegisterTargetMachine<MyISATargetMachine> X(getTheMyISATarget());
  
  // Register passes
  PassRegistry &PR = *PassRegistry::getPassRegistry();
  initializeMyISADAGToDAGISelPass(PR);
}

static std::string computeDataLayout(const Triple &TT) {
  return "e-m:e-p:64:64-p10:8:8-p20:8:8-i64:64-v128:64-v256:64-v512:64-v1024:64"
         "-n8:16:32:64-S128-A512-G1";
}

static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::PIC_);
}

MyISATargetMachine::MyISATargetMachine(const Target &T, const Triple &TT,
                                       StringRef CPU, StringRef FS,
                                       const TargetOptions &Options,
                                       std::optional<Reloc::Model> RM,
                                       std::optional<CodeModel::Model> CM,
                                       CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT), TT, CPU, FS, Options,
                        getEffectiveRelocModel(RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, CPU, FS, *this) {
  initAsmInfo();
}

MyISATargetMachine::~MyISATargetMachine() = default;

TargetPassConfig *MyISATargetMachine::createPassConfig(PassManagerBase &PM) {
  return new MyISAPassConfig(*this, PM);
}

MyISAPassConfig::MyISAPassConfig(MyISATargetMachine &TM, PassManagerBase &PM)
    : TargetPassConfig(TM, PM) {}

MyISAPassConfig::~MyISAPassConfig() = default;

bool MyISAPassConfig::addInstSelector() {
  addPass(createMyISAISelDag(getMyISATargetMachine()));
  return false;
}

void MyISAPassConfig::addPreRegAlloc() {
  // Add pre-register allocation passes
}

void MyISAPassConfig::addPostRegAlloc() {
  // Add post-register allocation passes
}