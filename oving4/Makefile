CC     = g++
OPT    = -O3
DEBUG  = -g
OTHER  = -Wno-deprecated
#CFLAGS = $(OPT) $(OTHER)
CFLAGS = $(DEBUG) $(OTHER)

MODULE = run
SB_SRCS = simple_bus.cpp \
	  simple_bus_arbiter.cpp \
	  simple_bus_types.cpp \
	  simple_bus_tools.cpp 

# modify next line to your own source files
SRCS = $(SB_SRCS) button.cpp control.cpp top.cpp
OBJS = $(SRCS:.cpp=.o)

all: run.x

# modify .o rules for your source files

button.o: if.h button.h button.cpp

control.o: if.h control.h control.cpp

top.o: if.h button.h control.h top.cpp adapter.h

adapter.o: adapter.h adapter.cpp

ifneq (, $(realpath $(HOME)/systemc/Makefile.defs))
include ~/systemc/Makefile.defs
else ifneq (, $(realpath ../Makefile.defs))
include ../Makefile.defs
else
$(error "Cannot find Makefile.defs")
endif
