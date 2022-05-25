#include "cpu/instr.h"
/*
Put the implementations of `lgdt' instructions here.
*/

make_instr_func(lgdt){
	OPERAND src;
	int len = 1;
	src.data_size = data_size;
	len += modrm_rm(eip + 1, &src);
	src.data_size = 16;
	src.type = OPR_MEM;
	operand_read(&src);
	cpu.gdtr.limit = src.val & 0xffff;

	src.data_size = 32;
	src.addr += 2;
	operand_read(&src);
	cpu.gdtr.base = src.val;
	print_asm_1("lgdt", "", len + 1, &src);
	return len;
}