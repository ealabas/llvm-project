// MyISAISelLowering.h
#ifndef LLVM_LIB_TARGET_MYISA_MYISASELLOWERING_H
#define LLVM_LIB_TARGET_MYISA_MYISASELLOWERING_H

#include "MyISA.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

class MyISASubtarget;
class MyISATargetMachine;

class MyISAISelLowering : public TargetLowering {
public:
  explicit MyISAISelLowering(const MyISATargetMachine &TM,
                            const MyISASubtarget &STI);

  // Lowering hooks
  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                              bool isVarArg,
                              const SmallVectorImpl<ISD::InputArg> &Ins,
                              const SDLoc &dl, SelectionDAG &DAG,
                              SmallVectorImpl<SDValue> &InVals) const override;
  
  SDValue LowerCall(TargetLowering::CallLoweringInfo &CLI,
                    SmallVectorImpl<SDValue> &InVals) const override;
  
  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals,
                      const SDLoc &dl, SelectionDAG &DAG) const override;
  
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;
  
  // Register information
  const MyISASubtarget &getSubtarget() const { return Subtarget; }
  
  // Helper functions
  CCAssignFn *CCAssignFnForCall(CallingConv::ID CC, bool IsVarArg) const;
  CCAssignFn *CCAssignFnForReturn(CallingConv::ID CC, bool IsVarArg) const;

private:
  const MyISASubtarget &Subtarget;
  
  // Lowering helpers
  SDValue LowerCallResult(SDValue Chain, SDValue InFlag,
                         CallingConv::ID CallConv, bool IsVarArg,
                         const SmallVectorImpl<ISD::InputArg> &Ins,
                         const SDLoc &dl, SelectionDAG &DAG,
                         SmallVectorImpl<SDValue> &InVals) const;
};

} // namespace llvm

#endif