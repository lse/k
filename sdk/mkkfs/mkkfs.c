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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include <k/kfs.h>

#define kfswrite(Buf, Sz, S) \
	do { \
		if (!fwrite (Buf, Sz, 1, S)) \
			err(1, "error writing to file\n"); \
	} while (0)

static int verbose;

#define pr_info(fmt, ...) \
	do { \
		if (verbose) { \
			printf("[+] " fmt, __VA_ARGS__); \
		} \
	} while (0)

/**
 * @brief Write superblock to rom.
 */
static inline void
kfs_write_superblock(FILE * out, const char *fsname, uint32_t blk_cnt,
		     uint32_t files_cnt)
{
	struct kfs_superblock sblock;

	memset(&sblock, 0, sizeof(struct kfs_superblock));
	sblock.magic = KFS_MAGIC;
	strncpy(sblock.name, fsname, KFS_NAME_SZ);
	sblock.ctime = time(NULL);
	sblock.blk_cnt = blk_cnt;
	sblock.inode_idx = 1;
	sblock.inode_cnt = files_cnt;
	sblock.cksum = kfs_checksum(&sblock, sizeof(struct kfs_superblock) - sizeof(sblock.cksum));
	fseek(out, 0, SEEK_SET);
	kfswrite(&sblock, sizeof(struct kfs_superblock), out);
}

/**
 * @brief read next block from file.
 *    fill kfs_block structure.
 * @return filled kfs_block or NULL if EOF reached.
 */
static struct kfs_block *kfs_read_block(FILE * fp, struct kfs_block *blk)
{
	memset(blk, 0, sizeof(struct kfs_block));
	if (!(blk->usage = fread(blk->data, 1, sizeof(blk->data), fp)))
		return NULL;
	return blk;
}

/**
 * @brief Allocate all needed inodes.
 */
static struct kfs_inode **kfs_alloc_inodes(char **argv, uint32_t off, uint32_t * inode_cnt)
{
	struct kfs_inode **inodes;
	uint32_t i;
	char **ptr;

	for (ptr = argv; *ptr; ++ptr)
		continue;

	*inode_cnt = ptr - argv;
	inodes = calloc((*inode_cnt + 1), sizeof(struct kfs_inode *));

	for (i = 0; *argv; ++i, ++argv) {
		inodes[i] = malloc(sizeof(struct kfs_inode));
		memset(inodes[i], 0, sizeof(struct kfs_inode));
		strncpy(inodes[i]->filename, basename(*argv), sizeof(inodes[i]->filename));

		struct stat st;
		if (stat(*argv, &st) < 0)
			err(1, "error stating file %s\n", *argv);

		inodes[i]->file_sz = st.st_size;
		inodes[i]->idx = off++;
		inodes[i]->next_inode = off;
		inodes[i]->blk_cnt = 0;
		inodes[i]->inumber = i + 1;
	}
	inodes[i - 1]->next_inode = 0;

	return inodes;
}

/**
 * @brief Write file inode & blocks to rom.
 * @return the next available block index;
 */
static uint32_t
kfs_write_inode(FILE * out, FILE * fp, struct kfs_inode *inode, uint32_t blk_idx)
{
	struct kfs_block blk;
	uint32_t i, j;

	pr_info("- writing inode %u\n", inode->inumber);
	pr_info("writing data blocks to offset %u\n", blk_idx * KFS_BLK_SZ);

	/* set file position to correct block */
	fseek(out, blk_idx * KFS_BLK_SZ, SEEK_SET);

	/* write direct blocks */
	for (i = 0; i < KFS_DIRECT_BLK && kfs_read_block(fp, &blk); ++blk_idx, ++i) {
		blk.idx = blk_idx;

		pr_info("writing direct data block to offset %u\n", blk_idx * KFS_BLK_SZ);

		blk.cksum = kfs_checksum(&blk, sizeof(struct kfs_block));
		kfswrite(&blk, sizeof(struct kfs_block), out);
		inode->d_blks[i] = blk_idx;
		inode->d_blk_cnt++;
	}
	inode->blk_cnt += i;

	/* write indirect blocks */
	if (!feof(fp)) {
		struct kfs_iblock iblock_idx;

		for (i = 0; i < KFS_INDIRECT_BLK && !feof(fp); ++i) {
			memset(&iblock_idx, 0, sizeof(iblock_idx));

			pr_info("writing indirect data blocks to index %u.\n", i);

			/* fill indirect blocks */
			for (j = 0; j < KFS_INDIRECT_BLK_CNT; ++j) {
				if (!kfs_read_block(fp, &blk)) {
					break;
				}
				blk.idx = blk_idx++;
				blk.cksum = kfs_checksum(&blk, sizeof(struct kfs_block));
				iblock_idx.blks[iblock_idx.blk_cnt++] = blk.idx;

				pr_info("writing indirect data block to offset %u\n", blk.idx * KFS_BLK_SZ);

				kfswrite(&blk, sizeof(struct kfs_block), out);
			}
			inode->blk_cnt += j;
			iblock_idx.idx = blk_idx++;
			inode->i_blks[i] = iblock_idx.idx;
			iblock_idx.cksum = kfs_checksum(&iblock_idx, sizeof(struct kfs_iblock) - sizeof(iblock_idx.cksum));
			kfswrite(&iblock_idx, sizeof(struct kfs_iblock), out);
			fseek(out, blk_idx * KFS_BLK_SZ, SEEK_SET);
		}
		inode->i_blk_cnt = i;
		if (!feof(fp)) {
			errx(1, "file is too large to be written to kfs");
			return 0;
		}
	}
	inode->cksum = kfs_checksum(inode, sizeof(struct kfs_inode) - sizeof(inode->cksum));
	/* seek to correct inode position in order to write it */
	pr_info("writing inode to offset %u\n", inode->idx * KFS_BLK_SZ);

	fseek(out, inode->idx * KFS_BLK_SZ, SEEK_SET);
	kfswrite(inode, sizeof(struct kfs_inode), out);

	return blk_idx;
}

/**
 * @brief Write every file to rom from blkoff offset.
 */
static uint32_t
kfs_write_files(FILE * out, char **files, size_t blkoff, uint32_t * inode_cnt)
{
	uint32_t i;
	struct kfs_inode **inodes = kfs_alloc_inodes(files, blkoff, inode_cnt);
	if (!inodes) {
		return 0;
	}
	size_t blk_idx = *inode_cnt + 1;

	pr_info("%u inodes will be written.\n", *inode_cnt);

	for (i = 0; *files; ++i, ++files) {
		FILE *fp = fopen(*files, "r");
		if (!fp) {
			err(1, "error opening file %s in read mode", *files);
			return 0;
		}
		blk_idx = kfs_write_inode(out, fp, inodes[i], blk_idx);
		fclose(fp);
	}
	return blk_idx;
}

static inline void usage(void)
{
	extern const char *__progname;

	fprintf(stderr, "usage: %s [-v] [-n name] -o rom_file files...\n", __progname);

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

	FILE *rom = fopen(rom_file, "w");
	if (!rom)
		err(1, "error opening file %s for writing", rom_file);

	pr_info("block size: %u\n", KFS_BLK_SZ);

	uint32_t blk_cnt;
	uint32_t inode_cnt;
	if (!(blk_cnt = kfs_write_files(rom, files, 1, &inode_cnt))) {
		fclose(rom);
		return 1;
	}

	kfs_write_superblock(rom, rom_name, blk_cnt, inode_cnt);

	return 0;
}
