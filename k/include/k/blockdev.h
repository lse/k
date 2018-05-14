#ifndef BLOCKDEV_H_
#define BLOCKDEV_H_

# include <assert.h>
# include <k/types.h>

struct blockdev;

struct blk_ops {
	void *(*read)(struct blockdev *, size_t);
	void (*free_blk)(struct blockdev *, void *);
};

struct blockdev {
	size_t blk_size;

	struct blk_ops *ops;
	void *blocks;
};

static inline void *block_read(struct blockdev *bd, size_t lba)
{
	assert(bd);
	assert(bd->ops);
	assert(bd->ops->read);

	return bd->ops->read(bd, lba);
}

static inline void block_free(struct blockdev *bd, void *ptr)
{
	assert(bd);
	assert(bd->ops);
	assert(bd->ops->free_blk);

	bd->ops->free_blk(bd, ptr);
}

#endif	/* BLOCKDEV_H_ */
