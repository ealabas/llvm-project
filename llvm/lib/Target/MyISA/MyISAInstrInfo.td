// MyISAInstrInfo.td
include "MyISAInstrFormats.td"
include "MyISARegisterInfo.td"

// Opcodes
def OPC_ADD    : Opcode<0x00>;
def OPC_SUB    : Opcode<0x01>;
def OPC_MUL    : Opcode<0x02>;
def OPC_DIV    : Opcode<0x03>;
def OPC_REDUCE : Opcode<0x04>;
def OPC_EQ     : Opcode<0x05>;
def OPC_NE     : Opcode<0x06>;
def OPC_GT     : Opcode<0x07>;
def OPC_GE     : Opcode<0x08>;
def OPC_LT     : Opcode<0x09>;
def OPC_LE     : Opcode<0x0A>;
def OPC_AND    : Opcode<0x0B>;
def OPC_OR     : Opcode<0x0C>;
def OPC_XOR    : Opcode<0x0D>;
def OPC_LOAD   : Opcode<0x0E>;
def OPC_STORE  : Opcode<0x0F>;
def OPC_JUMP   : Opcode<0x10>;
def OPC_NOP    : Opcode<0x11>;

// Arithmetic Operations
def ADD : MyISArFormat<OPC_ADD, (outs GPR:$rd), 
                      (ins GPR:$rs1, GPR:$rs2),
                      "ADD $rd, $rs1, $rs2",
                      [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]> {
  let rd = 0;
}

def SUB : MyISArFormat<OPC_SUB, (outs GPR:$rd), 
                      (ins GPR:$rs1, GPR:$rs2),
                      "SUB $rd, $rs1, $rs2",
                      [(set GPR:$rd, (sub GPR:$rs1, GPR:$rs2))]> {
  let rd = 0;
}

def MUL : MyISArFormat<OPC_MUL, (outs GPR:$rd), 
                      (ins GPR:$rs1, GPR:$rs2),
                      "MUL $rd, $rs1, $rs2",
                      [(set GPR:$rd, (mul GPR:$rs1, GPR:$rs2))]> {
  let rd = 0;
}

def DIV : MyISArFormat<OPC_DIV, (outs GPR:$rd), 
                      (ins GPR:$rs1, GPR:$rs2),
                      "DIV $rd, $rs1, $rs2",
                      [(set GPR:$rd, (sdiv GPR:$rs1, GPR:$rs2))]> {
  let rd = 0;
}

def REDUCE : MyISArFormat<OPC_REDUCE, (outs GPR:$rd), 
                         (ins GPR:$rs1, GPR:$rs2),
                         "REDUCE $rd, $rs1, $rs2",
                         []> {
  let rd = 0;
}

// Comparison Operations
def EQ : MyISArFormat<OPC_EQ, (outs GPR:$rd), 
                     (ins GPR:$rs1, GPR:$rs2),
                     "EQ $rd, $rs1, $rs2",
                     [(set GPR:$rd, (seteq GPR:$rs1, GPR:$rs2))]> {
  let rd = 0;
}

def NE : MyISArFormat<OPC_NE, (outs GPR:$rd), 
                     (ins GPR:$rs1, GPR:$rs2),
                     "NE $rd, $rs1, $rs2",
                     [(set GPR:$rd, (setne GPR:$rs1, GPR:$rs2))]> {
  let rd = 0;
}

def GT : MyISArFormat<OPC_GT, (outs GPR:$rd), 
                     (ins GPR:$rs1, GPR:$rs2),
                     "GT $rd, $rs1, $rs2",
                     [(set GPR:$rd, (setgt GPR:$rs1, GPR:$rs2))]> {
  let rd = 0;
}

def GE : MyISArFormat<OPC_GE, (outs GPR:$rd), 
                     (ins GPR:$rs1, GPR:$rs2),
                     "GE $rd, $rs1, $rs2",
                     [(set GPR:$rd, (setge GPR:$rs1, GPR:$rs2))]> {
  let rd = 0;
}

def LT : MyISArFormat<OPC_LT, (outs GPR:$rd), 
                     (ins GPR:$rs1, GPR:$rs2),
                     "LT $rd, $rs1, $rs2",
                     [(set GPR:$rd, (setlt GPR:$rs1, GPR:$rs2))]> {
  let rd = 0;
}

def LE : MyISArFormat<OPC_LE, (outs GPR:$rd), 
                     (ins GPR:$rs1, GPR:$rs2),
                     "LE $rd, $rs1, $rs2",
                     [(set GPR:$rd, (setle GPR:$rs1, GPR:$rs2))]> {
  let rd = 0;
}

// Logical Operations
def AND : MyISArFormat<OPC_AND, (outs GPR:$rd), 
                      (ins GPR:$rs1, GPR:$rs2),
                      "AND $rd, $rs1, $rs2",
                      [(set GPR:$rd, (and GPR:$rs1, GPR:$rs2))]> {
  let rd = 0;
}

def OR : MyISArFormat<OPC_OR, (outs GPR:$rd), 
                     (ins GPR:$rs1, GPR:$rs2),
                     "OR $rd, $rs1, $rs2",
                     [(set GPR:$rd, (or GPR:$rs1, GPR:$rs2))]> {
  let rd = 0;
}

def XOR : MyISArFormat<OPC_XOR, (outs GPR:$rd), 
                      (ins GPR:$rs1, GPR:$rs2),
                      "XOR $rd, $rs1, $rs2",
                      [(set GPR:$rd, (xor GPR:$rs1, GPR:$rs2))]> {
  let rd = 0;
}

// Memory Operations with direct addressing
def LOAD : MyISAiFormat<OPC_LOAD, (outs GPR:$rd),
                        (ins i48imm:$addr),
                        "LOAD $rd, $addr",
                        [(set GPR:$rd, (load i48imm:$addr))]> {
  let reg = 0;   // Will be set by $rd operand
  let unused = 0;
}

def STORE : MyISAiFormat<OPC_STORE, (outs),
                         (ins GPR:$rs, i48imm:$addr),
                         "STORE $rs, $addr",
                         [(store GPR:$rs, i48imm:$addr)]> {
  let reg = 0;   // Will be set by $rs operand
  let unused = 0; 
}

// Control Operations
def JUMP : MyISAjFormat<OPC_JUMP, (outs), 
                        (ins GPR:$rs, brtarget:$addr),
                        "JUMP $rs, $addr",
                        [(brind (add GPR:$rs, imm:$addr))]> {
  let rs = 0;
}

def NOP : MyISAnFormat<OPC_NOP>;