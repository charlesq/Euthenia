#!/bin/bash
export TR_DIR := $(CURDIR)
export CC := gcc
export CPPSTD := -std=c++11 -std=gnu++11
export WFLAGS := -Wall -Werror -gdwarf-2
export G++ := g++ -g  $(CPPSTD) $(WFLAGS) 
export BIN = $(TR_DIR)/bin
export INCLUDE := $(TR_DIR)/include
export LD_LIBRARY_PATH=$(BIN)
export HPP := $(TR_DIR)/hpp
export CSTD := -std=c11
export CC := gcc -g  $(CSTD) $(WFLAGS) 
export TRACELIB := traceroute_api
DIRS := api
all:
	$(CC) -c -Iinclude main.c ;\
	mv main.o $(BIN)/. ;\
	mkdir -p $(BIN); \
	for dir in ${DIRS}; do\
		cd $$dir; \
		make all; \
		cd .. ;\
	done; \
	cd $(BIN);\
	$(CC) -o traceroute main.o  -l$(TRACELIB) -L.  ;

clean:
	sudo rm /usr/lib/lib$(TRACELIB).so; \
	cd $(BIN);\
	rm -fr *;\
	cd $(TR_DIR);

install:
	for dir in ${DIRS}; do \
		cd $$dir; \
		sudo make install; \
		cd $(TR_DIR); \
	done;
