#!/bin/sh

qemu-system-$(./target-triplet-to-arch.sh $(./default-host.sh)) -s -S -kernel chunkymunky.kernel
