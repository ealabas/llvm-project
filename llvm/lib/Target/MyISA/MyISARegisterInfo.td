// MyISARegisterInfo.td
class MyISAReg<bits<4> enc, string n> : Register<n> {
  let HWEncoding{3-0} = enc;
}

// Define 16 general-purpose registers (R0-R15), each 512 bits
def R0  : MyISAReg<0, "R0">, DwarfRegNum<0>;
def R1  : MyISAReg<1, "R1">, DwarfRegNum<1>;
def R2  : MyISAReg<2, "R2">, DwarfRegNum<2>;
def R3  : MyISAReg<3, "R3">, DwarfRegNum<3>;
def R4  : MyISAReg<4, "R4">, DwarfRegNum<4>;
def R5  : MyISAReg<5, "R5">, DwarfRegNum<5>;
def R6  : MyISAReg<6, "R6">, DwarfRegNum<6>;
def R7  : MyISAReg<7, "R7">, DwarfRegNum<7>;
def R8  : MyISAReg<8, "R8">, DwarfRegNum<8>;
def R9  : MyISAReg<9, "R9">, DwarfRegNum<9>;
def R10 : MyISAReg<10, "R10">, DwarfRegNum<10>;
def R11 : MyISAReg<11, "R11">, DwarfRegNum<11>;
def R12 : MyISAReg<12, "R12">, DwarfRegNum<12>;
def R13 : MyISAReg<13, "R13">, DwarfRegNum<13>;
def R14 : MyISAReg<14, "R14">, DwarfRegNum<14>;
def R15 : MyISAReg<15, "R15">, DwarfRegNum<15>;

// General-purpose register class - 512-bit registers
def GPR : RegisterClass<"MyISA", [v16i32, v8i64, v32i16, i512], 512, 
  (add R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15)> {
  let Size = 512;
}