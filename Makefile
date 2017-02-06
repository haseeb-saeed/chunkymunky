HOST?=$(shell ./default-host.sh)
HOSTARCH:=$(shell ./target-triplet-to-arch.sh $(HOST))

CPPFLAGS?=-O2 -g
ASMFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=sysroot
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
#BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CXX:=$(HOST)-g++
NASM:=nasm

CPPFLAGS:=$(CPPFLAGS) -ffreestanding -Wall -Wextra -fno-exceptions -I$(DESTDIR)$(INCLUDEDIR)
ASMFLAGS:=-felf32
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lgcc -L $(DESTDIR)$(LIBDIR) -lk

ARCHDIR:=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(ARCH_LDFLAGS)
LIBS:=$(LIBS) $(ARCH_LIBS)

OBJS:=\
$(ARCH_OBJS) \
kernel/io.o \
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

.PHONY: all clean install install-headers# install-kernel
.SUFFIXES: .o .cc .asm

chunkymunky.kernel: $(OBJ_LINK_LIST) $(ARCHDIR)/linker.ld
	$(CXX) -T $(ARCHDIR)/linker.ld -o $@ $(CPPFLAGS) $(OBJ_LINK_LIST) $(LDFLAGS) $(LIBS)

%.o: %.cc
	$(CXX) -c $< -o $@ $(CPPFLAGS)

%.o: %.asm
	$(NASM) $< -o $@ $(ASMFLAGS)

clean:
	rm -f chunkymunky.kernel $(OBJS) $(ALL_OUR_OBJS) *.o */*.o */*/*.o

install: install-headers# install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -RTv arch/include $(DESTDIR)$(INCLUDEDIR)/arch
	cp -RTv kernel/include $(DESTDIR)$(INCLUDEDIR)/kernel
#
#install-kernel: chunkymunky.kernel
#	mkdir -p $(DESTDIR)$(BOOTDIR)
#	cp chunkymunky.kernel $(DESTDIR)$(BOOTDIR)
