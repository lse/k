/*
 * Copyright (c) LSE
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY LSE AS IS AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL LSE BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include <k/kfs.h>

#define align_up(v, d)	((((v) + (d) - 1) / (d)) * (d))

#define KFS_MAX_FILE_SZ ((KFS_DIRECT_BLK + KFS_INDIRECT_BLK * \
			  KFS_INDIRECT_BLK_CNT) * KFS_BLK_DATA_SZ)

static int verbose;

#define pr_info(fmt, ...) \
	do { \
		if (verbose) { \
			printf("[+] " fmt, __VA_ARGS__); \
		} \
	} while (0)

static ssize_t kfs_write(int fd, const void *buf, size_t len, off_t off)
{
	ssize_t rc = pwrite(fd, buf, len, off * KFS_BLK_SZ);

	if (rc < 0)
		err(1, "pwrite call failed");

	return rc;
}

/**
 * @brief Write superblock to rom.
 */
static void
kfs_write_superblock(int romfd, const char *fsname, u32 blk_cnt, u32 files_cnt)
{
	struct kfs_superblock sblock = {
		.magic = KFS_MAGIC,
#ifdef DEBUG
		.ctime = 0,
#else
		.ctime = time(NULL),
#endif
		.blk_cnt = blk_cnt,
		.inode_idx = 1,
		.inode_cnt = files_cnt,
		.cksum = 0
	};

	strncpy(sblock.name, fsname, KFS_NAME_SZ);
	sblock.cksum = kfs_checksum(&sblock,
				    sizeof(sblock) - sizeof(sblock.cksum));

	kfs_write(romfd, &sblock, sizeof(sblock), 0);
}

/**
 * @brief read next block from file.
 *    fill kfs_block structure.
 * @return filled kfs_block or NULL if EOF reached.
 */
static ssize_t kfs_read_block(int fd, struct kfs_block *blk)
{
	ssize_t rc = read(fd, blk->data, sizeof(blk->data));
	if (rc < 0)
		err(1, "read error");

	if (rc == 0)
		return 0;

	blk->usage = rc;

	return rc;
}

/**
 * @brief Write file inode & blocks to rom.
 * @return the next available block index;
 */
static u32
kfs_write_inode(int romfd, int fd, struct kfs_inode *inode, u32 blk_idx)
{
	for (size_t i = 0; i < KFS_DIRECT_BLK; ++blk_idx, ++i) {
		struct kfs_block blk = { 0 };

		if (!kfs_read_block(fd, &blk))
			return blk_idx;

		pr_info("write direct block to offset %u\n", blk_idx * KFS_BLK_SZ);

		blk.idx = blk_idx;
		blk.cksum = kfs_checksum(&blk, sizeof(blk));

		kfs_write(romfd, &blk, sizeof(blk), blk.idx);

		inode->d_blks[i] = blk.idx;
		inode->d_blk_cnt++;
	}

	for (size_t i = 0; i < KFS_INDIRECT_BLK; ++i) {
		struct kfs_iblock iblock_idx = { 0 };

		pr_info("write indirect data blocks to index %zu.\n", i);

		for (size_t j = 0; j < KFS_INDIRECT_BLK_CNT; ++blk_idx, ++j) {
			struct kfs_block blk = { 0 };

			if (!kfs_read_block(fd, &blk))
				break;

			pr_info("writing indirect data block to offset %u\n", blk_idx * KFS_BLK_SZ);

			blk.idx = blk_idx;
			blk.cksum = kfs_checksum(&blk, sizeof(blk));

			kfs_write(romfd, &blk, sizeof(blk), blk.idx);

			iblock_idx.blks[j] = blk.idx;
			iblock_idx.blk_cnt++;
		}
		if (!iblock_idx.blk_cnt)
			break;

		iblock_idx.idx = blk_idx++;
		iblock_idx.cksum = kfs_checksum(&iblock_idx, sizeof(iblock_idx) - sizeof(iblock_idx.cksum));

		inode->i_blks[i] = iblock_idx.idx;
		inode->i_blk_cnt++;

		kfs_write(romfd, &iblock_idx, sizeof(iblock_idx), iblock_idx.idx);
	}

	return blk_idx;
}

/**
 * @brief Write every file to rom from blkoff offset.
 */
static u32
kfs_write_files(int romfd, char **files, size_t nb_files, size_t blkoff)
{
	size_t inode_off = blkoff;
	size_t blk_idx = nb_files + blkoff;

	for (size_t i = 0; i < nb_files; ++i, inode_off++) {
		int fd = open(files[i], O_RDONLY);

		if (!fd)
			err(1, "unable to open \"%s\"", files[i]);

		struct stat st;
		fstat(fd, &st);

		if (st.st_size > KFS_MAX_FILE_SZ)
			err(1, "file \"%s\" of size %zu is too large to fit in kfs", files[i], st.st_size);

		struct kfs_inode inode = {
			.idx = inode_off,
			.next_inode = inode_off + 1,
			.inumber = i + 1, /* with this tool it will be the same as idx */
			.file_sz = st.st_size,
			.blk_cnt = align_up(st.st_size, KFS_BLK_DATA_SZ) / KFS_BLK_DATA_SZ,
		};

		strncpy(inode.filename, basename(files[i]), sizeof(inode.filename));

		/* fix last inode */
		if (i == nb_files - 1)
			inode.next_inode = 0;

		pr_info("- writing inode %u\n", inode.inumber);
		pr_info("writing data blocks to offset %zu\n", blk_idx * KFS_BLK_SZ);

		blk_idx = kfs_write_inode(romfd, fd, &inode, blk_idx);

		inode.cksum = kfs_checksum(&inode, sizeof(inode) - sizeof(inode.cksum));

		pr_info("writing inode to offset %u\n", inode.idx * KFS_BLK_SZ);

		kfs_write(romfd, &inode, sizeof(inode), inode.idx);

		close(fd);
	}

	return blk_idx;
}

static inline void usage(void)
{
	extern const char *__progname;

	fprintf(stderr, "usage: %s [-v] [-n name] -o rom_file files...\n",
		__progname);

	exit(1);
}

int main(int argc, char **argv)
{
	char *rom_file = NULL;
	char *rom_name = NULL;
	int opt;

	while ((opt = getopt(argc, argv, "n:o:v")) != -1) {
		switch (opt) {
		case 'n':
			rom_name = optarg;
			break;
		case 'o':
			rom_file = optarg;
			break;
		case 'v':
			verbose = 1;
			break;
		default:
			usage();
			break;
		}
	}

	argc -= optind;
	argv += optind;

	char **files = argv;
	size_t nb_files = argc;

	if (nb_files < 1)
		usage();

	if (!rom_file)
		usage();

	if (!rom_name)
		rom_name = rom_file;

	int romfd = open(rom_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (romfd < 0)
		err(1, "unable to open %s", rom_file);

	pr_info("block size: %u\n", KFS_BLK_SZ);
	pr_info("%zu inodes will be written.\n", nb_files);

	u32 blk_cnt = kfs_write_files(romfd, files, nb_files, 1);

	kfs_write_superblock(romfd, rom_name, blk_cnt, nb_files);

	return 0;
}
