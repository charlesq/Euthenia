TARGET := test
GLIBH := $(shell pkg-config --cflags glib-2.0)
GLIB :=  $(shell pkg-config --libs glib-2.0)
test: main.cpp
	g++ -g  -O0 $(GLIBH)  $^   -std=c++11 -std=gnu++11 -I/usr/include/mysql  $(GLIB) -lgobject-2.0   -lmysqlclient -gdwarf-2 -pthread  -o $(TARGET)
all: test


