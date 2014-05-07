# name of binary
MODNAME := notifyd

# input path to src dir
SRCDIR  := $(PWD)

# build path
TMPDIR  := $(SRCDIR)/.build.$(MODNAME)

# path for binary
BINDIR  := $(SRCDIR)

# use if you do not want to use default (debug|release)
#OBJDIR  :=

ALL_CFLAGS   = -Wall -Wno-deprecated -g -rdynamic # -O2 -fno-strict-aliasing

# specific flags
CFLAGS   := -std=c++11

# specific libs
LIBS     :=

# specific includes
INCLUDES :=

ALL_INCLUDES = -I./

# list of *.cpp
SOURCES :=                                              \
            $(SRCDIR)/main.cpp                          \

.PHONY: all
all:
	@echo compiling
	g++ $(SOURCES) $(ALL_INCLUDES) $(LIBS) $(CFLAGS) -o $(MODNAME)
	@echo ok

