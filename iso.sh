#!/bin/sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/chunkymunky.kernel isodir/boot/chunkymunky.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "chunkymunky" {
	multiboot /boot/chunkymunky.kernel
}
EOF
grub-mkrescue -o chunkymunky.iso isodir
