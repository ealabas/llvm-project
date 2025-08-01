// MyISAInstrFormats.td
class MyISAInst<dag outs, dag ins, string asmstr, list<dag> pattern> : Instruction {
  let Namespace = "MyISA";
  bits<64> Inst;
  
  bits<8> Opcode;
  let Inst{63-56} = Opcode;
}

// R-Type format (for arithmetic, comparison, logical operations)
class MyISArFormat<bits<8> opcode, dag outs, dag ins, string asmstr, list<dag> pattern>
    : MyISAInst<outs, ins, asmstr, pattern> {
  bits<6> rd;
  bits<6> rs1;
  bits<6> rs2;
  
  let Opcode = opcode;
  let Inst{55-50} = rd;
  let Inst{49-44} = rs1;
  let Inst{43-38} = rs2;
  let Inst{37-0} = 0; // Reserved
}

// I/S-Type format (for memory operations with direct address)
class MyISAiFormat<bits<8> opcode, dag outs, dag ins, string asmstr, list<dag> pattern>
    : MyISAInst<outs, ins, asmstr, pattern> {
  bits<6> reg;
  bits<2> unused;
  bits<48> imm;
  
  let Opcode = opcode;
  let Inst{55-50} = reg;
  let Inst{49-48} = unused;
  let Inst{47-0} = imm;
}

// J-Type format (for JUMP)
class MyISAjFormat<bits<8> opcode, dag outs, dag ins, string asmstr, list<dag> pattern>
    : MyISAInst<outs, ins, asmstr, pattern> {
  bits<6> rs;
  bits<44> addr;
  
  let Opcode = opcode;
  let Inst{55-50} = rs;
  let Inst{49-44} = 0; // Reserved
  let Inst{43-0} = addr;
}

// N-Type format (for NOP)
class MyISAnFormat<bits<8> opcode> : MyISAInst<(outs), (ins), "", []> {
  let Opcode = opcode;
  let Inst{55-0} = 0; // All reserved
}