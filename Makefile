HOST?=$(shell ./default-host.sh)
HOSTARCH:=$(shell ./target-triplet-to-arch.sh $(HOST))

CPPFLAGS?=-O2
ASMFLAGS?=
LDFLAGS?=
LIBS?=

#DESTDIR?=
#PREFIX?=/usr/local
#EXEC_PREFIX?=$(PREFIX)
#BOOTDIR?=$(EXEC_PREFIX)/boot
#INCLUDEDIR?=$(PREFIX)/include

CXX:=$(HOST)-g++
NASM:=nasm

CPPFLAGS:=$(CPPFLAGS) -ffreestanding -Wall -Wextra -fno-exceptions
ASMFLAGS:=-felf32
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lgcc

ARCHDIR:=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(ARCH_LDFLAGS)
LIBS:=$(LIBS) $(ARCH_LIBS)

OBJS:=\
$(ARCH_OBJS) \
kernel/kmain.o \

CRTI_OBJ:=$(ARCHDIR)/crti.o
CRTBEGIN_OBJ:=$(shell $(CXX) $(CFLAGS) $(LDFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CXX) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o)
CRTN_OBJ:=$(ARCHDIR)/crtn.o

ALL_OUR_OBJS:=\
$(CRTI_OBJ) \
$(OBJS) \
$(CRTN_OBJ) \

OBJ_LINK_LIST:=\
$(CRTI_OBJ) \
$(CRTBEGIN_OBJ) \
$(OBJS) \
$(CRTEND_OBJ) \
$(CRTN_OBJ) \

all: chunkymunky.kernel

.PHONY: all clean# install install-headers install-kernel

chunkymunky.kernel: $(OBJ_LINK_LIST) $(ARCHDIR)/linker.ld
	$(CXX) -T $(ARCHDIR)/linker.ld -o $@ $(CPPFLAGS) $(OBJ_LINK_LIST) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CXX) -c $< -o $@ $(CPPFLAGS)

%.o: %.asm
	$(NASM) $< -o $@ $(ASMFLAGS)

clean:
	rm -f chunkymunky.kernel $(OBJS) $(ALL_OUR_OBJS) *.o */*.o */*/*.o
#
#install: install-headers install-kernel
#
#install-headers:
#	mkdir -p $(DESTDIR)$(INCLUDEDIR)
#	cp -RTv include $(DESTDIR)$(INCLUDEDIR)
#
#install-kernel: chunkymunky.kernel
#	mkdir -p $(DESTDIR)$(BOOTDIR)
#	cp chunkymunky.kernel $(DESTDIR)$(BOOTDIR)
