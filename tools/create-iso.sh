#!/bin/sh

iso_filename=$1
base_dir=$2

mkdir -p $base_dir/boot/grub/

function get_make_var()
{
	make -pn -C $2 | grep "^$1 = " | cut -d' ' -f 3-
}

shift 2
for i in $@; do
	cat <<EOF
menuentry "k - $(get_make_var ROM_TITLE "$i")" {
	multiboot /k /bin/$(get_make_var TARGET "$i")
}
EOF
done > $base_dir/boot/grub/grub.cfg

grub-mkrescue -o $iso_filename $base_dir
