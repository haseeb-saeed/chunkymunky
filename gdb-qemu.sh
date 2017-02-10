#!/bin/sh

./iso.sh
qemu-system-$(./target-triplet-to-arch.sh $(./default-host.sh)) -s -S -cdrom chunkymunky.iso
