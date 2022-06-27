#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/

make_instr_func(pop_r_v){
	OPERAND src, dest;
	src.data_size = data_size;
	src.type = OPR_MEM;
	src.addr = cpu.esp;
	src.sreg = SREG_SS;
	operand_read(&src);

	dest.data_size = data_size;
	dest.type = OPR_REG;
	dest.addr = opcode & 0x7;
	dest.val = src.val;
	dest.sreg = SREG_SS;
	operand_write(&dest);

	cpu.esp += data_size / 8;
	print_asm_1("pop", opr_src.data_size == 8 ? "b" : (opr_src.data_size == 16 ? "w" : "l"), 1, &dest);
	return 1;
}

void POPONE(uint32_t *data){
	OPERAND src;
	src.data_size = data_size;
	src.type = OPR_MEM;
	src.addr = cpu.esp;
	operand_read(&src);
	*data = src.val;
	cpu.esp += data_size / 8;
	print_asm_1("pop", opr_src.data_size == 8 ? "b" : (opr_src.data_size == 16 ? "w" : "l"), 1, &src);
}

make_instr_func(popa){
	POPONE(&cpu.edi);
	POPONE(&cpu.esi);
	POPONE(&cpu.ebp);
	cpu.esp += data_size / 8;
	// POPONE(&cpu.esp);
	POPONE(&cpu.ebx);
	POPONE(&cpu.edx);
	POPONE(&cpu.ecx);
	POPONE(&cpu.eax);
	return 1;
}
