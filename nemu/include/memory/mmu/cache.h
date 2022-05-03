#ifndef __CACHE_H__
#define __CACHE_H__

#include "nemu.h"

#ifdef CACHE_ENABLED

#define CACHE_BLOCK 6
#define CACHE_BLOCK_SIZE (1 << CACHE_BLOCK)
#define CACHE_GROUP 3
#define CACHE_GROUP_SIZE (1 << CACHE_GROUP)
#define CACHE_SET 7
#define CACHE_SET_SIZE (1 << CACHE_SET)

// init the cache
void init_cache();

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data);

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len);

#endif

#endif
