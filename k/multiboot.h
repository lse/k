/* multiboot.h - the header for Multiboot */
/* Copyright (C) 1999, 2001  Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */
#ifndef MULTIBOOT_H_
#define MULTIBOOT_H_

#define MULTIBOOT_BOOTLOADER_MAGIC	0x2BADB002

typedef struct {
	unsigned int magic;
	unsigned int flags;
	unsigned int checksum;
	unsigned int header_addr;
	unsigned int load_addr;
	unsigned int load_end_addr;
	unsigned int bss_end_addr;
	unsigned int entry_addr;
} multiboot_header_t;

typedef struct {
	unsigned int tabsize;
	unsigned int strsize;
	unsigned int addr;
	unsigned int reserved;
} aout_symbol_table_t;

typedef struct {
	unsigned int num;
	unsigned int size;
	unsigned int addr;
	unsigned int shndx;
} elf_section_header_table_t;

typedef struct {
	unsigned int flags;
	unsigned int mem_lower;
	unsigned int mem_upper;
	unsigned int boot_device;
	unsigned int cmdline;
	unsigned int mods_count;
	unsigned int mods_addr;

	union {
		aout_symbol_table_t aout_sym;
		elf_section_header_table_t elf_sec;
	} u;

	unsigned int mmap_length;
	unsigned int mmap_addr;
} multiboot_info_t;

typedef struct {
	unsigned int mod_start;
	unsigned int mod_end;
	unsigned int string;
	unsigned int reserved;
} module_t;

typedef struct {
	unsigned int size;
	unsigned int base_addr_low;
	unsigned int base_addr_high;
	unsigned int length_low;
	unsigned int length_high;
	unsigned int type;
} memory_map_t;

#endif				/* !MULTIBOOT_H_ */
