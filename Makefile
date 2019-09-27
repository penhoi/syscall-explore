#!/usr/bin/make

CFLAGS      += -O2 -g
CPPFLAGS    += ${CFLAGS}

C_sources   := $(wildcard *.c)
CPP_sources := $(wildcard *.cpp)

C_targets   := $(patsubst %.c,%,${C_sources})
CPP_targets := $(patsubst %.cpp,%,${CPP_sources})
ALL_targets := ${C_targets} ${CPP_targets}


all: ${ALL_targets}


${C_targets}: %: %.c
	gcc ${CFLAGS}  $< -o $@


${CPP_targets}: %: %.cpp
	g++ ${CFLAGS}  $< -o $@


run: all
	@for exe in ${ALL_targets}; do echo "--------------execute $$exe ------------";  ./$$exe; done


clean:
	rm -f ${ALL_targets}
