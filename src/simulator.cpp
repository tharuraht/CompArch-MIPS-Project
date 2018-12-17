#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdint.h>

//debug library
// #include <bitset>

#include "decode.cpp"
#include "instructions.cpp"
#include "instr_ex.cpp"
#include "memory.hpp"


using namespace std;

int main (int argc, char* argv[]) {
    //Initialisation of PC, memory and registers
    uint32_t PC = 0x10000000;
    uint32_t B_del = PC + 4;                  //Branch delay slot

    int32_t mips_registers [32] = { };
    int32_t HI = 0;
    int32_t LO = 0;

    bool Branch = false;

    int32_t B_dest;

    //Function returns
    int32_t instr_range_error;
    int32_t decode_error;
    int32_t ex_error;


    //opens and load binary file into memory via array
    string binary_file;
    if (argc > 1) {
    binary_file = argv[1];
    }
    else {
      // cout<<"no argument passed"<<endl;
      exit (-21);
    }

    ifstream binary;
    binary.open (binary_file.c_str(), ios::out | ios::binary | ios::ate);
    if (!binary.is_open()) {
      // cout<<"doesnt open"<<endl;
      exit(-21);
    }


    Memory MIPS_Memory (binary);
    binary.close(); //closes file as no longer in use

    // int in = 0;

    uint32_t instruction;
    uint32_t* instruction_data = new uint32_t[6] ();
    while (PC != 0) {
      instr_range_error = MIPS_Memory.fetch_instruction(PC,instruction);
      if (instr_range_error != 0) {
        exit (instr_range_error);
      }

      // TEST ----------------------------------------------------------------------------------------------
      // cout<<"PC: "<<PC<<endl;
      // bitset<32> bintest(instruction);
      // cout<<"instruction is: "<<bintest<<endl;
      // cout<<"B_dest: "<<B_dest<<endl;
      // cout<<"B_del: "<<B_del<<endl;
      // ---------------------------------------------------------------------------------------------------

      //Decode to determine which instruction
      decode_error = mips_decode(instruction, instruction_data);

      if(decode_error !=0){           //exits program if error with decoding file
        // cout<<"decode error"<<endl;
        exit(decode_error);
      }

      // Reads every register bitwise
      // cout<<"Registers"<<endl;
      // for (int i =0 ; i < 31; i++) {
      //   // bitset<32>bitdata (mips_registers[i]);
      //   // cout<<i<<": "<<bitdata<<endl;
      //   cout<<i<<": "<<mips_registers[i]<<endl;
      // }
      // cout<<"end"<<endl;

      // bitset<32> in_b (instruction);
      // cout << "instruction "<<in++ <<": "<<in_b<< endl;

      //Execute instruction by calling function
      ex_error = ex_instructions(mips_registers, instruction_data, MIPS_Memory, PC, B_del, B_dest, Branch, HI, LO);
      if (ex_error != 0) {
        // cout<<"ex error"<<endl;                                                                  //Checks if instruction had exception
        exit (ex_error);
      }


      //Branch condition
      if (Branch) {
        PC = B_del;
        B_del = B_dest;
        Branch = false;
      }
      else {  //Normal PC progression
        PC = B_del;
        B_del = PC + 4;
      }
    }


    //Termination of simulator: return value is given
    unsigned int exit_code = mips_registers[2] & 0xFF;
    // cout<<"return "<<exit_code<<endl;
    exit(exit_code);
}
