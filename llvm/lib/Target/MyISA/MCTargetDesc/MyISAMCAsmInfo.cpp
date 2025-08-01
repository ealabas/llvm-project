// MyISAMCAsmInfo.cpp
#include "MyISAMCAsmInfo.h"
#include "llvm/ADT/Triple.h"

using namespace llvm;

MyISAMCAsmInfo::MyISAMCAsmInfo(const Triple &TT) {
  // Set up the assembly syntax
  CommentString = "#";
  SupportsQuotedNames = true;
  HasDotTypeDotSizeDirective = true;
  HasSingleParameterDotFile = true;
  HasIdentDirective = true;
  ZeroDirective = "\t.space\t";
  Data8bitsDirective = "\t.byte\t";
  Data16bitsDirective = "\t.short\t";
  Data32bitsDirective = "\t.long\t";
  Data64bitsDirective = "\t.quad\t";
  GlobalDirective = "\t.global\t";
  WeakDirective = "\t.weak\t";
  AscizDirective = "\t.asciz\t";
  HiddenVisibilityAttr = ".hidden";
  HiddenDeclarationVisibilityAttr = ".hidden";
  ProtectedVisibilityAttr = ".protected";

  // Set up alignment
  UsesELFSectionDirectiveForBSS = true;
  UsesELFSectionDirectiveForCommon = true;
  AlignmentIsInBytes = false;
  TextAlignIsInBytes = true;

  // Set up syntax for comments
  UsesIntegratedAssembler = true;
}