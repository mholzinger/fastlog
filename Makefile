# Makefile for fastlog API.
# Compatible with Linux, Darwin

VERSION    = $(VERMAJOR).$(VERMINOR).$(VERSUBMIN)
VERMAJOR  := 0
VERMINOR  := 1
VERSUBMIN := 0

# variables
INSTALL  := install -D --owner 0 --group 0
LDCONFIG := /sbin/ldconfig
ln_f     := ln -f
ln_sf    := ln -sf

# directory vars
LIB64    := lib$(shell [ -d /usr/lib64 ] && echo 64)
HOMEDIR  := $(shell echo $$HOME)
LOGHEADER = libfastlog
PROGHDRS := include

FILES = Makefile           \
		assets_pkg/        \
		include/           \
		source/

## system vars
ARCHTYPE := $(shell uname -m)
OS 		 := $(shell uname -s)

## command vars
CP       := cp
RM       := rm -rf

## project vars
TESTPROG := test
PROG     := libfastlog
PROGOBJS := test.o fastlog.o

# Remove $(junk) from all project $(DIRS)
DIRS  :=
junk  := *~ *.o
CLEAN += $(junk) $(foreach dir,$(DIRS),$(addprefix $(dir), $(junk)))

## compiler vars
CC       := gcc
CFLAGS    = $(DEBUG_FLAGS) $(OPTIMIZATIONS) -D_GNU_SOURCE -D_POSIX_SOURCE \
		    -D_REENTRANT -pedantic -std=c99 -Wall -Wextra -Wmissing-prototypes \
		    -Wredundant-decls -Wstrict-prototypes -Wundef
#		    -Wredundant-decls -Wshadow -Wstrict-prototypes -Wundef

COMPILE_LIB      = $(CC) $(CFLAGS) $(CPPFLAGS) $(LARGEFILES) $(PLATFORM) -c
COMPILE_EX      = $(CC) $(CFLAGS) $(CPPFLAGS) $(LARGEFILES) $(PLATFORM)
CPPFLAGS       = -Iinclude
LINKOPT       := -fPIC
OUTPUT_OPTION  = -o $@

vpath %.c $(CURDIR)/source
vpath %.h $(CURDIR)/include

## debug vars
ifdef TDEBUG
	DEBUG_FLAGS   := -DTDEBUG -g -ggdb
	OPTIMIZATIONS := -O0
else
	DEBUG_FLAGS   :=
endif

ifeq ($(ARCHTYPE),x86_64)
	LARGEFILES :=
	LIBDIR     := $(DESTDIR)/usr/lib64
	ifeq ($(UBUNTU),Ubuntu)
		LIBDIR := $(DESTDIR)/usr/lib
	endif
else
	LARGEFILES := -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64
	LIBDIR     := $(DESTDIR)/usr/lib
endif

## architecture vars

# 8.04 64-bit uses /usr/lib as its default and /usr/lib32 for 32-bit libs.
UBUNTU ?= $(shell if [ -e /etc/lsb-release ]; then grep DISTRIB_ID /etc/lsb-release | cut -d '=' -f2; fi)
RHEL   ?= $(shell if [ -e /etc/redhat-release ]; then grep -i release /etc/redhat-release | cut -d ' ' -f7; fi)
SLE    ?= $(shell if [ -e /etc/SuSE-release ]; then grep -i version /etc/SuSE-release | cut -d ' ' -f3; fi)
CENTOS ?= $(shell if [ -e /etc/redhat-release ]; then grep -i release /etc/redhat-release | cut -d ' ' -f3 | cut -d '.' -f1; fi)

# darwin or linux?
ifeq ($(OS),Darwin)
	#BUILD_ARCH := -arch i386 -arch ppc
	BUILD_ARCH := -arch i386 -arch x86_64
	LIBDIR    := $(DESTDIR)/usr/local/lib
	LIBTOOL   := libtool -static -o
	SOEXT     := dylib
	BUILD_LIB := -dynamiclib -compatibility_version $(VERMAJOR) \
				 -current_version $(VERSION) -install_name \
				 $(LIBDIR)/$(PROG).$(VERSION).$(SOEXT)
	CFLAGS    += -D_DARWIN_SOURCE -D_DARWIN_C_SOURCE
	LIBNAME    = $(PROG).$(VERSION).$(SOEXT)
	PLATFORM  := -DTOSDARWIN
else
	ifeq ($(OS),Linux)
		SOEXT     := so
		BUILD_LIB := -shared -Wl,-O1 -Wl,-soname,$(PROG).$(SOEXT).$(VERMAJOR) \
		 			 -static-libgcc
		LIBTOOL   := ar rvcs
		CLIB      := -lc
		LIBNAME   := $(PROG).$(SOEXT).$(VERSION)
		PLATFORM  := -DTOSLINUX
	endif
endif

all: clean $(PROGOBJS) $(LIBNAME) $(TESTPROG)

fastlog.o: flog.c
	@echo 'Building target: $@'
	$(COMPILE_LIB) $(BUILD_ARCH) $(LINKOPT) $(OUTPUT_OPTION) $^
	@echo 'Finished building target: $@'
	@echo

test.o: test.c
	@echo 'Building target: $@'
	$(COMPILE_LIB) $(BUILD_ARCH) $(LINKOPT) $(OUTPUT_OPTION) $^
	@echo 'Finished building target: $@'
	@echo

$(TESTPROG): $(TESTPROG).o fastlog.o
	@echo 'Building target: $@'
	$(COMPILE_EX) -o $(TESTPROG) $(TESTPROG).o fastlog.o
	@echo 'Finished building target: $@'
	@echo

#$(LIBNAME): $(PROGOBJS)
$(LIBNAME): fastlog.o
	@printf "#\n# Building $@\n"
	$(CC) $(BUILD_ARCH) $(BUILD_LIB) $(OUTPUT_OPTION) $^ $(CLIB)
	@echo 'Finished building target: $@'
	@echo

.PHONY: clean
clean:
	@printf "#\n# Clean up ... $@\n"
	$(RM) $(CLEAN)
	$(RM) $(TESTPROG)
	$(RM) $(PROG)-$(VERSION)/
	$(RM) $(PROG).$(SOEXT)
	$(RM) $(PROG).$(SOEXT).[0-9]
	$(RM) $(PROG).$(SOEXT).[0-9].[0-9].[0-9]
	$(RM) $(PROG).[0-9].[0-9].[0-9].$(SOEXT)
	$(RM) ._* include/._* source/._*
	@echo ' '

.PHONY: distclean
distclean: clean
	@printf "#\n# Clean up ... $@\n"
	$(RM) $(PROG)-$(VERSION).tar.gz
	$(RM) $(LIBDIR)/$(PROG).$(SOEXT)
	$(RM) $(LIBDIR)/$(PROG).$(SOEXT).[0-9]
	$(RM) $(LIBDIR)/$(PROG).$(SOEXT).[0-9].[0-9].[0-9]
	$(RM) $(LIBDIR)/$(PROG).[0-9].[0-9].[0-9].$(SOEXT)
	@echo ' '

.PHONY: install
install: uninstall
	@printf "#\n# $@ing...\n"
	@if [ "$(OS)" = "Linux" ]; then \
		echo "# Linux installation..."; \
		$(INSTALL) $(PROG).$(SOEXT).$(VERSION) $(LIBDIR)/$(PROG).$(SOEXT).$(VERSION); \
		$(LDCONFIG) -n $(LIBDIR); \
		$(ln_sf) $(PROG).$(SOEXT).$(VERMAJOR) $(LIBDIR)/$(PROG).$(SOEXT); \
	elif [ "$(OS)" = "Darwin" ]; then \
		echo "# Darwin installation..."; \
		$(CP) $(LIBNAME) $(LIBDIR) ; \
		$(ln_sf) $(LIBDIR)/$(LIBNAME) $(LIBDIR)/$(PROG).$(SOEXT); \
	fi
	mkdir -p $(DESTDIR)/usr/include/fastlog
	$(CP) include/fastlog.h $(DESTDIR)/usr/include/fastlog/
	@printf "# ...finito\n"

.PHONY: uninstall
uninstall:
	@printf "#\n# $@...\n"
	@printf "# Removing $(PROG) from $(LIBDIR) if it exists...\n"
	$(RM) $(LIBDIR)/$(PROG).*
	@printf "# ...finito\n"
