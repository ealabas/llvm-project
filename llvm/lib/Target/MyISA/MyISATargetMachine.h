// MyISATargetMachine.h
#ifndef LLVM_LIB_TARGET_MYISA_MYISATARGETMACHINE_H
#define LLVM_LIB_TARGET_MYISA_MYISATARGETMACHINE_H

#include "MyISASubtarget.h"
#include "llvm/Target/TargetMachine.h"
#include <optional>

namespace llvm {

class MyISATargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  MyISASubtarget Subtarget;

public:
  MyISATargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                     StringRef FS, const TargetOptions &Options,
                     std::optional<Reloc::Model> RM,
                     std::optional<CodeModel::Model> CM, CodeGenOpt::Level OL,
                     bool JIT);
  ~MyISATargetMachine() override;

  const MyISASubtarget *getSubtargetImpl() const { return &Subtarget; }
  const MyISASubtarget *getSubtargetImpl(const Function &) const override {
    return &Subtarget;
  }

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
};

} // namespace llvm

#endif