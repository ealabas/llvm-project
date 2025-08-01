# TargetInfo/CMakeLists.txt
add_llvm_library(LLVMMyISAInfo
  MyISATargetInfo.cpp
  )

add_dependencies(LLVMMyISAInfo MyISACommonTableGen)