#include "cpu/instr.h"

static void instr_execute_1op_cc_pass()
{
	operand_read(&opr_src);
	cpu.eip += sign_ext(opr_src.val, opr_src.data_size);
}

static void instr_execute_1op_cc_fail() {/* Do nothing */}

#define jcc_helper(suffix) \
	make_instr_impl_1op_cc(j, i, suffix, e) \
	make_instr_impl_1op_cc(j, i, suffix, a) \
	make_instr_impl_1op_cc(j, i, suffix, b) \
	make_instr_impl_1op_cc(j, i, suffix, g) \
	make_instr_impl_1op_cc(j, i, suffix, l) \
	make_instr_impl_1op_cc(j, i, suffix, o) \
	make_instr_impl_1op_cc(j, i, suffix, p) \
	make_instr_impl_1op_cc(j, i, suffix, s) \
	make_instr_impl_1op_cc(j, i, suffix, ae) \
	make_instr_impl_1op_cc(j, i, suffix, ge) \
	make_instr_impl_1op_cc(j, i, suffix, le) \
	make_instr_impl_1op_cc(j, i, suffix, na) \
	make_instr_impl_1op_cc(j, i, suffix, ne) \
	make_instr_impl_1op_cc(j, i, suffix, no) \
	make_instr_impl_1op_cc(j, i, suffix, np) \
	make_instr_impl_1op_cc(j, i, suffix, ns) \

jcc_helper(near)
jcc_helper(short_)
make_instr_impl_1op_cc(j, i, short_, ecxz)
// make_instr_func(je_near)
// {
// 	int len = 1;
// 	opr_src.data_size = opr_dest.data_size = 32;
// 	opr_src.type = OPR_IMM;
// 	opr_src.sreg = SREG_CS;
// 	opr_src.addr = eip + 1;
// 	len += opr_src.data_size / 8;
// 	print_asm_1("j", "e", len, &opr_src);
// 	if (condition_e)
// 		instr_execute_1op_cc_pass();
// 	else
// 		instr_execute_1op_cc_fail();
// 	return len;
// }