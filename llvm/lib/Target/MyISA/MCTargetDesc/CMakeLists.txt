# MCTargetDesc/CMakeLists.txt
add_llvm_library(LLVMMyISADesc
  MyISAMCTargetDesc.cpp
  MyISAMCAsmInfo.cpp
  MyISAInstPrinter.cpp
  MyISAMCExpr.cpp
  )

add_dependencies(LLVMMyISADesc MyISACommonTableGen)