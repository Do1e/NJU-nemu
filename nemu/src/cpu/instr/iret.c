#include "cpu/instr.h"
/*
Put the implementations of `iret' instructions here.
*/

make_instr_func(iret){
	OPERAND sta;
	sta.type = OPR_MEM;
	sta.data_size = data_size;
	sta.sreg = SREG_SS;
	
	sta.addr = cpu.esp;
	operand_read(&sta);
	print_asm_0("iret", "", 1);
	cpu.eip = sta.val;
	cpu.esp += data_size / 8;

	sta.addr = cpu.esp;
	operand_read(&sta);
	cpu.cs.val = sta.val & 0xffff;
	cpu.esp += data_size / 8;

	sta.addr = cpu.esp;
	operand_read(&sta);
	cpu.eflags.val = sta.val;
	cpu.esp += data_size / 8;

	return 0;
}