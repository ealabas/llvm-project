// MyISAISelDAGToDAG.cpp
#include "MyISA.h"
#include "MyISATargetMachine.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "myisa-isel"

namespace {

class MyISADAGToDAGISel : public SelectionDAGISel {
public:
  explicit MyISADAGToDAGISel(MyISATargetMachine &TM) : SelectionDAGISel(TM) {}

  bool runOnMachineFunction(MachineFunction &MF) override {
    return SelectionDAGISel::runOnMachineFunction(MF);
  }

private:
  SDNode *Select(SDNode *N) override;

  // Include the auto-generated code for instruction selection
#include "MyISAGenDAGISel.inc"
};

} // end anonymous namespace

SDNode *MyISADAGToDAGISel::Select(SDNode *N) {
  // If we have a custom node, handle it here
  switch (N->getOpcode()) {
  default:
    break;
  }

  // Select the default instruction
  return SelectCode(N);
}

FunctionPass *llvm::createMyISAISelDag(MyISATargetMachine &TM) {
  return new MyISADAGToDAGISel(TM);
}