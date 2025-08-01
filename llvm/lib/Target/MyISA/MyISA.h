// MyISA.h
#ifndef LLVM_LIB_TARGET_MYISA_MYISA_H
#define LLVM_LIB_TARGET_MYISA_MYISA_H

#include "MCTargetDesc/MyISAMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class FunctionPass;
class MyISATargetMachine;
class PassRegistry;

// Initialize the passes for the MyISA target
void initializeMyISADAGToDAGISelPass(PassRegistry &);
void initializeMyISAExpandPseudoPass(PassRegistry &);

// Function passes
FunctionPass *createMyISAISelDag(MyISATargetMachine &TM);
FunctionPass *createMyISAExpandPseudoPass();

} // namespace llvm

#endif // LLVM_LIB_TARGET_MYISA_MYISA_H