#include "cpu/instr.h"
#include "device/port_io.h"
/*
Put the implementations of `out' instructions here.
*/

make_instr_func(out_b){
	pio_write(cpu.edx, 1, cpu.eax);
	print_asm_0("out", "b", 1);
	return 1;
}

make_instr_func(out_v){
	pio_write(cpu.edx, data_size / 8, cpu.eax);
	print_asm_0("out", (data_size == 32) ? "l" : "w", 1);
	return 1;
}

make_instr_func(out_i_b){
	OPERAND imm;
	imm.type = OPR_IMM;
	imm.data_size = 8;
	imm.addr = eip + 1;
	imm.sreg = SREG_CS;
	operand_read(&imm);
	pio_write(imm.val, 1, cpu.eax);
	print_asm_1("out", "b", 2, &imm);
	return 2;
}

make_instr_func(out_i_v){
	OPERAND imm;
	imm.type = OPR_IMM;
	imm.data_size = data_size;
	imm.addr = eip + 1;
	imm.sreg = SREG_CS;
	operand_read(&imm);
	pio_write(imm.val, data_size / 8, cpu.eax);
	print_asm_1("out", (data_size == 32) ? "l" : "w", 2, &imm);
	return 2;
}