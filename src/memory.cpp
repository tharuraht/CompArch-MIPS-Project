#include "memory.hpp"

Memory::Memory (std::ifstream &binary) {
  //Assigns memory space to vectors (in bytes)
  ADDR_INSTR.assign(0x1000000, 0);
  ADDR_DATA.assign(0x4000000, 0);

  int bin_size = binary.tellg();
  char bin_array [bin_size];
  binary.seekg(0, std::ios::beg);
  binary.read (bin_array, bin_size);     //load into char array, then convert into a vector

  //copies into correct 32-bit vector
  for (unsigned int i = 0; (i < sizeof(bin_array)); i++) {
    ADDR_INSTR[i] = bin_array[i];
    // test
    // bitset<8> printbin(ADDR_INSTR[i]);
    // cout<<"Instruction: "<<printbin<<endl;
  }
}

mips_error Memory::fetch_instruction (uint32_t PC, uint32_t &instruction) {
  //function takes the PC and uses it to fetch the instruction relevent instruction to be executed
  //check for out of memory exception
  if ((PC < 0x10000000 ) || (PC >= 0x11000000 ) || ((PC & 0x3) != 0)) {
    return -11;
  }
  uint32_t instruction_i = PC - 0x10000000;
  instruction = ADDR_INSTR[instruction_i] << 24;
  instruction += ADDR_INSTR[instruction_i + 1] << 16;
  instruction += ADDR_INSTR[instruction_i + 2] << 8;
  instruction += ADDR_INSTR[instruction_i + 3];
  return 0;
}

mips_error Memory::data_read_w (uint32_t address, int32_t &data) {
  if ((address & 0x3) != 0) {
    return -11;
  }
  //I/O interface
  else if ((address >= 0x30000000) && (address <=0x30000003)) {
    int8_t GETC_in = std::getchar();

    if (GETC_in != EOF && !std::cin.eof()) {
      if (!std::cin.good()) {    //exits with I/O error if cin error occurs
        return -21;
      }
      if (address == 0x30000000) {
        data = GETC_in & 0xFF; //only ls byte has char, rest is 0
      }
      else {
        return -11;     //memory exception as word isnt aligned
      }
    }
    else {
      //EOF
      data = 0xFFFFFFFF;
    }
  }
  else if ((address >= 0x20000000 ) && (address < 0x24000000)) {           //data memory
    uint32_t data_i = address - 0x20000000;
    data = ADDR_DATA[data_i] << 24;
    data += ADDR_DATA[data_i + 1] << 16;
    data += ADDR_DATA[data_i + 2] << 8;
    data += ADDR_DATA[data_i + 3];
  }
  else if ((address >= 0x10000000) && (address < 0x11000000)) {           //instruction memory
    uint32_t data_i = address - 0x10000000;
    data = ADDR_INSTR[data_i] << 24;
    data += ADDR_INSTR[data_i + 1] << 16;
    data += ADDR_INSTR[data_i + 2] << 8;
    data += ADDR_INSTR[data_i + 3];
  }
  else {
    return -11;
  }

  return 0;
}

mips_error Memory::data_read_hw (uint32_t address, int16_t &data) {
  if ((address & 0x1) != 0) {            //checks for misalign
    return -11;
  }
  //I/O interface
  else if ((address >= 0x30000000) && (address <=0x30000003)) {
    int8_t GETC_in = std::getchar();
    if (GETC_in != EOF && !std::cin.eof()) {
      if (!std::cin.good()) {    //exits with I/O error if cin error occurs
        return -21;
      }
      if (address == 0x30000002) {
        data = GETC_in & 0xFF; //only ls byte has char, rest is 0
      }
      else {
        data = 0;
      }
    }
    else {
      //EOF
      data = -1;
    }
  }
  else if ((address >= 0x20000000 ) && (address < 0x24000000)) {
    uint32_t data_i = address - 0x20000000;
    data = (ADDR_DATA[data_i] << 8) & 0xFF00;
    data += ADDR_DATA[data_i + 1];
  }
  else if ((address >= 0x10000000) && (address < 0x11000000)) {
    uint32_t data_i = address - 0x10000000;
    data = (ADDR_INSTR[data_i] <<8) & 0xFF00;
    data += ADDR_INSTR[data_i + 1];
  }
  else {
    return -11;
  }
  return 0;
}

mips_error Memory::data_read_b (uint32_t address, int8_t &data) {
  //I/O interface
  if ((address >= 0x30000000) && (address <=0x30000003)) {
    int8_t GETC_in = std::getchar();
    if (GETC_in != EOF && !std::cin.eof()) {
      if (!std::cin.good()) {    //exits with I/O error if cin error occurs
        return -21;
      }
      if (address == 0x30000003) {
        data = GETC_in & 0xFF; //only ls byte has char, rest is 0
      }
      else {
        data = 0;
      }
    }
    else {
      //EOF
      data = -1;
    }
  }
  else if ((address >= 0x20000000 ) && (address < 0x24000000)) {
    uint32_t data_i = address - 0x20000000;
    data = ADDR_DATA[data_i];
  }
  else if ((address >= 0x10000000) && (address < 0x11000000)) {
    uint32_t data_i = address - 0x10000000;
    data = ADDR_INSTR[data_i];
  }
  else {                    //out of range error
    return -11;
  }
  return 0;
}

mips_error Memory::data_write_w (uint32_t address, int32_t data) {
  if ((address & 0x3) != 0) return -11;
  //I/O interface
  else if ((address >= 0x30000004) && (address <=0x30000007)) {
    if (address == 0x30000004) {
      int8_t PUTC_out = (int8_t)(data & 0xFF);
      if (!std::cout.good()) {    //exits with I/O error
        return -21;
      }
      std::putchar(PUTC_out);
    }
    else
      return -11;     //memory exception as word isnt aligned
  }
  else if ((address >= 0x24000000) || (address < 0x20000000)) {  //checks for out of memory range error
    return -11;
  }
  else {
    //remove offset
    uint32_t data_i = address - 0x20000000;
    ADDR_DATA[data_i] = (uint8_t)((data & 0xFF000000) >> 24);
    ADDR_DATA[data_i + 1] = (uint8_t)((data & 0xFF0000) >> 16);
    ADDR_DATA[data_i + 2] = (uint8_t)((data & 0xFF00) >> 8);
    ADDR_DATA[data_i + 3] = (uint8_t)(data & 0xFF);
  }
  return 0;
}

mips_error Memory::data_write_hw (uint32_t address, int16_t data) {
  if ((address & 0x1) != 0) return -11;
  //Checks if writing to I/O interface
  else if ((address >= 0x30000004) && (address <=0x30000007)) {
    if (address == 0x30000006) {
      int8_t PUTC_out = (int8_t)(data & 0xFF);
      if (!std::cout.good()) {    //exits with I/O error
        return -21;
      }
      std::putchar(PUTC_out);
    }
    else if (address == 0x30000004) {
      if (!std::cout.good()) {    //exits with I/O error
        return -21;
      }
      std::putchar(0);
    }
    else {
      return -11;
    }
  }
  else if ((address >= 0x24000000) || (address < 0x20000000)) {  //checks for out of memory range error
    return -11;
  }
  else {
    uint32_t data_i = address - 0x20000000;
    ADDR_DATA[data_i] = (uint8_t)((data & 0xFF00) >> 8);
    ADDR_DATA[data_i + 1] = (uint8_t)(data & 0xFF);
  }
  return 0;
}


mips_error Memory::data_write_b (uint32_t address, int8_t data) {
  //Checks if writing to I/O interface
  if ((address >= 0x30000004) && (address <=0x30000007)) {
    if (address == 0x30000007) {
      if (!std::cout.good()) {    //exits with I/O error
        return -21;
      }
      std::putchar(data);
    }
    else {
      if (!std::cout.good()) {    //exits with I/O error
        return -21;
      }
      std::putchar(0);
    }
  }
  else if ((address >= 0x24000000) || (address < 0x20000000)) {  //checks for out of memory range error
    return -11;
  }
  else {
    uint32_t data_i = address - 0x20000000;
    ADDR_DATA[data_i] = data;
  }
  return 0;
}
