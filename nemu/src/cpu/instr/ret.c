#include "cpu/instr.h"
/*
Put the implementations of `ret' instructions here.
*/


make_instr_func(ret_near){
	OPERAND sta;
	sta.type = OPR_MEM;
	sta.data_size = data_size;
	sta.addr = cpu.esp;
	sta.sreg = SREG_SS;
	operand_read(&sta);
	cpu.esp += data_size / 8;
	print_asm_0("ret", "", 1);
	cpu.eip = sta.val;
	return 0;
}

make_instr_func(ret_near_imm16){
	OPERAND sta, imm;
	imm.type = OPR_IMM;
	imm.data_size = 16;
	imm.addr = eip + 1;
	imm.sreg = SREG_SS;
	operand_read(&imm);

	sta.type = OPR_MEM;
	sta.data_size = data_size;
	sta.addr = cpu.esp;
	sta.sreg = SREG_SS;
	operand_read(&sta);

	imm.val = sign_ext(imm.val, 16);
	cpu.esp += (data_size / 8 + imm.val);
	cpu.eip = sta.val;
	print_asm_0("ret", "", 1);
	return 0;
}
