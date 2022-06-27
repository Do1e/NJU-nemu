#include "cpu/instr.h"
/*
Put the implementations of `sti' instructions here.
*/

make_instr_func(sti){
	cpu.eflags.IF = 0;
	print_asm_0("sti", "", 1);
	return 1;
}
