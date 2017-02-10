#!/bin/sh

./iso.sh
qemu-system-$(./target-triplet-to-arch.sh $(./default-host.sh)) -cdrom chunkymunky.iso
