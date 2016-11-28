#!/bin/sh

iso_filename=$1

mkdir -p root/
mkdir -p root/roms/
mkdir -p root/boot/grub/

cp k/k root/

find roms -name "*.rom" -exec cp {} root/roms/ \;

for rom in $(find root/roms -name "*.rom") ; do
	name=$(basename $rom .rom)
	cat <<EOF
menuentry "k - $name" {
	multiboot /k /$name
	module /roms/$name.rom
}
EOF
done > root/boot/grub/grub.cfg

grub-mkrescue -o $iso_filename root/
