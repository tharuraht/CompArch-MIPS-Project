// #include "instructions.hpp"
#include <stdint.h>
#include <climits>
#include <vector>
#include "memory.cpp"

typedef int32_t reg;
typedef int32_t mips_error;

//R type--------------------------------------------------------------------------
mips_error ADD (reg &rs, reg &rt, reg &rd) {
  /*
  adds two regs, stores result in reg
  has overflow
  if overflow occurs:
    destination reg not modified
    integer overflow exception, return an error code -10
  */
  int32_t result = rs + rt;
  if ((rs>0) && (rt > 0) && (result <= 0)) {
    return -10;
  }
  else if ((rs < 0) && (rt < 0) && (result >= 0)) {
    return -10;
  }
  else {
    rd = result;
    return 0;
  }
}

mips_error ADDU (reg &rs, reg &rt, reg &rd) {
  //adds two regs, stores result in reg
  //no overflow
  rd = rs + rt;
  return 0;
}

mips_error AND (reg &rs, reg &rt, reg &rd) {
  //peforms bitwise AND on rs and rt, stores result in rd
  rd = rs & rt;
  return 0;
}

mips_error JR (const reg &rs, int32_t &B_dest, bool &Branch) {
  //Jump Register
  //Address exception error can occur **********************************************
  B_dest = rs;
  Branch = true;
  return 0;
}

mips_error MFHI (reg &rd, reg &HI) {
  //Move from HI register
  //Copies HI register to rd
  rd = HI;
  return 0;
}

mips_error MFLO (reg &rd, reg &LO) {
  //Move from LO register
  //Copies HI register to rd
  rd = LO;
  return 0;
}

mips_error MTHI (reg &rs, reg &HI) {
  //Move to HI register
  HI = rs;
  return 0;
}

mips_error MTLO (reg &rs, reg &LO) {
  //Move to LO register
  //Copies HI register to rd
  LO = rs;
  return 0;
}

mips_error OR (reg &rs, reg &rt, reg &rd) {
  //performs bitwise OR between rs and rt, stores in rd
  rd = rs | rt;
  return 0;
}

mips_error SLL (const reg &rt, reg &rd, uint32_t sa) {
  //Shift word left logical
  //Contents of rt are shifted left sa times
  rd = rt << sa;
  return 0;
}

mips_error SLLV (const reg &rs, const reg &rt, reg &rd) {
  //Shift word left logical variable
  //Shifts the contents of rt left, with sign extension
  //Shift amount determined by ls 5 bits of rs, result stored in rd
  int shift = rs & 0x1F;
  rd = rt << shift;

  return 0;
}

mips_error SLT (reg &rs, reg &rt, reg &rd) {
  //Set on less than
  //Compares rs and rt as signed integers and records comparison in rd
  //If rs < rt, then result is true
  if (rs < rt)
    rd = 1;
  else
    rd = 0;

  return 0;
}

mips_error SLTU (reg &rs, reg &rt, reg &rd) {
  //Set on less than unsigned
  //Sets rd to 1 if rs < rt (as unsigned)
  uint32_t u_rs = rs;
  uint32_t u_rt = rt;

  if (u_rs < u_rt)
    rd = 1;
  else
    rd = 0;

  return 0;
}

mips_error SRA (reg &rt, reg &rd, uint32_t sa) {
  //Shift word right arithmetic
  //Shifts the contents of rt right, duplicating the MSB into the new bits
  //Shift amount disctated by shift, result stored in rd

  rd = rt >> sa;
  return 0;
}

mips_error SRAV (reg &rs, reg &rt, reg &rd) {
  //Shift word right arithmetic variable
  //Shifts the contents of rt right, with sign extension
  //Shift amount determined by ls 5 bits of rs, result stored in rd
  int shift = rs & 0x1F;
  rd = rt >> shift;

  return 0;
}

mips_error SRL (reg &rt, reg &rd, uint32_t sa) {
  //Shift word right logical
  //Shifts contents right (inserting zeros) by amount from sa
  //Result places in rd
  int32_t temp = rt;
  for (unsigned int i=0; i<sa ;i++) {
    temp = temp >> 1;
    temp = temp & 0x7FFFFFFF;
  }
  rd = temp;
  return 0;
}

mips_error SUB (reg &rs, reg &rt, reg &rd) {
  //subtract
  //can overflow (e.g. if subtracting a negative number from a positive gives a negative)
  int32_t result = rs - rt;
  if (((rs >= 0) && (rt < 0) && (result < 0)) || ((rs < 0) && (rt >= 0) && (result >= 0))) {   //checks for overflow + underflow
    return -10;
  }
  else {
    rd = result;
    return 0;
  }
}

mips_error SUBU (reg &rs, reg &rt, reg &rd) {
  //subtract unsigned
  rd = rs - rt;
  return 0;
}

mips_error XOR (const reg &rs,const reg &rt, reg &rd) {
  //exclusive OR
  rd = rs ^ rt;
  return 0;
}

mips_error SRLV (const reg &rs, const reg &rt, reg &rd) {
  //Shift word right logical variable
  //rt is shifted right with zero-extension by ls 5 bits of rs and stored in rd
  int shift_a = rs & 0x1F;
  uint32_t temp = rt;
  // for  (int i=0; i < shift_a; i++) {
  //   temp = temp >> 1;
  //   temp = temp & 0x7FFFFFFF;
  // }
  // rd = temp;
  rd = temp >> shift_a;
  return 0;
}

mips_error JALR(const reg &rs, reg &rd, const int32_t &PC, int32_t &B_dest, bool &Branch){

  int32_t temp = rs;

  rd = PC + 8;

  B_dest = temp;
  Branch = true;

  // if(rs % 4 != 0){
  //
  //   B_dest = ADDR_DATA; //execute delayed instruction and then updated with memory
  // }

  return 0;

}

mips_error MULT(const reg &rs, const reg &rt, reg &HI, reg &LO){

  int64_t ans = (int64_t) rs * (int64_t) rt;


  HI = (int32_t) (ans >> 32) & 0xFFFFFFFF;
  LO =  (uint32_t) (ans & 0xFFFFFFFF);

  return 0;

}

mips_error MULTU(const reg &rs, const reg &rt, reg &HI, reg &LO){

  uint64_t temp_rs = (uint32_t) rs;
  uint64_t temp_rt = (uint32_t) rt;

  uint64_t ans = temp_rs * temp_rt;

  HI = (int32_t) (ans >> 32) & 0xFFFFFFFF;
  LO =  (uint32_t) (ans & 0xFFFFFFFF);

  return 0;

}

mips_error DIV(const reg &rs, const reg &rt, reg &HI, reg &LO){

int32_t dividend = rs;
int32_t divisor = rt;

if(divisor != 0){
  HI = dividend % divisor;
  LO = dividend / divisor;
}
return 0;
}

mips_error DIVU(const reg &rs, const reg &rt, reg &HI, reg &LO){

  uint32_t dividend = rs;
  uint32_t divisor = rt;

  if(divisor != 0){
    HI = dividend % divisor;
    LO = dividend / divisor;
  }
  return 0;
  }


//I type----------------------------------------------------------------------------------------------
mips_error ADDI (reg &rs, reg &rt, int16_t imm) {
  //adds reg and sign-extended immediate value, stores result in reg
  //has overflow
  int32_t se_imm = imm;   //sign extend immediate value
  int32_t result = rs + se_imm;
  if ((rs>0) && (se_imm > 0) && (result <= 0)) {
    return -10;
  }
  else if ((rs < 0) && (se_imm < 0) && (result >= 0)) {
    return -10;
  }
  // if (((rs > 0) && (rt > INT_MAX - rs) && (se_imm > 0) ) || ((rs < 0) && (rt < INT_MIN - rs) && (se_imm < 0))) {
  //   return -10;
  // }
  else {
    rt = result;
    return 0;
  }
}

mips_error ADDIU (reg &rs, reg &rt, int16_t imm) {
  //adds rs and sign-extended immediate value, stores result in rt
  //no overflow
  int32_t se_imm = imm;   //sign extend immediate value
  rt = rs + se_imm;
  return 0;
}

mips_error ANDI (reg &rs, reg &rt, int16_t imm) {
  //performs bitwise AND with rs and immediate (zero-extended), stores in rt
  int32_t ze_imm = imm & 0x0000FFFF;    //zero-extension
  rt = rs & ze_imm;
  return 0;
}

mips_error BEQ (const reg &rs, const reg &rt, const int16_t &imm, const uint32_t &B_del, int32_t &B_dest, bool &Branch) {
  //Branch on equal
  //Offset is imm left shifted by 2, and added to address of instruction FOLLOWING BRANCH (not the branch), found in the branch delay slot
  //If rs == rt, then branch to the effective target address after the instrction in the delay slot is executed
  uint32_t Branch_address = B_del + (imm << 2);
  if (rs == rt) {
    B_dest = Branch_address;
    Branch = true;
  }
  return 0;
}

mips_error BGEZ (const reg &rs, const int16_t &imm, const uint32_t &B_del, int32_t &B_dest, bool &Branch) {
  //Branch on >= 0
  //As in BEQ
  uint32_t Branch_address = B_del + (imm << 2);
  if (rs >= 0) {
    B_dest = Branch_address;
    Branch = true;
  }
  return 0;
}

mips_error BGEZAL (const reg &rs, const int16_t &imm, const uint32_t &B_del, int32_t &B_dest, bool &Branch, uint32_t PC, reg &r31){
  //branches if rs >=0
  //need to confirm operation of PC+8
  //uint32_t se_offset = imm;
  uint32_t Branch_address = B_del + (imm << 2);
  // if(id_1 == 31){
  //   return 0;
  // }
  if (rs >= 0) {
    B_dest = Branch_address;
    Branch = true;
  }
  r31 = PC + 8;
  return 0;
}

mips_error BLTZAL (const reg &rs, const int16_t &imm, const uint32_t &B_del, int32_t &B_dest, bool &Branch, uint32_t PC, reg &r31){
  //branches if rs < 0
  //need to confirm operation of PC+8
  uint32_t Branch_address = B_del + (imm << 2);

  // if(instruction_data[1] == 31){
  //   return -20; //internal error of simulator
  // }

  if (rs < 0) {
    B_dest = Branch_address;
    Branch = true;
  }
  r31 = PC + 8;
  return 0;
}

mips_error BGTZ (const reg &rs, const int16_t &imm, const uint32_t &B_del, int32_t &B_dest, bool &Branch) {
  //Branch on > 0
  //As in BEQ
  uint32_t Branch_address = B_del + (imm << 2);
  if (rs > 0) {
    B_dest = Branch_address;
    Branch = true;
  }
  return 0;
}

mips_error BLEZ (const reg &rs, const int16_t &imm, const uint32_t &B_del, int32_t &B_dest, bool &Branch) {
  //Branch on rs <= 0
  //As in BEQ
  uint32_t Branch_address = B_del + (imm << 2);
  if (rs <= 0) {
    B_dest = Branch_address;
    Branch = true;
  }
  return 0;
}

mips_error BLTZ (const reg &rs, const int16_t &imm, const uint32_t &B_del, int32_t &B_dest, bool &Branch) {
  //Branch on rs < 0
  //As in BEQ
  uint32_t Branch_address = B_del + (imm << 2);
  if (rs < 0) {
    B_dest = Branch_address;
    Branch = true;
  }
  return 0;
}

mips_error BNE (const reg &rs, const reg &rt, const int16_t &imm, const uint32_t &B_del, int32_t &B_dest, bool &Branch) {
  //Branch on rs != rt
  uint32_t Branch_address = B_del + (imm << 2);
  if (rs != rt) {
    B_dest = Branch_address;
    Branch = true;
  }
  return 0;
}

mips_error LB (const reg &base, reg &rt, int16_t offset, Memory &MIPS_Memory) {
  //Load byte
  //Contents of 8-bit byte at memory address [base + offset] fetched, signed extended and stored in rt
  int32_t se_offset = offset;
  int32_t eff_addr = se_offset + base;
  int8_t byte_r;
  mips_error mem_error = MIPS_Memory.data_read_b(eff_addr, byte_r);
  rt = (int32_t)byte_r;
  return mem_error;
}

mips_error LBU (const reg &base, reg &rt, int16_t offset, Memory &MIPS_Memory) {
  //Load byte unsigned
  //Contents of 8-bit byte at memory address [base + offset] fetched, zero extended and stored in rt
  int32_t se_offset = offset;
  int32_t eff_addr = se_offset + base;
  // int32_t error = data_access (ADDR_DATA, eff_addr, rt, 'R', false);
  int8_t byte_r;
  mips_error mem_error = MIPS_Memory.data_read_b(eff_addr, byte_r);
  rt = (int32_t)byte_r;
  rt = rt & 0x000000FF;    //ensures zero extension
  return mem_error;
}

mips_error LH (const reg &base, reg &rt, int16_t offset, Memory &MIPS_Memory) {
  //Load halfword
  //Finds effective address from base + offset (sign extended) and places memory contents into rt
  //Memory exception if ls bit are non-zero
  int32_t se_offset = offset;
  int32_t eff_addr = se_offset + base;
  if ((eff_addr & 0x1) != 0) {
    return -11;
  }
  int16_t char_r;
  mips_error mem_error = MIPS_Memory.data_read_hw(eff_addr, char_r);
  rt = (int32_t)char_r;
  return mem_error;
}

mips_error LHU (const reg &base, reg &rt, int16_t offset, Memory &MIPS_Memory) {
  //Load halfword unsigned
  //Finds effective address from base + offset (sign extended) and places memory contents into rt
  //Memory exception if ls bit are non-zero
  int32_t se_offset = offset;
  int32_t eff_addr = se_offset + base;
  if ((eff_addr & 0x1) != 0) {
    return -11;
  }
  int16_t char_r;
  mips_error mem_error = MIPS_Memory.data_read_hw(eff_addr, char_r);
  rt = (int32_t)char_r;
  rt = rt & 0x0000FFFF;   //for zero extension
  return mem_error;
}

mips_error LUI (reg &rt, int16_t imm) {
  //Load upper immediate
  //16 bit imm is shifted left 16 bits and concatenated with low 16 bits of zeros, stored in rt
  rt = imm;
  rt = (rt << 16) & 0xFFFF0000;
  return 0;
}


mips_error LWL (reg &base, reg &rt, int16_t imm, Memory &MIPS_Memory) {
  //Load upper immediate
  //16 bit imm is shifted left 16 bits and concatenated with low 16 bits of zeros, stored in rt
  uint32_t eff_addr = imm + base;
  int32_t pre_rt;
  mips_error mem_error = 0;
  int bytecount = eff_addr % 4;   //least significant bytes from the complete word
  switch (bytecount) {
    case 0: return MIPS_Memory.data_read_w(eff_addr, rt);
    break;

    case 1:
    mem_error = MIPS_Memory.data_read_w(eff_addr - 1, pre_rt);
    if (mem_error != 0 ) return mem_error;
    pre_rt = pre_rt << 8;
    rt = rt & 0xFF;
    rt += pre_rt;
    return mem_error;
    break;

    case 2:
    mem_error = MIPS_Memory.data_read_w(eff_addr - 2, pre_rt);
    if (mem_error != 0 ) return mem_error;
    pre_rt = pre_rt << 16;
    rt = rt & 0xFFFF;
    rt += pre_rt;
    return mem_error;
    break;

    case 3:
    mem_error = MIPS_Memory.data_read_w(eff_addr - 3, pre_rt);
    if (mem_error != 0 ) return mem_error;
    pre_rt = pre_rt << 24;
    rt = rt & 0xFFFFFF;
    rt += pre_rt;
    return mem_error;
    break;
  }
  if (mem_error != 0 ) 
    return mem_error;
  else 
    return 0;
}

mips_error LWR (reg &base, reg &rt, int16_t imm, Memory &MIPS_Memory) {
  //Load upper immediate
  //16 bit imm is shifted left 16 bits and concatenated with low 16 bits of zeros, stored in rt
  uint32_t eff_addr = imm + base;
  int32_t pre_rt;
  mips_error mem_error;
  int bytecount = eff_addr % 4;   //least significant bytes from the complete word
  switch (bytecount) {
    case 3: return MIPS_Memory.data_read_w(eff_addr - 3, rt);
    break;

    case 2:
    mem_error = MIPS_Memory.data_read_w(eff_addr - 2, pre_rt);
    if (mem_error != 0 ) return mem_error;
    pre_rt = pre_rt >> 8;
    pre_rt = pre_rt & 0xFFFFFF;
    rt = rt & 0xFF000000;
    rt += pre_rt;
    return mem_error;
    break;

    case 1:
    mem_error = MIPS_Memory.data_read_w(eff_addr - 1, pre_rt);
    if (mem_error != 0 ) return mem_error;
    pre_rt = pre_rt >> 16;
    pre_rt = pre_rt & 0xFFFF;
    rt = rt & 0xFFFF0000;
    rt += pre_rt;
    return mem_error;
    break;

    case 0:
    mem_error = MIPS_Memory.data_read_w(eff_addr, pre_rt);
    if (mem_error != 0 ) return mem_error;
    pre_rt = pre_rt >> 24;
    pre_rt = pre_rt & 0xFF;
    rt = rt & 0xFFFFFF00;
    rt += pre_rt;
    return mem_error;
  }


  return 0;
}

mips_error LW (const reg &base, reg &rt, int16_t offset, Memory &MIPS_Memory) {
  //Load Word
  //Finds effective address from base + offset (sign extended) and places memory contents into rt
  //Memory exception if ls 2 bits are non-zero
  int32_t se_offset = offset;
  int32_t eff_addr = se_offset + base;
  // return data_access (ADDR_DATA, eff_addr, rt, 'R', true);
  return MIPS_Memory.data_read_w(eff_addr, rt);
}

mips_error ORI (reg &rs, reg &rt, int16_t imm) {
  //bitwise OR between rs and immediate (zero-extended), stores in rt
  int32_t ze_imm = imm & 0x0000FFFF;    //zero-extension
  rt = rs | ze_imm;
  return 0;
}

mips_error SB (const reg &base, reg &rt, int16_t offset, Memory &MIPS_Memory) {
  //Store byte
  //LS 8 bit byte from rt is stored in memory at address base + offset
  int32_t se_offset = offset;
  int32_t eff_addr = se_offset + base;
  int8_t st_reg = (int8_t)(rt & 0xFF);
  return MIPS_Memory.data_write_b(eff_addr, st_reg);

}

mips_error SH (const reg &base, reg &rt, int16_t offset, Memory &MIPS_Memory) {
  //Store half word
  //LS 16 bits of rt is stored at address base + offset
  int32_t se_offset = offset;
  int32_t eff_addr = se_offset + base;
  //ls 8 bits
  int16_t st_reg = (int16_t)(rt & 0xFFFF);
  return MIPS_Memory.data_write_hw(eff_addr, st_reg);
}

mips_error SLTI (reg &rs, reg &rt, int16_t imm) {
  //Set on less than immediate signed
  //Sets rt to 1 if rs < immediate (signed)
  int32_t se_imm = imm;
  if (rs < se_imm)
    rt = 1;
  else
    rt = 0;

  return 0;
}

mips_error SLTIU (reg &rs, reg &rt, int16_t imm) {
  //Set on less than immediate unsigned
  //Sets rt to 1 if rs < immediate (sign extended)
  uint32_t u_rs = rs;
  int32_t se_imm = imm;
  uint32_t u_imm = se_imm;
  if (u_rs < u_imm)
    rt = 1;
  else
    rt = 0;

  return 0;
}

mips_error SW (const reg &base, reg &rt, int16_t offset, Memory &MIPS_Memory) {
  //Store word
  //Stores ls 32-bit word of rt in memory at location specified by effective address
  //Effective address is found by contents of base + offset
  //ADDRESS ERROR EXCEPTION CAN OCCUR: lower order 2 bits of address must be zero else exception -11 (byte addressing);
  int32_t se_offset = offset;
  int32_t eff_addr = se_offset + base;

  // return data_access (ADDR_DATA, eff_addr, rt, 'W', true);
  return MIPS_Memory.data_write_w(eff_addr, rt);
}

mips_error XORI (const reg &rs, reg &rt, int16_t imm) {
  //Exclusive OR immediate
  //Performs bitwise XOR between rs and zero-extended immediate, stores in rt
  int32_t ze_imm = imm & 0x0000FFFF;
  rt = rs ^ ze_imm;
  return 0;
}


//J type -------------------------------------------------------------------------------------------------------------------
mips_error J (uint32_t &instr_index, const uint32_t &B_del, int32_t &B_dest, bool &Branch) {
  //Jump within current 256MB region
  //Low 28 bits is instr_index left shifted by 2
  //Remaining upper bits are the corresponding upper bits of the address in the branch delay slot
  //Is a delayed branch
  B_dest = B_del & 0xF0000000;
  B_dest += instr_index << 2;
  Branch = true;
  return 0;
}

mips_error JAL (reg &r31, const uint32_t &B_del, int32_t &B_dest, bool &Branch, const int32_t &PC, uint32_t &instr_index){
  //To procedure call within current 256MB aligned region
  //need to confirm changing PC by 8
  r31 = PC + 8;
  B_dest = B_del & 0xF0000000;
  B_dest += instr_index << 2;
  Branch = true;
  return 0;
}
