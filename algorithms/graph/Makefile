#!/bin/bash
export CSTD := -std=c11
export CC := gcc -g $(CSTD)
export CPPSTD := -std=c++11 -std=gnu++11
export WFLAGS := -Wall -g
export G++ := g++ -g $(CPPSTD) -gdwarf-2
export BIN := bin
export INCLUDE := include
export SRC := src
export target := test
all: $(SRC)/graph.cpp $(INCLUDE>/edge.h $(INCLUDE)/graph.h $(INCLUDE)/search.h $(INCLUDE)/BFS.h $(SRC)/BFS.cpp $(SRC)/DFS.cpp $(INCLUDE)/DFS.h  $(SRC)/prim.cpp $(INCLUDE)/prim.h $(SRC)/test.cpp  $(INCLUDE)/compare.h $(INCLUDE)/kruskal.h $(SRC)/kruskal.cpp
	@mkdir -p  $(BIN); \
	cp graph.txt $(BIN); \
	$(G++)  -c  -o $(BIN)/graph.o -I. $(SRC)/graph.cpp ; \
	$(G++) -c  -o $(BIN)/BFS.o  -I. $(SRC)/BFS.cpp; \
	$(G++) -c -o $(BIN)/DFS.o -I. $(SRC)/DFS.cpp;\
	$(G++) -c -o $(BIN)/prim.o -I. $(SRC)/prim.cpp;\
	$(G++) -c -o $(BIN)/kruskal.o  -I. $(SRC)/kruskal.cpp;\
	$(G++) $(BIN)/graph.o $(BIN)/DFS.o $(BIN)/BFS.o $(BIN)/prim.o $(BIN)/kruskal.o -I. $(SRC)/test.cpp -o $(BIN)/test


clean: 
	cd $(BIN); \
	rm *  ;\
	cd $(SRC)
