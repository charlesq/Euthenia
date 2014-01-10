STDFLAGS := -std=c++11 -std=gnu++11 
CFLAGS :=   -gdwarf-2
G++ := g++
BIN := bin
TARGET2 := $(BIN)/abstract_cache.o 
TARGET1 := $(BIN)/cache_imp.o
SRC := main
TARGET := $(BIN)/$(SRC)
INCLUDE := $(CURDIR)
all: $(TARGET)
.PHONY: abstract
.PHONY: imp
imp: $(TARGET1)
abstract:$(TARGET2)

$(TARGET): $(SRC).cpp
	mkdir  -p $(BIN); \
	$(G++) $(CFLAGS) $(STDFLAGS) -o $@ -I$(INCLUDE)  $^ ;
$(BIN)/%.o: %.hpp delay.h
	$(G++) -c $(CFLAGS) $(STDFLAGS)  -I$(INCLUDE) -o $@ $< ;

clean:
	rm $(BIN)/*; \

