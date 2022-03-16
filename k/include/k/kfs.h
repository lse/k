#ifndef K_KFS_H
#define K_KFS_H

#include <k/types.h>

#define KFS_MAGIC 0xd35f9caa

#define KFS_MIN_BLK_SZ 512

#define KFS_BLK_SZ 4096

#define KFS_BLK_DATA_SZ (KFS_BLK_SZ - 3 * 4)

#define KFS_DIRECT_BLK 10

#define KFS_INDIRECT_BLK 16

#define KFS_INDIRECT_BLK_CNT 16

#define KFS_FNAME_SZ 32

#define KFS_NAME_SZ 32

#define __packed __attribute__((__packed__))

struct kfs_block {
	u32 idx;
	u32 usage;
	u32 cksum;
	u8 data[KFS_BLK_DATA_SZ];
} __packed;

struct kfs_iblock {
	u32 idx;
	u32 blk_cnt;
	u32 blks[KFS_INDIRECT_BLK_CNT];
	u32 cksum;
} __packed;

struct kfs_inode {
	u32 inumber;
	char filename[KFS_FNAME_SZ];
	u32 file_sz;
	u32 idx;
	u32 blk_cnt;
	u32 next_inode;
	u32 d_blk_cnt;
	u32 i_blk_cnt;
	u32 d_blks[KFS_DIRECT_BLK];
	u32 i_blks[KFS_INDIRECT_BLK];
	u32 cksum;
} __packed;

struct kfs_blk {
	union {
		struct kfs_block blk;
		struct kfs_iblock iblk;
		struct kfs_inode ino;
		u8 whole_blk[KFS_BLK_SZ];
	};
};

struct kfs_superblock {
	u32 magic;
	char name[KFS_NAME_SZ];
	s32 ctime;
	u32 blk_cnt;
	u32 inode_cnt;
	u32 inode_idx;
	u32 cksum;
} __packed;

#define ADLER32_MOD 65521

/**
 * @brief Adler32 checksum.
 */
static inline unsigned int kfs_checksum(const void *data, size_t size)
{
	unsigned int a = 1;
	unsigned int b = 0;
	size_t i = 0;
	const u8 *buf = data;

	while (i < size) {
		a += buf[i++];
		b += a;
	}
	return ((b % ADLER32_MOD) << 16) | (a % ADLER32_MOD);
}

#endif
