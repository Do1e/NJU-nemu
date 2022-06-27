#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

static void push(uint32_t val){
	OPERAND dest;
	data_size = 32;
	cpu.esp -= data_size / 8;
	dest.data_size = data_size;
	dest.type = OPR_MEM;
	dest.addr = cpu.esp;
	dest.val = val;
	dest.sreg = SREG_SS;
	operand_write(&dest);
}

void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
	// printf("Please implement raise_intr()");
	// fflush(stdout);
	// assert(0);
	push(cpu.eflags.val);
	push(cpu.cs.val);
	push(cpu.eip);

	GateDesc *gd = (GateDesc *)(hw_mem + page_translate(segment_translate(cpu.idtr.base + intr_no * 8, SREG_CS)));
	if(gd->type == 0xE){
		cpu.eflags.IF = 0;
	}
	cpu.cs.val = gd->selector;
	load_sreg(SREG_CS);
	cpu.eip = (gd->offset_31_16 << 16) + gd->offset_15_0;
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
