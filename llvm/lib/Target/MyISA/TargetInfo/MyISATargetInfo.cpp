// MyISATargetInfo.cpp
#include "MyISATargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/TargetParser.h"

using namespace llvm;

// Define the target features
namespace {
// Target feature for vector operations
static const char *const MyISAFeatureNames[] = {
  "vector",
  "simd",
  "512bit",
  nullptr
};

// Target description
static Target TheMyISATarget;
}

// Register the target
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeMyISATargetInfo() {
  // Register the target
  RegisterTarget<Triple::myisa> X(TheMyISATarget, "myisa", 
                                  "MyISA Vector Accelerator", 
                                  "MyISA");
  
  // Set the target features
  TheMyISATarget.setFeatureNames(MyISAFeatureNames);
  
  // Register target-specific information
  TargetRegistry::RegisterTarget(TheMyISATarget, "myisa", 
                                "MyISA Vector Accelerator", 
                                "MyISA", 
                                Triple::myisa);
}

// Get the target instance
Target &llvm::getTheMyISATarget() {
  return TheMyISATarget;
}