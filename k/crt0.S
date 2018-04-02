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
#include "multiboot.h"

#define HEADER_FLAGS (MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO)
#define STACK_SIZE	8192

	.section .multiboot
	.type multiboot_header, @object
multiboot_header:
	.align		MULTIBOOT_HEADER_ALIGN
	.long		MULTIBOOT_HEADER_MAGIC
	.long		HEADER_FLAGS
	.long		-(MULTIBOOT_HEADER_MAGIC + HEADER_FLAGS)
	.size multiboot_header, . - multiboot_header

	.section .text
	.global k_entry
	.type k_entry, @function
k_entry:
	lea end_stack, %esp
	push %ebx	/* multiboot info */
	push %eax	/* magic */
	call k_main	/* kernel entry point */
.Lend:
	jmp .Lend
	.size k_entry, . - k_entry

.section .bss
stack:
	.space STACK_SIZE
	.align 16
end_stack:
.global end_stack
