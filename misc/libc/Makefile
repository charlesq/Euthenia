TARGET := test
GLIBH := $(shell pkg-config --cflags glib-2.0)
GLIB :=  $(shell pkg-config --libs glib-2.0)
test: main.c
	g++ -g  -O0   $^   -std=c++11 -std=gnu++11 -gdwarf-2  -o $(TARGET)
all: test


