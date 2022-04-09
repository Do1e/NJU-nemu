#include "cpu/instr.h"
/*
Put the implementations of `push' instructions here.
*/

static void instr_execute_1op(){
	OPERAND dest;
	operand_read(&opr_src);
	cpu.esp -= data_size / 8;
	dest.data_size = data_size;
	dest.type = OPR_MEM;
	dest.addr = cpu.esp;
	dest.val = opr_src.val;
	operand_write(&dest);
}

make_instr_impl_1op(push, r, v);
make_instr_impl_1op(push ,rm, v);
make_instr_impl_1op(push ,i, b);
make_instr_impl_1op(push ,i, v);

void PUSHONE(uint32_t data){
	OPERAND dest;
	cpu.esp -= data_size / 8;
	dest.data_size = data_size;
	dest.type = OPR_MEM;
	dest.addr = cpu.esp;
	dest.val = data;
	operand_write(&dest);
	print_asm_1("push", "", 1, &dest);
}

make_instr_func(pusha){
	OPERAND temp;
	temp.data_size = data_size;
	temp.val = cpu.esp;

	PUSHONE(cpu.eax);
	PUSHONE(cpu.ecx);
	PUSHONE(cpu.edx);
	PUSHONE(cpu.ebx);
	PUSHONE(temp.val);
	PUSHONE(cpu.ebp);
	PUSHONE(cpu.esi);
	PUSHONE(cpu.edi);
	return 1;
}