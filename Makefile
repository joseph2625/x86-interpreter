SHELL := /bin/bash

srcdir          := ./
SRCDIR          := $(srcdir)src/
OBJDIR_DEBUG    := $(srcdir)obj-debug/
OBJDIR_RELEASE  := $(srcdir)obj-release/
BINDIR          := $(srcdir)bin/
INCLUDEDIR      := $(srcdir)include/
NAME_DEBUG      := x86-interpreter-debug
NAME_RELEASE    := x86-interpreter-release

SFILES  := c
OFILES  := o
CC      := gcc
CFLAGS  := -Wall -std=c99 -m32 -fms-extensions -D_POSIX_C_SOURCE=199309
CFLAGS_DEBUG := $(CFLAGS) -ggdb
CFLAGS_RELEASE := $(CFLAGS) -O3

LDFLAGS := -m32 -lpthread -lrt

SOURCES := $(SRCDIR)image.c $(SRCDIR)interpreter.c $(SRCDIR)runtime.c $(SRCDIR)main.c $(SRCDIR)syscall.c $(SRCDIR)pe.c
OBJECTS_DEBUG := $(patsubst $(SRCDIR)%.$(SFILES), $(OBJDIR_DEBUG)%.$(OFILES), $(SOURCES))
OBJECTS_RELEASE := $(patsubst $(SRCDIR)%.$(SFILES), $(OBJDIR_RELEASE)%.$(OFILES), $(SOURCES))
ALLFILES := $(SOURCES)

all:  $(NAME_DEBUG) $(NAME_RELEASE)

$(NAME_DEBUG): $(OBJECTS_DEBUG)
	@$(CC) $(LDFLAGS) $^ -o $(BINDIR)$@

$(NAME_RELEASE): $(OBJECTS_RELEASE)
	@$(CC) $(LDFLAGS) $^ -o $(BINDIR)$@

$(OBJDIR_DEBUG)%$(OFILES):  $(SRCDIR)%$(SFILES)
	@$(CC) $(CFLAGS_DEBUG) -I$(INCLUDEDIR) -c $< -o $@

$(OBJDIR_RELEASE)%$(OFILES):  $(SRCDIR)%$(SFILES)
	@$(CC) $(CFLAGS_RELEASE) -I$(INCLUDEDIR) -c $< -o $@

clean:
	@rm -f $(OBJECTS_DEBUG) $(OBJECTS_RELEASE) $(EXE)
