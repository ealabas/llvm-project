// MyISAMachineFunctionInfo.cpp
#include "MyISAMachineFunctionInfo.h"
#include "MyISATargetMachine.h"

using namespace llvm;

void MyISATargetMachine::resetMachineFunctionInfo(MachineFunction &MF) const {
  MF.getInfo<MyISAMachineFunctionInfo>()->setSRetReturnReg(0);
}