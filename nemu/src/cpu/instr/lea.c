#include "cpu/instr.h"
/*
Put the implementations of `lea' instructions here.
*/

make_instr_func(lea)
{
	int len = 1;
	OPERAND src, dest;
	src.data_size = data_size;
	dest.data_size = data_size;
	len += modrm_r_rm(eip + 1, &dest, &src);
	dest.val = src.addr;
	operand_write(&dest);
	return len;
}
