#ifndef MEMORY_H
#define MEMORY_H

#include "list.h"
#include "multiboot.h"

struct memory_map {
	struct list list;
	unsigned int base_addr;
	unsigned int size;
	int type;
};

void memory_dump();
void memory_init(multiboot_info_t *info);
void *memory_reserve(size_t size);
void *memory_reserve_ex(unsigned int base_addr, size_t size);
void memory_release(void *ptr);

struct cache {
	struct list freelist;
	struct list caches;
	void *base;
	size_t size;
	size_t bsize;
};

struct cache *cache_new(void *base, size_t nmemb, size_t bsize);
void *cache_alloc(struct cache *cache);
void cache_free(struct cache *cache, void *ptr);

#endif /* MEMORY_H */
