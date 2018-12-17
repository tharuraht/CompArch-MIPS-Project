//This class contains the class containing the memory
//This ensures memory is protected from unexpected interfaces
#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <vector>
#include <stdint.h>
#include <iostream>

typedef int32_t mips_error;

class Memory {
private:
  std::vector <uint8_t> ADDR_INSTR;
  std::vector <uint8_t> ADDR_DATA;

public:
  Memory (std::ifstream &binary); //to initialise binary in instruction memory
  mips_error fetch_instruction (uint32_t PC, uint32_t &instruction);
  mips_error data_read_w (uint32_t address, int32_t &data);
  mips_error data_read_hw (uint32_t address, int16_t &data);
  mips_error data_read_b (uint32_t address, int8_t &data);

  mips_error data_write_w (uint32_t address, int32_t data);
  mips_error data_write_hw (uint32_t address, int16_t data);
  mips_error data_write_b (uint32_t address, int8_t data);
};

#endif
