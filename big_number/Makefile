#!/bin/bash
CFLAGS :=   -gdwarf-2
G++ := g++
BIN := bin
STDFLAGS := -std=c++11 -std=gnu++11
SRC := big_number
TARGET := lib$(SRC).so
INCLUDE := $(CURDIR)
TEST := test
all: $(BIN)/$(TEST)

$(BIN)/$(TARGET): $(SRC).hpp
	mkdir  -p $(BIN); \
	$(G++) -c -fPIC  $(CFLAGS) $(STDFLAGS) -I$(INCLUDE) -o $(BIN)/$(SRC).o  $^ ; \
	$(G++) -shared -o $(BIN)/$(TARGET) $(BIN)/$(SRC).o; \
	sudo ldconfig; \

$(BIN)/$(TEST): $(TEST).cpp  $(SRC).hpp
	$(G++) $(CFLAGS) $(STDFLAGS)   -I$(INCLUDE) -o $@ $^; 


clean:
	rm $(BIN)/*; \

#%.hpp$(BIN)%.o:
