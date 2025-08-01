// MyISATargetInfo.h
#ifndef LLVM_LIB_TARGET_MYISA_TARGETINFO_MYISATARGETINFO_H
#define LLVM_LIB_TARGET_MYISA_TARGETINFO_MYISATARGETINFO_H

#include "llvm/TargetParser/Triple.h"
#include "llvm/Support/Target.h"

namespace llvm {

class Target;

Target &getTheMyISATarget();

} // namespace llvm

#endif // LLVM_LIB_TARGET_MYISA_TARGETINFO_MYISATARGETINFO_H