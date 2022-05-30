#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	// printf("\nPlease implement page_translate()\n");
	// fflush(stdout);
	// assert(0);
	uint32_t dir = (laddr >> (PAGE_OFFSET_BIT + PAGE_PAGE_BIT)) & PAGE_DIR_MASK;
	uint32_t page = (laddr >> PAGE_OFFSET_BIT) & PAGE_PAGE_MASK;
	uint32_t offset = laddr & PAGE_OFFSET_MASK;
	PDE pde;
	pde.val = paddr_read((cpu.cr3.pdbr << PAGE_OFFSET_BIT) + dir * sizeof(PDE), sizeof(PDE));
	assert(pde.present == 1);
	PTE pte;
	pte.val = paddr_read((pde.page_frame << PAGE_OFFSET_BIT) + page * sizeof(PTE), sizeof(PTE));
	assert(pte.present == 1);
	return (pte.page_frame << PAGE_OFFSET_BIT) | offset;
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
