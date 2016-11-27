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
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>

#include <k/kfs.h>

#define kfswrite(Buf, Sz, Nm, S) \
  do { \
    if (!fwrite (Buf, Sz, Nm, S)) { \
      fprintf (stderr, "error writing to file\n"); \
      fclose (S); \
      exit (EXIT_FAILURE); \
    } \
  } \
  while (0)

#define OPT_VERBOSE (1 << 0)

static int _opt = 0;

static inline int is_opt(const char *str)
{
	return !strcmp(str, "-v")
		|| !strcmp(str, "-r")
		|| !strcmp(str, "-n")
		|| !strcmp(str, "-f")
		|| !strcmp(str, "-b");
}

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
	kfswrite(&sblock, sizeof(struct kfs_superblock), 1, out);
}

/**
 * @brief read next block from file.
 *    fill kfs_block structure.
 * @return filled kfs_block or NULL if EOF reached.
 */
static struct kfs_block *kfs_read_block(FILE * fp, struct kfs_block *blk)
{
	size_t sz;

	memset(blk, 0, sizeof(struct kfs_block));
	if (!(sz = fread(blk->data, 1, sizeof(blk->data), fp)))
		return NULL;
	blk->usage = sz;
	return blk;
}

/**
 * @brief Allocate all needed inodes.
 */
static struct kfs_inode **kfs_alloc_inodes(char **argv, uint32_t off, uint32_t * inode_cnt)
{
	struct stat st;
	struct kfs_inode **inodes;
	uint32_t i;
	char **ptr;

	for (ptr = argv; *ptr && !is_opt(*ptr); ++ptr)
		continue;
	*inode_cnt = ptr - argv;
	inodes = calloc((*inode_cnt + 1), sizeof(struct kfs_inode *));

	for (i = 0; *argv; ++i, ++argv) {
		inodes[i] = malloc(sizeof(struct kfs_inode));
		memset(inodes[i], 0, sizeof(struct kfs_inode));
		strncpy(inodes[i]->filename, *argv, sizeof(inodes[i]->filename));

		if (stat(*argv, &st) < 0) {
			fprintf(stderr, "error stating file %s: %s\n", *argv, strerror(errno));
			return NULL;
		}
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

	if (_opt & OPT_VERBOSE) {
		printf("- writing inode %u\n", inode->inumber);
		printf("writing data blocks to offset %u\n", blk_idx * KFS_BLK_SZ);
	}
	/* set file position to correct block */
	fseek(out, blk_idx * KFS_BLK_SZ, SEEK_SET);

	/* write direct blocks */
	for (i = 0; i < KFS_DIRECT_BLK && kfs_read_block(fp, &blk); ++blk_idx, ++i) {
		blk.idx = blk_idx;
		if (_opt & OPT_VERBOSE)
			printf("writing direct data block to offset %u\n", blk_idx * KFS_BLK_SZ);

		blk.cksum = kfs_checksum(&blk, sizeof(struct kfs_block));
		kfswrite(&blk, sizeof(struct kfs_block), 1, out);
		inode->d_blks[i] = blk_idx;
		inode->d_blk_cnt++;
	}
	inode->blk_cnt += i;

	/* write indirect blocks */
	if (!feof(fp)) {
		struct kfs_iblock iblock_idx;

		for (i = 0; i < KFS_INDIRECT_BLK && !feof(fp); ++i) {
			memset(&iblock_idx, 0, sizeof(iblock_idx));
			if (_opt & OPT_VERBOSE)
				printf("writing indirect data blocks to index %u.\n", i);

			/* fill indirect blocks */
			for (j = 0; j < KFS_INDIRECT_BLK_CNT; ++j) {
				if (!kfs_read_block(fp, &blk)) {
					break;
				}
				blk.idx = blk_idx++;
				blk.cksum = kfs_checksum(&blk, sizeof(struct kfs_block));
				iblock_idx.blks[iblock_idx.blk_cnt++] = blk.idx;
				if (_opt & OPT_VERBOSE) {
					printf("writing indirect data block to offset %u\n", blk.idx * KFS_BLK_SZ);
				}
				kfswrite(&blk, sizeof(struct kfs_block), 1, out);
			}
			inode->blk_cnt += j;
			iblock_idx.idx = blk_idx++;
			inode->i_blks[i] = iblock_idx.idx;
			iblock_idx.cksum = kfs_checksum(&iblock_idx, sizeof(struct kfs_iblock) - sizeof(iblock_idx.cksum));
			kfswrite(&iblock_idx, sizeof(struct kfs_iblock), 1, out);
			fseek(out, blk_idx * KFS_BLK_SZ, SEEK_SET);
		}
		inode->i_blk_cnt = i;
		if (!feof(fp)) {
			fprintf(stderr, "file is too large to be written to kfs\n");
			return 0;
		}
	}
	inode->cksum = kfs_checksum(inode, sizeof(struct kfs_inode) - sizeof(inode->cksum));
	/* seek to correct inode position in order to write it */
	if (_opt & OPT_VERBOSE)
		printf("writing inode to offset %u\n", inode->idx * KFS_BLK_SZ);

	fseek(out, inode->idx * KFS_BLK_SZ, SEEK_SET);
	kfswrite(inode, sizeof(struct kfs_inode), 1, out);

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

	if (_opt & OPT_VERBOSE)
		printf("%u inodes will be written.\n", *inode_cnt);

	for (i = 0; *files; ++i, ++files) {
		FILE *fp = fopen(*files, "r");
		if (!fp) {
			fprintf(stderr, "error opening file %s in read mode: %s\n", *files, strerror(errno));
			return 0;
		}
		blk_idx = kfs_write_inode(out, fp, inodes[i], blk_idx);
		fclose(fp);
	}
	return blk_idx;
}

static inline void usage(void)
{
	printf("usage: mkkfs [-v] -r rom_file -n name -f "
	       "file_1 file_2 ... file_n\n");
}

int main(int argc, char **argv)
{
	char **ptr, **files, *rom_file, *rom_name;

	if (argc < 3) {
		usage();
		return EXIT_FAILURE;
	}
	ptr = files = NULL;
	rom_file = rom_name = NULL;

	/* parse command line. */
	for (ptr = ++argv; *ptr;) {
		if (**ptr == '-') {
			if (!strcmp(*ptr, "-v")) {
				_opt |= OPT_VERBOSE;
				++ptr;
				continue;
			}
			if (!strcmp(*ptr, "-f")) {
				if (!ptr[1] || is_opt(ptr[1])) {
					fprintf(stderr, "-f option must be followed by file names.\n");
					exit(EXIT_FAILURE);
				}
				files = ptr + 1;
				for (++ptr; *ptr && !is_opt(*ptr); ++ptr) ;
				continue;
			}
			if (!strcmp(*ptr, "-r")) {
				if (!ptr[1] || is_opt(ptr[1])) {
					fprintf(stderr, "-r option must be followed by file name.\n");
					exit(EXIT_FAILURE);
				}
				rom_file = ptr[1];
				ptr += 2;
				continue;
			}
			if (!strcmp(*ptr, "-n")) {
				if (!ptr[1] || is_opt(ptr[1])) {
					fprintf(stderr, "-n option must be followed by file system name.\n");
					exit(EXIT_FAILURE);
				}
				rom_name = ptr[1];
				ptr += 2;
				continue;
			}
		}
		fprintf(stderr, "invalid option %s.\n", *ptr);
		exit(EXIT_FAILURE);
	}
	if (!files || !rom_file || !rom_name) {
		fprintf(stderr, "missing argument.\n");
		usage();
		exit(EXIT_FAILURE);
	}
	FILE *rom = fopen(rom_file, "w");
	if (!rom) {
		fprintf(stderr, "error opening file %s for writing: %s\n", rom_file, strerror(errno));
		return EXIT_FAILURE;
	}
	if (_opt & OPT_VERBOSE)
		printf("block size: %u\n", KFS_BLK_SZ);

	uint32_t blk_cnt, inode_cnt;
	if (!(blk_cnt = kfs_write_files(rom, files, 1, &inode_cnt))) {
		fclose(rom);
		return EXIT_FAILURE;
	}
	kfs_write_superblock(rom, rom_name, blk_cnt, inode_cnt);
	fclose(rom);

	return EXIT_SUCCESS;
}
