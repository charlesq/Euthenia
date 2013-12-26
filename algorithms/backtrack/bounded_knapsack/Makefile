TARGET := test
test: main.cpp
	g++ -g $^ -std=c++11 -std=gnu++11 -I/usr/include/mysql -lmysqlclient -gdwarf-2 -pthread  -o $(TARGET)
all: test


