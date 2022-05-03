#include "memory/mmu/cache.h"
#include "memory/memory.h"

#include<time.h>
#include<stdlib.h>

#ifdef CACHE_ENABLED

int cache_hit = 0;
int cache_miss = 0;
extern uint8_t hw_mem[MEM_SIZE_B];
struct cache_group{
	struct cache_line{
		bool valid_bit;
		uint32_t tag;
		uint8_t data[CACHE_BLOCK_SIZE];
	}cache_group[CACHE_GROUP_SIZE];
}cache[CACHE_SET_SIZE];

uint32_t hw_mem_read(paddr_t paddr, size_t len);
void hw_mem_write(paddr_t paddr, size_t len, uint32_t data);

// init the cache
void init_cache()
{
	// implement me in PA 3-1
	memset(cache, 0, sizeof(cache));
	srand(time(NULL));
}

void replace_cache(uint32_t grpID, uint32_t grpIndex, uint32_t cachePos, uint32_t tag, paddr_t paddr)
{
	cache[grpID].cache_group[grpIndex].valid_bit = 1;
	cache[grpID].cache_group[grpIndex].tag = tag;
	memcpy(cache[grpID].cache_group[grpIndex].data, hw_mem + paddr - cachePos, CACHE_BLOCK_SIZE);
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data)
{
	// implement me in PA 3-1
	uint32_t grpID = (paddr >> CACHE_BLOCK) & ((1 << CACHE_SET) - 1);
	uint32_t cachePos = paddr & ((1 << CACHE_BLOCK) - 1);
	uint32_t tag = paddr >> (CACHE_BLOCK + CACHE_SET);
	bool hit = false;
	for(int i = 0; i < CACHE_GROUP_SIZE; ++i){
		if(cache[grpID].cache_group[i].valid_bit != 0 && cache[grpID].cache_group[i].tag == tag){
			++cache_hit;
			hit = true;
			if(cachePos + len <= CACHE_BLOCK_SIZE){  // 不跨行
				memcpy(cache[grpID].cache_group[i].data + cachePos, &data, len);
				hw_mem_write(paddr, len, data);
				return;
			}
			else{  // 跨行
				uint32_t left = cachePos + len - CACHE_BLOCK_SIZE;
				uint32_t already = len - left;
				uint32_t alreadynum = data & ((1 << (already * 8)) - 1);
				uint32_t leftnum = data >> (already * 8);
				memcpy(cache[grpID].cache_group[i].data + cachePos, &alreadynum, already);
				hw_mem_write(paddr, already, alreadynum);
				cache_write(paddr + already, left, leftnum);
				return;
			}
		}
	}
	if(!hit){
		++cache_miss;
		hw_mem_write(paddr, len, data);
	}
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len)
{
	// implement me in PA 3-1
	uint32_t grpID = (paddr >> CACHE_BLOCK) & ((1 << CACHE_SET) - 1);
	uint32_t cachePos = paddr & ((1 << CACHE_BLOCK) - 1);
	uint32_t tag = paddr >> (CACHE_BLOCK + CACHE_SET);
	uint32_t ret = 0;
	bool hit = false;
	for(int i = 0; i < CACHE_GROUP_SIZE; ++i){
		if(cache[grpID].cache_group[i].valid_bit != 0 && cache[grpID].cache_group[i].tag == tag){
			++cache_hit;
			hit = true;
			if(cachePos + len <= CACHE_BLOCK_SIZE){  // 不跨行
				// 小端序
				for(int j = cachePos + len - 1; j >= (int)cachePos; --j){
					ret = (ret << 8) | cache[grpID].cache_group[i].data[j];
				}
				return ret;
			}
			else{  // 跨行
				uint32_t left = cachePos + len - CACHE_BLOCK_SIZE;
				uint32_t already = len - left;
				for(int j = cachePos + already - 1; j >= cachePos; --j){
					ret = (ret << 8) | cache[grpID].cache_group[i].data[j];
				}
				uint32_t temp = (cache_read(paddr + already, left) << (already * 8));
				return ret | temp;
			}
		}
	}
	if(!hit){
		++cache_miss;
		for(int i = 0; i < CACHE_GROUP_SIZE; ++i){
			if(cache[grpID].cache_group[i].valid_bit == 0){
				replace_cache(grpID, i, cachePos, tag, paddr);
				return hw_mem_read(paddr, len);
			}
			// 替换算法采用随机方式
			if(i == CACHE_GROUP_SIZE - 1){
				uint32_t randID = (uint32_t)rand() % CACHE_GROUP_SIZE;
				replace_cache(grpID, randID, cachePos, tag, paddr);
				return hw_mem_read(paddr, len);
			}
		}
	}
	return 0;
}
#endif
