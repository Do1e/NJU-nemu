#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/

make_instr_func(call_near){
	OPERAND imm, dest;
	imm.type = OPR_IMM;
	imm.data_size = data_size;
	imm.addr = eip + 1;
	imm.sreg = SREG_CS;
	operand_read(&imm);

	dest.type = OPR_MEM;
	dest.data_size = data_size;
	cpu.esp -= data_size / 8;
	dest.addr = cpu.esp;
	dest.val = cpu.eip + data_size / 8 + 1;
	dest.sreg = SREG_SS;
	operand_write(&dest);

	int offset = sign_ext(imm.val, data_size);
	print_asm_1("call", "", 1 + data_size / 8, &imm);
	cpu.eip += offset;
	return data_size / 8 + 1;
}

make_instr_func(call_near_indirect){
	int len = 1;
	OPERAND rm, esp, dest;
	rm.data_size = data_size;
	len += modrm_rm(eip + 1, &rm);
	operand_read(&rm);

	esp.data_size = data_size;
	esp.type = OPR_REG;
	esp.addr = REG_ESP;
	operand_read(&esp);
	esp.val -= 4;
	operand_write(&esp);

	dest.data_size = data_size;
	dest.type = OPR_MEM;
	dest.addr = esp.val;
	dest.val = cpu.eip + len;
	dest.sreg = SREG_CS;
	operand_read(&dest);

	print_asm_1("call", "", 1 + data_size / 8, &rm);
	cpu.eip = rm.val;
	return 0;
}
