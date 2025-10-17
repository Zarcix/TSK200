#ifndef TSK_MISC_PARSE_INSTRUCTION
#define TSK_MISC_PARSE_INSTRUCTION

#include "../../tsk/instruction.h"

Instruction parse_single_instruction(char instruction[]);

void parse_instruction_list(int instructionCount, char* instructionList[instructionCount], Instruction instructionWriteList[]);

#endif