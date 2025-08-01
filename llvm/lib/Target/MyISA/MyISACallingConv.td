// MyISACallingConv.td

def CC_MyISA : CallingConv<[
  // Return value in R1 (512-bit register)
  CCIfType<[v16i32, v8i64, v32i16, i512], CCAssignToReg<[MyISA::R1]>>,
  
  // Arguments in R2, R3, R4, R5 (512-bit registers)
  CCIfType<[v16i32, v8i64, v32i16, i512], CCAssignToReg<[MyISA::R2, MyISA::R3, MyISA::R4, MyISA::R5]>>,
  
  // Additional arguments passed on the stack
  CCIfType<[v16i32, v8i64, v32i16, i512], CCAssignToStack<8, 8>>,
  
  // For other types (if any), assign to stack
  CCAssignToStack<8, 8>
]>;

// Define which registers are callee-saved
def CSR_MyISA : CalleeSavedRegs<(add 
  // Callee-saved registers (must be preserved by callee)
  MyISA::R6, MyISA::R7, MyISA::R8, MyISA::R9, 
  MyISA::R10, MyISA::R11, MyISA::R12
)>;

// Define calling convention for vector operations
def CC_MyISA_Vector : CallingConv<[
  // Vector operations use all available registers
  CCIfType<[v16i32, v8i64, v32i16, i512], CCAssignToReg<[MyISA::R1, MyISA::R2, MyISA::R3, MyISA::R4, MyISA::R5]>>,
  CCIfType<[v16i32, v8i64, v32i16, i512], CCAssignToStack<8, 8>>
]>;