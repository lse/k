# Introduction

## Table of contents

1. [Checkout out](#checking-out)
    1. [The tarball](#the-tarball)
    1. [Dependancies](#dependancies-for-building)
    3. [Source tree](#source-tree)
    4. [Intel Manuals](#intel-manuals)
2. [Build System](#build-system)
3. [Booting your kernel](#booting-your-kernel-in-qemu)
4. [Debugging your kernel](#debugging-your-kernel)

## Checking out

### The tarball

A readonly git repository given by the LSE can be cloned:

    git clone git://git.lse.epita.fr/k.git

### Dependancies for building

* gcc-multilib
* grub2
* libisoburn
* find

### Source tree

Here is the description of some important files:

    Makefile            # top-level Makefile
    config.mk           # build-system configuration
    k                   # kernel source folder
    k/elf.h             # ELF header
    k/crt0.S            # crt0 for the kernel
    k/k.c               # kernel entry point
    k/multiboot.h       # Multiboot Specification header
    k/k.lds             # LD script for the kernel binary
    k/memory.c          # kernel memory allocator
    k/include/k/atapi.h # ATAPI related definitions
    k/include/k/kstd.h  # k standard definitions
    k/include/k/kfs.h   # KFS structures definitions
    k/include/k/types.h # kernel types definitions
    roms                # rom folder
    roms/chichepong     # chichepong folder
    roms/roms.lds       # LD script for rom binaries
    libs                # SDK folder
    libs/libc           # a basic libc available everywhere
    libs/libk           # userland functions
    tools               # Tools folder
    tools/mkksf         # small program to generate your own sounds
    tools/mkkfs         # small program to create kfs roms
    tools/create-iso.sh # small tool to generate the iso image

### Intel Manuals

You will find the essential Intel Manuals describing the processor, all
the instructions as well as the programming guide on the [intel
website][1].

The most interesting one is probably the "Volume 3A: System Programming
Guide" which describe everything that is needed to develop an operating
system.

[1]: http://www.intel.com/products/processor/manuals/

## Build System

The build system uses information stored in config.mk. Feel free to modify this
file, but your project must work with the original one.

Here are make rules you need to know:

    make | make k.iso  # create an ISO with all the roms
    make k             # compile your kernel
    make rom/GAME      # compile the rom in the folder rom/$(GAME)
    make clean         # clean the tree

## Booting your kernel in qemu

    qemu-system-x86_64 -cdrom k.iso [ -enable-kvm ]

## Debugging your kernel

Build your kernel with debug flags.

* Run QEMU with a gdb server and stop the CPU at the first instruction:

```bash
    qemu-system-x86_64 -cdrom k.iso -s -S
```

* Run gdb with your kernel binary:

```bash
gdb k/k
```

* Once in gdb, connect to QEMU:

```bash
target remote localhost:1234
```

* Add some breakpoints:

```bash
b my_symbol
```

* Run the simulation in gdb:

```bash
continue
```
