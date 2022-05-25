#include "cpu/instr.h"

make_instr_func(jmp_near)
{
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;

    operand_read(&rel);

    int offset = sign_ext(rel.val, data_size);
    // thank Ting Xu from CS'17 for finding this bug
    print_asm_1("jmp", "", 1 + data_size / 8, &rel);

    cpu.eip += offset;

    return 1 + data_size / 8;
}

make_instr_func(jmp_far_imm){
    OPERAND rm1, rm2;
    rm1.type = OPR_IMM;
    rm1.sreg = SREG_CS;
    rm1.data_size = data_size;
    rm1.addr = eip + 1;
    operand_read(&rm1);

    
    rm2.data_size = 16;
    rm2.addr = rm1.addr + data_size / 8;
    rm2.type = OPR_IMM;
    rm2.sreg = SREG_CS;
    operand_read(&rm2);
    cpu.cs.val = rm2.val;
    print_asm_2("ljmp", "", 7, &rm2, &rm1);
    cpu.eip = rm1.val;
    load_sreg(1);
    return 0;
}

make_instr_func(jmp_short){
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.data_size = 8;
    rel.addr = eip + 1;

    operand_read(&rel);
    int offset = sign_ext(rel.val, 8);
    print_asm_1("jmp", "", 1 + data_size / 8, &rel);
    cpu.eip += offset;
    return 2;
}

make_instr_func(jmp_near_indirect){
    OPERAND rel;
    rel.data_size = data_size;
    modrm_rm(eip + 1, &rel);

    operand_read(&rel);
    print_asm_1("jmp", "", 1 + data_size / 8, &rel);
    cpu.eip = rel.val;
    return 0;
}