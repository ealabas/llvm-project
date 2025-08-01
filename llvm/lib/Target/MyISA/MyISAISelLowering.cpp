// MyISAISelLowering.cpp
#include "MyISAISelLowering.h"
#include "MyISATargetMachine.h"
#include "MyISARegisterInfo.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "myisa-lower"

MyISAISelLowering::MyISAISelLowering(const MyISATargetMachine &TM,
                                     const MyISASubtarget &STI)
    : TargetLowering(TM), Subtarget(STI) {
  // Set up the register classes
  addRegisterClass(MVT::i512, &MyISA::GPRRegClass);
  addRegisterClass(MVT::v16i32, &MyISA::GPRRegClass);
  addRegisterClass(MVT::v8i64, &MyISA::GPRRegClass);
  addRegisterClass(MVT::v32i16, &MyISA::GPRRegClass);
  
  // Compute register properties
  computeRegisterProperties(Subtarget.getRegisterInfo());
  
  // Set operation actions
  setOperationAction(ISD::LOAD, MVT::i512, Legal);
  setOperationAction(ISD::STORE, MVT::i512, Legal);
  setOperationAction(ISD::ADD, MVT::i512, Legal);
  setOperationAction(ISD::SUB, MVT::i512, Legal);
  setOperationAction(ISD::MUL, MVT::i512, Legal);
  setOperationAction(ISD::SDIV, MVT::i512, Legal);
  
  // Set operation actions for vector types
  for (MVT VT : {MVT::v16i32, MVT::v8i64, MVT::v32i16}) {
    setOperationAction(ISD::LOAD, VT, Legal);
    setOperationAction(ISD::STORE, VT, Legal);
    setOperationAction(ISD::ADD, VT, Legal);
    setOperationAction(ISD::SUB, VT, Legal);
    setOperationAction(ISD::MUL, VT, Legal);
    setOperationAction(ISD::SDIV, VT, Legal);
  }
}

SDValue MyISAISelLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &dl,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  
  MachineFunction &MF = DAG.getMachineFunction();
  MachineRegisterInfo &RegInfo = MF.getRegInfo();
  
  // Assign locations to all incoming arguments
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, isVarArg, MF, ArgLocs, *DAG.getContext());
  
  CCInfo.AnalyzeFormalArguments(Ins, CCAssignFnForCall(CallConv, isVarArg));
  
  // Create frame index for arguments passed on the stack
  unsigned StackSlotSize = 8; // 64-bit stack slots
  int StackOffset = 0;
  
  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];
    SDValue ArgValue;
    
    if (VA.isRegLoc()) {
      // Argument passed in register
      unsigned VReg = RegInfo.createVirtualRegister(&MyISA::GPRRegClass);
      RegInfo.addLiveIn(VA.getLocReg(), VReg);
      ArgValue = DAG.getCopyFromReg(Chain, dl, VReg, VA.getValVT());
    } else {
      // Argument passed on stack
      int FI = MF.getFrameInfo().CreateFixedObject(
          VA.getValVT().getSizeInBits() / 8, StackOffset, false);
      SDValue FIN = DAG.getFrameIndex(FI, getPointerTy());
      ArgValue = DAG.getLoad(VA.getValVT(), dl, Chain, FIN,
                            MachinePointerInfo::getFixedStack(MF, FI));
      StackOffset += StackSlotSize;
    }
    
    InVals.push_back(ArgValue);
  }
  
  return Chain;
}

SDValue MyISAISelLowering::LowerCall(
    TargetLowering::CallLoweringInfo &CLI,
    SmallVectorImpl<SDValue> &InVals) const {
  
  SelectionDAG &DAG = CLI.DAG;
  SDLoc &dl = CLI.DL;
  SmallVector<ISD::OutputArg, 32> &Outs = CLI.Outs;
  SmallVector<SDValue, 32> &OutVals = CLI.OutVals;
  SmallVector<ISD::InputArg, 32> &Ins = CLI.Ins;
  SDValue Chain = CLI.Chain;
  SDValue Callee = CLI.Callee;
  bool &isTailCall = CLI.IsTailCall;
  CallingConv::ID CallConv = CLI.CallConv;
  bool isVarArg = CLI.IsVarArg;
  
  // Analyze operands of the call
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), ArgLocs,
                 *DAG.getContext());
  
  CCInfo.AnalyzeCallOperands(Outs, CCAssignFnForCall(CallConv, isVarArg));
  
  // Create a temporary register for the call address
  unsigned CallReg = MyISA::R1;
  SDValue CallRegVal = DAG.getRegister(CallReg, getPointerTy());
  
  // Emit the call instruction
  SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Glue);
  SmallVector<SDValue, 8> Ops;
  Ops.push_back(Chain);
  Ops.push_back(Callee);
  
  // Add arguments to the call
  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];
    SDValue Arg = OutVals[i];
    
    if (VA.isRegLoc()) {
      Ops.push_back(DAG.getRegister(VA.getLocReg(), VA.getValVT()));
    } else {
      // Handle stack arguments
      llvm_unreachable("Stack arguments not yet implemented");
    }
  }
  
  Chain = DAG.getNode(MyISAISD::CALL, dl, NodeTys, Ops);
  
  // Handle return values
  if (!Ins.empty()) {
    return LowerCallResult(Chain, CallReg, CallConv, isVarArg, Ins, dl, DAG, InVals);
  }
  
  return Chain;
}

SDValue MyISAISelLowering::LowerReturn(
    SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs,
    const SmallVectorImpl<SDValue> &OutVals, const SDLoc &dl,
    SelectionDAG &DAG) const {
  
  // Analyze return values
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());
  
  CCInfo.AnalyzeReturn(Outs, CCAssignFnForReturn(CallConv, isVarArg));
  
  // Copy return values into appropriate registers
  SmallVector<SDValue, 4> RetOps;
  RetOps.push_back(Chain);
  
  for (unsigned i = 0, e = RVLocs.size(); i != e; ++i) {
    CCValAssign &VA = RVLocs[i];
    SDValue RetValue = OutVals[i];
    
    if (VA.isRegLoc()) {
      RetOps.push_back(DAG.getCopyToReg(Chain, dl, VA.getLocReg(), RetValue));
    } else {
      llvm_unreachable("Return values on stack not yet implemented");
    }
  }
  
  // Return with the return address in R15
  RetOps.push_back(DAG.getRegister(MyISA::R15, MVT::i64));
  
  return DAG.getNode(MyISAISD::RET, dl, MVT::Other, RetOps);
}

CCAssignFn *MyISAISelLowering::CCAssignFnForCall(CallingConv::ID CC,
                                                   bool IsVarArg) const {
  return RetCC_MyISA_Vector;
}

CCAssignFn *MyISAISelLowering::CCAssignFnForReturn(CallingConv::ID CC,
                                                     bool IsVarArg) const {
  return RetCC_MyISA_Vector;
}

SDValue MyISAISelLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
    default:
      llvm_unreachable("Unimplemented operand");
  }
  return SDValue();
}