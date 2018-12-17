
mips_error ex_instructions (int32_t* mips_registers, uint32_t* instruction_data, Memory &MIPS_Memory, uint32_t PC, uint32_t B_del, int32_t &B_dest, bool &Branch, int32_t &HI,int32_t &LO) {
  int32_t error_code;
  int32_t& regf_0 = mips_registers[0];

  if (instruction_data[0] == 0) {       //R type ------------------------------------------------------------------------------------------
    //Extract registers
    int32_t& regf_rs = mips_registers[instruction_data[1]];
    int32_t& regf_rt = mips_registers[instruction_data[2]];
    int32_t& regf_rd = mips_registers[instruction_data[3]];

    int32_t sa = instruction_data[4];

    int32_t rs = regf_rs;
    int32_t rt = regf_rt;
    int32_t rd = regf_rd;

    //Pick instruction based on function code
    switch (instruction_data[5]) {
      case 0b100000: error_code = ADD (rs, rt, rd);
      break;
      case 0b100001: error_code = ADDU (rs, rt, rd);
      break;
      case 0b100100: error_code = AND (rs, rt, rd);
      break;
      case 0b011010: error_code = DIV(rs, rt, HI, LO);
      break;
      case 0b011011: error_code = DIVU(rs, rt, HI, LO);
      break;
      case 0b001001: error_code = JALR (rs, rd, PC, B_dest, Branch);
      break;
      case 0b001000: error_code = JR (rs, B_dest, Branch);
      break;
      case 0b010000: error_code = MFHI (rd, HI);
      break;
      case 0b010010: error_code = MFLO (rd, LO);
      break;
      case 0b011000: error_code = MULT(rs, rt, HI, LO);
      break;
      case 0b011001: error_code = MULTU(rs, rt, HI, LO);
      break;
      case 0b010001: error_code = MTHI (rs, HI);
      break;
      case 0b010011: error_code = MTLO (rs, LO);
      break;
      case 0b100101: error_code = OR (rs, rt, rd);
      break;
      case 0b000000: error_code = SLL (rt, rd, sa);
      break;
      case 0b000100: error_code = SLLV (rs, rt, rd);
      break;
      case 0b101010: error_code = SLT (rs, rt, rd);
      break;
      case 0b101011: error_code = SLTU (rs, rt, rd);
      break;
      case 0b000011: error_code = SRA (rt, rd, sa);
      break;
      case 0b000111: error_code = SRAV (rs, rt, rd);
      break;
      case 0b000010: error_code = SRL (rt, rd, sa);
      break;
      case 0b100010: error_code = SUB (rs, rt, rd);
      break;
      case 0b100011: error_code = SUBU (rs, rt, rd);
      break;
      case 0b100110: error_code = XOR (rs, rt, rd);
      break;
      case 0b000110: error_code = SRLV (rs, rt, rd);
      break;

      default: error_code = -12;                       //default case is when function code is invalid

    }
    //Updates register file
    if (&regf_rs != &regf_0) {                                                //ignore changes to zero register
      regf_rs = rs;
    }
    if (&regf_rt != &regf_0) {
      regf_rt = rt;
    }
    if (&regf_rd != &regf_0) {
      regf_rd = rd;
    }
  }

  else if ((instruction_data[0] == 0b000010) || (instruction_data[0] == 0b000011)) {    //J type is 00001x ----------------------
    uint32_t& instr_index = instruction_data[1];
    if (instruction_data[0] == 0b000010) {
      error_code = J (instr_index, B_del, B_dest, Branch);
    }
    else if (instruction_data[0] == 0b000011) {
      error_code = JAL (mips_registers[31], B_del, B_dest, Branch, PC, instr_index);
    }
  }

  else if ((instruction_data[0] & 0x3C) == 0b010000) {       //ignore 0100xx (different type) ----------------------------------
    //straight to invalid opcode
    error_code = -12;

  }
  else {  //I type is everything else ----------------------------------------------------------------
    //Created references to register file registers
    int32_t& rs = mips_registers[instruction_data[1]];
    int32_t& rt = mips_registers[instruction_data[2]];
    int32_t imm = instruction_data[3];

    // int32_t rs = regf_rs;
    // int32_t rt = regf_rt;

    // bitset<32>out_rs (mips_registers[instruction_data[1]]);
    // cout<<"rs "<<out_rs<<endl;

    switch (instruction_data[0]) {
      case 0b001000: error_code = ADDI (rs, rt, imm);
      break;
      case 0b001001: error_code = ADDIU (rs, rt, imm);
      break;
      case 0b001100: error_code = ANDI (rs, rt, imm);
      break;
      case 0b000100: error_code = BEQ (rs, rt, imm, B_del, B_dest, Branch);
      break;

      case 0b000001: //Special Branch case
      if (instruction_data[2] == 0b00001) {
        //cout<<"imm: "<<imm<<endl;
        error_code = BGEZ (rs, imm, B_del, B_dest, Branch);
      }
      else if (instruction_data[2] == 0b10001) {
        if (&rs == &mips_registers[31]) {
          error_code = 0;                         //undefined if rs is register 31
        }
        else {
          error_code = BGEZAL (rs, imm, B_del, B_dest, Branch, PC, mips_registers[31]);
        }
      }
      else if (instruction_data[2] == 0b10000){
        if (&rs == &mips_registers[31]) {
          error_code = 0;                         //undefined if rs is register 31
        }
        else {
          error_code = BLTZAL (rs, imm, B_del, B_dest, Branch, PC, mips_registers[31]);
        }
      }
      else if (instruction_data[2] == 0b00000) {
        error_code = BLTZ (rs, imm, B_del, B_dest, Branch);
      }
      else {
        error_code = -12;
      }
      break;


      case 0b000111: error_code = BGTZ (rs, imm, B_del, B_dest, Branch);
      break;
      case 0b000110: error_code = BLEZ (rs, imm, B_del, B_dest, Branch);
      break;
      case 0b000101: error_code = BNE (rs, rt, imm, B_del, B_dest, Branch);
      break;
      case 0b100000: error_code = LB (rs, rt, imm, MIPS_Memory);
      break;
      case 0b100100: error_code = LBU (rs, rt, imm, MIPS_Memory);
      break;
      case 0b100001: error_code = LH (rs, rt, imm, MIPS_Memory);
      break;
      case 0b100101: error_code = LHU (rs, rt, imm, MIPS_Memory);
      break;
      case 0b001111: error_code = LUI (rt, imm);
      break;
      case 0b100011: error_code = LW (rs, rt, imm, MIPS_Memory);
      break;
      case 0b100010: error_code = LWL (rs, rt, imm, MIPS_Memory);
      break;
      case 0b100110: error_code = LWR (rs, rt, imm, MIPS_Memory);
      break;
      case 0b001101: error_code = ORI (rs, rt, imm);
      break;
      case 0b101000: error_code = SB (rs, rt, imm, MIPS_Memory);
      break;
      case 0b101001: error_code = SH (rs, rt, imm, MIPS_Memory);
      break;
      case 0b001010: error_code = SLTI (rs, rt, imm);
      break;
      case 0b001011: error_code = SLTIU (rs, rt, imm);
      break;
      case 0b101011: error_code = SW (rs, rt, imm, MIPS_Memory);
      break;
      case 0b001110: error_code = XORI (rs, rt, imm);
      break;


      default: error_code = -12;
    }

    //ignore changes to register zero
    if (&rs == &regf_0){
      rs = 0;
    }
    if (&rt == &regf_0) {
      rt = 0;
    }
  }

  return error_code;
}
