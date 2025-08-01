// MyISAMCTargetDesc.h
#ifndef LLVM_LIB_TARGET_MYISA_MYISAMCTARGETDESC_H
#define LLVM_LIB_TARGET_MYISA_MYISAMCTARGETDESC_H

namespace llvm {

class Target;
class MCAsmBackend;
class MCCodeEmitter;
class MCInstrInfo;
class MCRegisterInfo;
class MCSubtargetInfo;
class MCInstPrinter;
class MCStreamer;
class MCTargetOptions;
class MCRelocationInfo;
class MCSymbol;
class Triple;
class raw_pwrite_stream;
class raw_ostream;

MCCodeEmitter *createMyISAMCCodeEmitter(const MCInstrInfo &MCII, MCContext &Ctx);
MCAsmBackend *createMyISAAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                   const MCRegisterInfo &MRI,
                                   const MCTargetOptions &Options);
MCInstPrinter *createMyISAMCInstPrinter(const Triple &T, unsigned SyntaxVariant,
                                       const MCAsmInfo &MAI,
                                       const MCInstrInfo &MII,
                                       const MCRegisterInfo &MRI);

} // namespace llvm

// Defines symbolic names for MyISA registers.
#define GET_REGINFO_ENUM
#include "MyISAGenRegisterInfo.inc"

// Defines symbolic names for MyISA instructions.
#define GET_INSTRINFO_ENUM
#include "MyISAGenInstrInfo.inc"

// Defines the enum of all the MyISA specific target nodes.
#define GET_TARGET_RECOGNIZER_ENUM
#include "MyISAGenDAGISel.inc"

#endif // LLVM_LIB_TARGET_MYISA_MYISAMCTARGETDESC_H