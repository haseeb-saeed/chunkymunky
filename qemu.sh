#!/bin/sh

qemu-system-$(./target-triplet-to-arch.sh $(./default-host.sh)) -kernel chunkymunky.kernel
