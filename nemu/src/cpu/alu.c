#include "cpu/cpu.h"

inline void set_PF(uint32_t res){
	uint32_t tmp = 0x80;
	cpu.eflags.PF = 0;
	while(tmp){
		cpu.eflags.PF += ((tmp & res) != 0);
		tmp >>= 1;
	}
	cpu.eflags.PF = (cpu.eflags.PF + 1) & 1;
}
inline void set_ZF(uint32_t res){
	cpu.eflags.ZF = (res == 0);
}
inline void set_SF(uint32_t res, size_t data_size){
	cpu.eflags.SF = sign(sign_ext(res, data_size));
}
void set_PF(uint32_t res);
void set_ZF(uint32_t res);
void set_SF(uint32_t res, size_t data_size);


uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t res = (src + dest) & (0xFFFFFFFF >> (32 - data_size));
	src &= (0xFFFFFFFF >> (32 - data_size));
	dest &= (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.CF = (sign_ext(res, data_size) < sign_ext(src, data_size))
		|| (sign_ext(res, data_size) < sign_ext(dest, data_size));
	set_PF(res);
	set_ZF(res);
	set_SF(res, data_size);
	/* set OF */
	// if(sign(sign_ext(src, data_size)) == sign(sign_ext(dest, data_size))){
	// 	if(sign(sign_ext(src, data_size)) == sign(sign_ext(res, data_size))){
	// 		cpu.eflags.OF = 0;
	// 	}else{
	// 		cpu.eflags.OF = 1;
	// 	}
	// }else{
	// 	cpu.eflags.OF = 0;
	// }
	cpu.eflags.OF = (sign(sign_ext(src, data_size)) == sign(sign_ext(dest, data_size)))
		&& (sign(sign_ext(src, data_size)) != sign(sign_ext(res, data_size)));
	return res;
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	if(cpu.eflags.CF == 0) return alu_add(src, dest, data_size);
	uint32_t res = (src + dest + cpu.eflags.CF) & (0xFFFFFFFF >> (32 - data_size));
	src &= (0xFFFFFFFF >> (32 - data_size));
	dest &= (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.CF = (sign_ext(res, data_size) <= sign_ext(src, data_size))
		|| (sign_ext(res, data_size) <= sign_ext(dest, data_size));
	set_PF(res);
	set_ZF(res);
	set_SF(res, data_size);
	cpu.eflags.OF = (sign(sign_ext(src, data_size)) == sign(sign_ext(dest, data_size)))
		&& (sign(sign_ext(src, data_size)) != sign(sign_ext(res, data_size)));
	return res;
#endif
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t res = (dest - src) & (0xFFFFFFFF >> (32 - data_size));
	src &= (0xFFFFFFFF >> (32 - data_size));
	dest &= (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.CF = src > dest;
	set_PF(res);
	set_ZF(res);
	set_SF(res, data_size);
	cpu.eflags.OF = (sign(sign_ext(src, data_size)) != sign(sign_ext(dest, data_size)))
		&& (sign(sign_ext(dest, data_size)) != sign(sign_ext(res, data_size)));
	return res;
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	if(cpu.eflags.CF == 0) return alu_sub(src, dest, data_size);
	uint32_t res = (dest - src - cpu.eflags.CF) & (0xFFFFFFFF >> (32 - data_size));
	src &= (0xFFFFFFFF >> (32 - data_size));
	dest &= (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.CF = src >= dest;
	set_PF(res);
	set_ZF(res);
	set_SF(res, data_size);
	cpu.eflags.OF = (sign(sign_ext(src, data_size)) != sign(sign_ext(dest, data_size)))
		&& (sign(sign_ext(dest, data_size)) != sign(sign_ext(res, data_size)));
	return res;
#endif
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint64_t res = (uint64_t)src * dest;
	cpu.eflags.CF = (res >> data_size != 0);
	cpu.eflags.OF = (res >> data_size != 0);
	return res;
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	int64_t res = (int64_t)src * dest;
	cpu.eflags.CF = (res >> data_size != 0);
	cpu.eflags.OF = (res >> data_size != 0);
	return res;
#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	assert(src != 0);
	return dest / src;
#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	assert(src != 0);
	return dest / src;
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	assert(src != 0);
	return dest % src;
	return 0;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	assert(src != 0);
	return dest % src;
	return 0;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t res = (dest & src) & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.CF = 0;
	set_PF(res);
	set_ZF(res);
	set_SF(res, data_size);
	cpu.eflags.OF = 0;
	return res;
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t res = (dest ^ src) & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.CF = 0;
	set_PF(res);
	set_ZF(res);
	set_SF(res, data_size);
	cpu.eflags.OF = 0;
	return res;
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t res = (dest | src) & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.CF = 0;
	set_PF(res);
	set_ZF(res);
	set_SF(res, data_size);
	cpu.eflags.OF = 0;
	return res;
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t res;
	dest &= (0xFFFFFFFF >> (32 - data_size));
	res = (dest << src) & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.CF = (dest >> (data_size - src)) & 1;
	set_PF(res);
	set_ZF(res);
	set_SF(res, data_size);
	cpu.eflags.OF = (src == 1) && (((res >> (data_size - 1)) & 1) != cpu.eflags.CF);
	return res;
#endif
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t res;
	dest &= (0xFFFFFFFF >> (32 - data_size));
	res = (dest >> src) & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.CF = (dest >> (src - 1)) & 1;
	set_PF(res);
	set_ZF(res);
	set_SF(res, data_size);
	cpu.eflags.OF = (src == 1) && ((dest >> (data_size - 1)) & 1);
	return res;
#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t res;
	dest &= (0xFFFFFFFF >> (32 - data_size));
	res = (dest >> src) & (0xFFFFFFFF >> (32 - data_size));
	if(((dest >> (data_size - 1)) & 1) != 0){
		res |= (0xFFFFFFFF << (data_size - src));
		res &= (0xFFFFFFFF >> (32 - data_size));
	}
	cpu.eflags.CF = (dest >> (src - 1)) & 1;
	set_PF(res);
	set_ZF(res);
	set_SF(res, data_size);
	cpu.eflags.OF = 0;
	return res;
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	return alu_shl(src, dest, data_size);
#endif
}

inline uint32_t sign_ext(uint32_t x, size_t data_size)
{
	assert(data_size == 16 || data_size == 8 || data_size == 32);
	switch (data_size)
	{
	case 8:
		return (int32_t)((int8_t)(x & 0xff));
	case 16:
		return (int32_t)((int16_t)(x & 0xffff));
	default:
		return (int32_t)x;
	}
}

inline uint64_t sign_ext_64(uint32_t x, size_t data_size)
{
	assert(data_size == 16 || data_size == 8 || data_size == 32);
	switch (data_size)
	{
	case 8:
		return (int64_t)((int8_t)(x & 0xff));
	case 16:
		return (int64_t)((int16_t)(x & 0xffff));
	default:
		return (int64_t)((int32_t)x);
	}
}