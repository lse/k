#!/bin/sh

set -e

iso_filename=$1
base_dir=$2

unset MFLAGS MAKEFLAGS

if command -v grub-mkrescue >/dev/null 2>&1; then
	mkrescue=grub-mkrescue
elif command -v grub2-mkrescue >/dev/null 2>&1; then
	mkrescue=grub2-mkrescue
else
	echo 'grub-mkrescue not found, aborting...' >&2
	exit 1
fi

mkdir -p $base_dir/boot/grub/

get_make_var()
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

$mkrescue -o $iso_filename $base_dir
