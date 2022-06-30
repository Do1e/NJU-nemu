#include "cpu/instr.h"
#include "device/port_io.h"
/*
Put the implementations of `in' instructions here.
*/

make_instr_func(in_b){
	cpu.eax = pio_read(cpu.edx, 1);
	print_asm_0("in", "b", 1);
	return 1;
}

make_instr_func(in_v){
	cpu.eax = pio_read(cpu.edx, data_size / 8);
	print_asm_0("in",(data_size == 32) ? "l" : "w", 1);
	return 1;
}

make_instr_func(in_i_b){
	OPERAND imm;
	imm.type = OPR_IMM;
	imm.data_size = 8;
	imm.addr = eip + 1;
	imm.sreg = SREG_CS;
	operand_read(&imm);
	cpu.eax = pio_read(imm.val, 1);
	print_asm_1("in", "b", 2, &imm);
	return 2;
}

make_instr_func(in_i_v){
	OPERAND imm;
	imm.type = OPR_IMM;
	imm.data_size = data_size;
	imm.addr = eip + 1;
	imm.sreg = SREG_CS;
	operand_read(&imm);
	cpu.eax = pio_read(imm.val, data_size / 8);
	print_asm_1("in", (data_size == 32) ? "l" : "w", 2, &imm);
	return 2;
}