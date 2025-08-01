// MyISAMCAsmInfo.h
#ifndef LLVM_LIB_TARGET_MYISA_MYISAMCASMINFO_H
#define LLVM_LIB_TARGET_MYISA_MYISAMCASMINFO_H

#include "llvm/MC/MCAsmInfo.h"

namespace llvm {

class Triple;

class MyISAMCAsmInfo : public MCAsmInfo {
public:
  explicit MyISAMCAsmInfo(const Triple &TT);
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_MYISA_MYISAMCASMINFO_H