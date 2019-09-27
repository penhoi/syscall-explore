#!/usr/bin/make

CFLAGS      += -O2 -g
CPPFLAGS    += ${CFLAGS}

C_sources   := $(wildcard *.c)
CPP_sources := $(wildcard *.cpp)

C_targets   := $(patsubst %.c,%,${C_sources})
CPP_targets := $(patsubst %.cpp,%,${CPP_sources})
ALL_targets := ${C_targets} ${CPP_targets}

BUILD_DIR	:= ./build


.PHONY: CREATE_BUILD_DIR


all: CREATE_BUILD_DIR ${ALL_targets}


CREATE_BUILD_DIR: 
	mkdir -p ${BUILD_DIR}


${C_targets}: %: %.c
	gcc ${CFLAGS}  $< -o ${BUILD_DIR}/$@


${CPP_targets}: %: %.cpp
	g++ ${CFLAGS}  $< -o ${BUILD_DIR}/$@


run: all
	@for exe in ${ALL_targets}; do echo "--------------execute $$exe ------------";  ./${BUILD_DIR}/$$exe; done


clean:
	rm -f ${ALL_targets} a.out
	rm -rf ${BUILD_DIR}
