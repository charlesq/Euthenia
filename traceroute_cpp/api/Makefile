#!/bin/bash
#export CC := gcc -g
#export CPPSTD := -std=c++11 -std=gnu++11
#export WFLAGS := -Wall -g
#export G++ := g++ -g  $(CPPSTD)
#export CSTD := -std=c11
DIRS :=
all: $(BIN)/lib$(TRACELIB).so
$(BIN)/lib$(TRACELIB).so: $(BIN)/traceroute_options.o $(BIN)/$(TRACELIB).o
	$(G++) -shared -o $@  $^ ; \
	sudo cp ${BIN}/lib${TRACELIB}.so /usr/lib/. ; \
        sudo ldconfig;



$(BIN)/traceroute_options.o: traceroute_options.c $(INCLUDE)/traceroute_options.h
	$(G++) -c  -fpic  -I$(INCLUDE) -I$(HPP) $^ ;\
	mv traceroute_options.o $@; 


$(BIN)/$(TRACELIB).o: traceroute_api.c $(HPP)/traceroute.hpp $(HPP)/sock_eng.hpp $(INCLUDE)/message.h $(HPP)/sock_lib.hpp $(HPP)/probe.hpp $(HPP)/tcp_eng.hpp  $(INCLUDE)/sock_lib_args.h
	$(G++) -c -Wall -Werror -fpic -I$(INCLUDE) -I$(HPP) -o $@ $<;
clean:

install:
	sudo cp ${BIN}/lib${TRACELIB}.so /usr/lib/. ; \
	sudo ldconfig;
