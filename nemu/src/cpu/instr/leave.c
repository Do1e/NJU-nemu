#include "cpu/instr.h"
/*
Put the implementations of `leave' instructions here.
*/


make_instr_func(leave){
	OPERAND dest;
	dest.type = OPR_MEM;
	dest.addr = cpu.ebp;
	dest.data_size = data_size;

	operand_read(&dest);

	cpu.esp = cpu.ebp;
	cpu.esp += data_size / 8;
	cpu.ebp = dest.val;
	print_asm_0("leave", "", 1);
	return 1;
}