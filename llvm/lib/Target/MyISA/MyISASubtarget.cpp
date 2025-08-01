// MyISASubtarget.cpp
#include "MyISASubtarget.h"
#include "MyISA.h"
#include "MyISATargetMachine.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "myisa-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "MyISAGenSubtargetInfo.inc"

MyISASubtarget::MyISASubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                               const TargetMachine &TM)
    : MyISAGenSubtargetInfo(TT, CPU, /*TuneCPU*/ CPU, FS),
      InstrInfo(*this), RegInfo(*this), TLInfo(TM, *this) {
  
  // Parse features
  StringRef CPUName = CPU;
  if (CPUName.empty())
    CPUName = "generic";
  
  // Initialize features
  HasVector = false;
  HasSIMD = false;
  Has512Bit = false;
  
  // Parse subtarget features
  ParseSubtargetFeatures(CPUName, /*TuneCPU*/ CPUName, FS);
  
  // Set default features
  if (!HasVector)
    HasVector = true; // Default to having vector operations
  if (!HasSIMD)
    HasSIMD = true;    // Default to having SIMD operations
  if (!Has512Bit)
    Has512Bit = true;  // Default to having 512-bit registers
}