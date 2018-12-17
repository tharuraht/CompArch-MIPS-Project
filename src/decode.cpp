/*
DECODE
Instruction is accepted and then decoded and separates the bits into 3 function fields - 3 types: R, I, J
*/

#include "decode.hpp"

using namespace std;

//Initial function that determines the TYPE of instruction
  mips_error mips_decode(const uint32_t& instruction, uint32_t* instruction_data){
	uint32_t opcode = instruction >> 26;
	if (opcode==0){
		return mips_decode_R(instruction, instruction_data);
  }
	else if ((opcode==2) || (opcode==3)){
		return mips_decode_J(instruction, instruction_data);
  }
	else{
		return mips_decode_I(instruction, instruction_data);
  }
}



//DECODE R TYPE
  mips_error mips_decode_R(const uint32_t& instruction, uint32_t* instruction_data){
	instruction_data[0] = (uint32_t)(instruction >> 26); 																		       //OP
	instruction_data[1] = (uint32_t)((instruction & 0x03E00000) >> 21) ;                           //rs
	instruction_data[2] = (uint32_t)((instruction & 0x1F0000) >> 16); 														 //rt
	instruction_data[3] = (uint32_t)((instruction & 0xF800) >> 11); 															 //rd
	instruction_data[4] = (uint32_t)((instruction & 0x7C0) >> 6)  ;                                //shamt
	instruction_data[5] = (uint32_t)(instruction & 0x3F); 																	       //function
	return 0;
}

//DECODE I TYPE
mips_error mips_decode_I(const uint32_t& instruction, uint32_t* instruction_data){
	instruction_data[0] = (uint32_t)(instruction >> 26); 																		        //OP
	instruction_data[1] = (uint32_t)((instruction & 0x03E00000) >> 21);                             //rs
	instruction_data[2] = (uint32_t)((instruction & 0x1F0000) >> 16);																//rt
	instruction_data[3] = (uint32_t)(instruction & 0xFFFF);                                         //Immediate Constant
	instruction_data[4] = 0; 																																				//Leave Blank
	instruction_data[5] = 0; 																																				//Leave Blank
	return 0;
}

//DECODE J TYPE
mips_error mips_decode_J(const uint32_t& instruction, uint32_t* instruction_data){
	instruction_data[0] = (uint32_t)(instruction >> 26); 																		        //OP
	instruction_data[1] = (uint32_t)(instruction & 0x3FFFFFF) ; 																		//Memory address
	instruction_data[2] = 0; 																																				//empty
	instruction_data[3] = 0; 																																				//empty
	instruction_data[4] = 0; 																																				//empty
	instruction_data[5] = 0; 																																				//empty
	return 0;
}
