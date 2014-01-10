#!/bin/bash
export CSTD := -std=c11
export SORT_DIR := $(CURDIR)
export CC := gcc -g $(CSTD)
export CPPSTD := -std=c++11 -std=gnu++11
export WFLAGS := -Wall -g
export G++ := g++ -g $(CPPSTD)
export BIN = $(SORT_DIR)/bin
export INCLUDE = $(SORT_DIR)/include
SUBDIRS = module sort_base 
export target := sorting_test
all:  
	@mkdir -p  $(BIN); \
	for dir in ${SUBDIRS}; do\
		cd $$dir; \
		make all; \
                cd .. ;\
	done; \
	$(G++) -shared $(BIN)/module.o $(BIN)/sort_base.o -o $(BIN)/libsort.so \
	$(CC) -c  -o $(BIN)/main.o -Iinclude/ main.c ; \
	$(CC) -o $(BIN)/$(target) $(BIN)/main.o -lsort -L$(BIN); \
	sudo cp $(BIN)/libsort.so /usr/lib/. ; \
	sudo ldconfig

clean: 
	sudo rm /usr/lib/libsort.so; \
	cd $(BIN); \
	rm *  ;\	
	cd $(SORT_DIR)
install:
	sudo cp $(BIN)/*.so  /usr/lib/.
