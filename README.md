K
=

dependancies for building
--------------------------

* gcc-multilib
* grub2 (for grub-mkrescue)
* libisoburn
* find

launch k
--------
	$ qemu-system-x86_64 -cdrom k.iso -serial stdio

TODO
----

* rework syscall apis
* change fs (iso? something else?)
* propagation of all changes in courses
* maybe rename libc/libk
* rework/rewrite *all* roms
* more userland tests for the students
* how the sound is working?
* implement mouse support in ref
