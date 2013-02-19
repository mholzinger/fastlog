ARCHTYPE = $(shell uname -m)
CC = gcc
CFLAGS = $(PLATFORM) -std=c99 -D_POSIX_SOURCE -pedantic -Wall $(DEBUG_FLAGS) \
	-D_GNU_SOURCE $(CXXFLAGS) -I$(INCLUDEPATH)
COMPILE.c = $(CC) $(CFLAGS) $(PLATFORM) -c
OS = $(shell uname -s)
OUTPUT_OPTION = -o $@
INCLUDEPATH = $(CURDIR)/include
PROG = test
PROGOBJS = test.o flog.o 
RM = rm -rf

vpath %.c source

ifeq ($(OS),Darwin)
	CXXFLAGS   = -D_DARWIN_C_SOURCE
	PLATFORM   := -DTOSDARWIN
else
	ifeq ($(OS),Linux)
		PLATFORM   := -DTOSLINUX
	endif
endif

ifdef TDEBUG
	DEBUG_FLAGS = -DTDEBUG -g -ggdb -Wall
else
	DEBUG_FLAGS =
endif

all: clean $(PROG)

.PHONY: clean
clean:
	@echo 'clean'
	$(RM) *.o $(PROG) *._* *~
	@echo ' '

$(PROG): $(PROGOBJS)
	@echo 'Building target: $@'
	$(CC) $(LDFLAGS) $(OUTPUT_OPTION) $(PROGOBJS)
	@echo 'Finished building target: $@'
	@echo ' '

$(PROG).o: source/$(PROG).c
	@echo 'Building target: $@'
	$(CC) $(CFLAGS) -c $(OUTPUT_OPTION) source/$(PROG).c
	@echo 'Finished building target: $@'
	@echo ' '
