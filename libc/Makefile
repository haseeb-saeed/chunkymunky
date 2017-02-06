HOST?=$(shell ../default-host.sh)
HOSTARCH:=$(shell ../target-triplet-to-arch.sh $(HOST))

CPPFLAGS?=-O2
LDFLAGS?=
LIBS?=

DESTDIR?=../sysroot
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

AR:=$(HOST)-ar
CXX:=$(HOST)-g++

CPPFLAGS:=$(CPPFLAGS) -ffreestanding -Wall -Wextra -fno-exceptions -D__is_libc -Iinclude
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_libk

OBJS=\
string/memcpy.o \
string/memmove.o \
string/memset.o \
string/memchr.o \
string/memcmp.o \
string/strlen.o \
string/strrev.o \
string/itoa.o \
string/utoa.o \

LIBK_OBJS=$(OBJS:.o=.libk.o)
BINARIES=libk.a

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libk.o .cc

all: $(BINARIES)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $(BINARIES) $(LIBK_OBJS)

.cc.o:
	$(CXX) -MD -c $< -o $@ $(CPPFLAGS)

.cc.libk.o:
	$(CXX) -MD -c $< -o $@ $(LIBK_CPPFLAGS)

clean:
	rm -f $(BINARIES) *.a
	rm -rf $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -rf $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-libs

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(OBJS:.o=.d)
-include $(LINK_OBJS:.o=.d)
