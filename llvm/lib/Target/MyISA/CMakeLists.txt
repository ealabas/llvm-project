# llvm/lib/Target/MyISA/CMakeLists.txt

# Add subdirectories
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)

# Create the target library
add_llvm_target(MyISA
  MyISAAsmPrinter.cpp
  MyISAFrameLowering.cpp
  MyISAISelDAGToDAG.cpp
  MyISAISelLowering.cpp
  MyISAInstrInfo.cpp
  MyISAMachineFunctionInfo.cpp
  MyISARegisterInfo.cpp
  MyISASubtarget.cpp
  MyISATargetMachine.cpp
  MyISATargetTransformInfo.cpp
  MyISAMCInstLower.cpp
  )

# Add dependencies
add_dependencies(LLVMMyISA MyISACommonTableGen)