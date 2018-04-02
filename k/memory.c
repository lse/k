#include "memory.h"

#include <k/types.h>
#include <stdio.h>

static struct list memory_map = {&memory_map, &memory_map};
static struct cache *memory_map_cache;
static struct cache *metadata_cache;

static void *__memory_reserve_ex(unsigned int base_addr, size_t size, int in_extend);
static void *__cache_alloc(struct cache *cache, int in_extend);

static void cache_initialize(struct cache *c, void *p, size_t sz, size_t bsize)
{
	list_init(&c->freelist);
	list_init(&c->caches);
	c->base = p;
	c->size = sz;
	c->bsize = bsize;

	for (size_t i = 0; i < sz / bsize; ++i) {
		struct list *l = (struct list *)((char *) p + i * bsize);
		list_init(l);
		list_insert(c->freelist.prev, l);
	}
}

static void cache_extend(struct cache *old, void *p, size_t sz, size_t bsize, int in_extend)
{
	struct cache *c = __cache_alloc(metadata_cache, in_extend);
	cache_initialize(c, p, sz, bsize);
	list_insert(old->caches.prev, &c->caches);
}

struct cache *cache_new(void *base, size_t nmemb, size_t bsize)
{
	struct cache *c = cache_alloc(metadata_cache);

	cache_initialize(c, base, nmemb * bsize, bsize);

	return c;
}

static size_t freelist_size(struct cache *cache)
{
	if (list_empty(&cache->freelist))
		return 0;

	size_t len = 1;

	struct list *l = cache->freelist.next;

	for (; l != &cache->freelist; l = l->next) {
		len++;
	}

	return len;
}

static size_t cache_remaining(struct cache *cache)
{
	size_t len = freelist_size(cache);

	struct cache *c;
	list_for_each(c, &cache->caches, caches) {
		len += freelist_size(c);
	}

	return len;
}

static void *__cache_alloc(struct cache *cache, int in_extend)
{
try_alloc:
	if (!list_empty(&cache->freelist)) {
		struct list *l = cache->freelist.next;
		list_remove(l);
		return (void *)l;
	}

	struct cache *c;
	list_for_each(c, &cache->caches, caches) {
		if (!list_empty(&c->freelist)) {
			return cache_alloc(c);
		}
	}

	if (cache_remaining(metadata_cache) <= 2 && !in_extend) {
		void *p = __memory_reserve_ex(0, metadata_cache->size, 1);
		cache_extend(metadata_cache, p,
			     metadata_cache->size,
			     metadata_cache->bsize, 1);
	}

	void *p = memory_reserve(c->size);
	cache_extend(c, p, c->size, c->bsize, 0);

	goto try_alloc;
}

void *cache_alloc(struct cache *cache)
{
	return __cache_alloc(cache, 0);
}

static int cache_try_free(struct cache *cache, void *ptr)
{
	if ((size_t)cache->base <= (size_t)ptr
	 && (size_t)cache->base + cache->size > (size_t)ptr) {
		list_init(ptr);
		list_insert(&cache->freelist, ptr);
		return 1;
	}
	return 0;
}

void cache_free(struct cache *cache, void *ptr)
{
	if (cache_try_free(cache, ptr)) {
		return;
	}
	struct cache *c;
	list_for_each(c, &cache->caches, caches) {
		if (cache_try_free(c, ptr)) {
			return;
		}
	}
	/* FIXME: handle the case when the pointer does not belong to the cache */
}

static void memory_initialize(struct memory_map *m,
			      unsigned addr, unsigned size, int type)
{
	list_init(&m->list);
	m->base_addr = addr;
	m->size = size;
	m->type = type;
}

void memory_dump()
{
	struct memory_map *m;
	list_for_each(m, &memory_map, list) {
		printf("{.base_addr=%p, .length=0x%x, .type=%u}\n",
				m->base_addr, m->size, m->type);
	}
}

extern void *_end[]; /* kernel data end address */

#define BASE_METADATA_CACHE_NMEMB	10

void memory_init(multiboot_info_t *info)
{
	unsigned int last_loaded_addr = info->mods_count
		? ((multiboot_module_t *)info->mods_addr)[info->mods_count - 1].mod_end
		: 0;

	if (last_loaded_addr < (u32)_end) {
		last_loaded_addr = (u32)_end; /* XXX: needs to align up */
	}

	/* take the first good memory region */
	unsigned int reservation_addr = last_loaded_addr;
	unsigned int reservation_len = (BASE_METADATA_CACHE_NMEMB + 1) * sizeof(struct cache);

	unsigned int num_mem_zone = info->mmap_length / sizeof(multiboot_memory_map_t);

	metadata_cache = (void *)reservation_addr;
	cache_initialize(metadata_cache, metadata_cache + 1, BASE_METADATA_CACHE_NMEMB * sizeof(struct cache),
			 sizeof(struct cache));

	memory_map_cache = cache_new((void *)(reservation_addr + reservation_len),
				      num_mem_zone * 2, sizeof(struct memory_map));

	reservation_len += num_mem_zone * 2 * sizeof(struct memory_map);

	multiboot_memory_map_t *map = (void *)info->mmap_addr;

	for (size_t i = 0; i < num_mem_zone; ++i) {
		struct memory_map *m = cache_alloc(memory_map_cache);

		memory_initialize(m, (u32)map[i].addr, (u32)map[i].len,
				  map[i].type - 1);

		list_insert(memory_map.prev, &m->list);
	}

	/* reserve all low memory to avoid overwriting grub data */
	memory_reserve_ex(0, info->mem_lower * 1024);
	/* reserve kernel and fs data */
	memory_reserve_ex(0x100000, last_loaded_addr - 0x100000);
	/* reserve initial cache datas */
	memory_reserve_ex(reservation_addr, reservation_len);
}

static void *memory_split(struct memory_map *m, size_t size)
{
	struct memory_map *t = cache_alloc(memory_map_cache);
	memory_initialize(t, m->base_addr, size, 2);

	list_insert(m->list.prev, &t->list);

	m->size -= size;
	m->base_addr += size;

	return (void *)t->base_addr;
}

static void *__memory_reserve_ex(unsigned int base_addr, size_t size, int in_extend)
{
	if (cache_remaining(memory_map_cache) <= 2 && !in_extend) {
		void *p = __memory_reserve_ex(0, 10 * sizeof(struct cache), 1);
		cache_extend(memory_map_cache, p, 10 * sizeof(struct cache),
				sizeof(struct cache), 0);
	}

	struct memory_map *m;
	list_for_each(m, &memory_map, list) {
		if (m->type)
			continue;

		if (!base_addr && m->size >= size) {
			if (m->size == size) {
				m->type = 2;
				return (void *)m->base_addr;
			}
			return memory_split(m, size);
		} else if (base_addr >= m->base_addr && base_addr < m->base_addr + m->size) {
			if (base_addr + size >= m->base_addr + m->size) {
				return NULL;
			}
			unsigned first_size = base_addr - m->base_addr;
			if (first_size) {
				memory_split(m, first_size);
				return memory_reserve_ex(base_addr, size);
			}

			return memory_split(m, size);
		}
	}

	return NULL;
}

void *memory_reserve_ex(unsigned int base_addr, size_t size)
{
	return __memory_reserve_ex(base_addr, size, 0);
}

void *memory_reserve(size_t size)
{
	return memory_reserve_ex(0, size);
}

void memory_release(void *ptr)
{
	struct memory_map *m;
	list_for_each(m, &memory_map, list) {
		if (m->base_addr == (unsigned int)ptr) {
			m->type = 0;
			/* FIXME: need to merge free blocks */
			return;
		}
	}
}

