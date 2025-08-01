// MyISAFrameLowering.cpp
#include "MyISAFrameLowering.h"
#include "MyISATargetMachine.h"
#include "MyISARegisterInfo.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "myisa-frame-lowering"

MyISAFrameLowering::MyISAFrameLowering(const MyISASubtarget &STI)
    : TargetFrameLowering(StackGrowsDown, /*StackAlignment=*/16, /*LocalAreaOffset=*/0),
      STI(STI) {}

bool MyISAFrameLowering::hasReservedCallFrame(const MachineFunction &MF) const {
  return !MF.getFrameInfo().hasVarSizedObjects();
}

MachineBasicBlock::iterator MyISAFrameLowering::eliminateCallFramePseudoInstr(
    MachineFunction &MF, MachineBasicBlock &MBB,
    MachineBasicBlock::iterator I) const {
  return MBB.erase(I);
}

bool MyISAFrameLowering::hasFP(const MachineFunction &MF) const {
  const TargetRegisterInfo *RegInfo = MF.getSubtarget().getRegisterInfo();
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  return MF.getTarget().Options.DisableFramePointerElim(MF) ||
         RegInfo->hasStackRealignment(MF) || MFI.hasVarSizedObjects() ||
         MFI.isFrameAddressTaken();
}

void MyISAFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                              BitVector &SavedRegs,
                                              RegScavenger *RS) const {
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
}

void MyISAFrameLowering::emitPrologue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {
  MachineFrameInfo &MFI = MF.getFrameInfo();
  const MyISARegisterInfo *RegInfo = STI.getRegisterInfo();
  const MyISAInstrInfo *TII = STI.getInstrInfo();
  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

  // Get the number of bytes to allocate from the FrameInfo
  uint64_t StackSize = MFI.getStackSize();

  // Early exit if there is no need to allocate on the stack
  if (StackSize == 0 && !MFI.adjustsStack())
    return;

  // Adjust the stack pointer
  if (StackSize) {
    // Load the stack size into a temporary register
    unsigned TempReg = RegInfo->findDeadCallerSavedReg(MBB, MBBI);
    if (TempReg == MyISA::NoRegister) {
      TempReg = MyISA::R1; // Use R1 as a temporary if no dead register is found
    }

    // Load the stack size into the temporary register
    BuildMI(MBB, MBBI, DL, TII->get(MyISA::LOAD), TempReg)
        .addImm(StackSize);

    // Subtract the stack size from the stack pointer
    BuildMI(MBB, MBBI, DL, TII->get(MyISA::SUB), MyISA::R14)
        .addReg(MyISA::R14)
        .addReg(TempReg);
  }

  // If we need a frame pointer, set it up
  if (hasFP(MF)) {
    // Copy the stack pointer to the frame pointer
    BuildMI(MBB, MBBI, DL, TII->get(MyISA::ADD), MyISA::R13)
        .addReg(MyISA::R14)
        .addReg(MyISA::R0); // R0 is zero
  }
}

void MyISAFrameLowering::emitEpilogue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {
  MachineFrameInfo &MFI = MF.getFrameInfo();
  const MyISARegisterInfo *RegInfo = STI.getRegisterInfo();
  const MyISAInstrInfo *TII = STI.getInstrInfo();
  MachineBasicBlock::iterator MBBI = MBB.end();
  DebugLoc DL;

  // Find the return instruction
  while (MBBI != MBB.begin()) {
    --MBBI;
    if (MBBI->isReturn())
      break;
  }

  if (MBBI == MBB.begin())
    return;

  DL = MBBI->getDebugLoc();

  // Get the number of bytes to deallocate from the FrameInfo
  uint64_t StackSize = MFI.getStackSize();

  // If we have a frame pointer, restore the stack pointer
  if (hasFP(MF)) {
    // Copy the frame pointer to the stack pointer
    BuildMI(MBB, MBBI, DL, TII->get(MyISA::ADD), MyISA::R14)
        .addReg(MyISA::R13)
        .addReg(MyISA::R0); // R0 is zero
  }

  // Adjust the stack pointer
  if (StackSize) {
    // Load the stack size into a temporary register
    unsigned TempReg = RegInfo->findDeadCallerSavedReg(MBB, MBBI);
    if (TempReg == MyISA::NoRegister) {
      TempReg = MyISA::R1; // Use R1 as a temporary if no dead register is found
    }

    // Load the stack size into the temporary register
    BuildMI(MBB, MBBI, DL, TII->get(MyISA::LOAD), TempReg)
        .addImm(StackSize);

    // Add the stack size to the stack pointer
    BuildMI(MBB, MBBI, DL, TII->get(MyISA::ADD), MyISA::R14)
        .addReg(MyISA::R14)
        .addReg(TempReg);
  }
}

unsigned MyISAFrameLowering::getStackAlignment(const MachineFunction &MF) const {
  return 16; // 16-byte stack alignment
}

bool MyISAFrameLowering::spillCalleeSavedRegisters(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
    ArrayRef<CalleeSavedInfo> CSI, const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return false;

  const MyISAInstrInfo *TII = STI.getInstrInfo();
  DebugLoc DL = MI != MBB.end() ? MI->getDebugLoc() : DebugLoc();

  for (const CalleeSavedInfo &Info : CSI) {
    // Store the register to the stack
    unsigned Reg = Info.getReg();
    int FrameIdx = Info.getFrameIdx();
    
    // Create the store instruction
    BuildMI(MBB, MI, DL, TII->get(MyISA::STORE))
        .addReg(Reg, getKillRegState(true))
        .addFrameIndex(FrameIdx)
        .addImm(0); // Offset
  }

  return true;
}

bool MyISAFrameLowering::restoreCalleeSavedRegisters(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
    ArrayRef<CalleeSavedInfo> CSI, const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return false;

  const MyISAInstrInfo *TII = STI.getInstrInfo();
  DebugLoc DL = MI != MBB.end() ? MI->getDebugLoc() : DebugLoc();

  for (const CalleeSavedInfo &Info : CSI) {
    // Load the register from the stack
    unsigned Reg = Info.getReg();
    int FrameIdx = Info.getFrameIdx();
    
    // Create the load instruction
    BuildMI(MBB, MI, DL, TII->get(MyISA::LOAD), Reg)
        .addFrameIndex(FrameIdx)
        .addImm(0); // Offset
  }

  return true;
}