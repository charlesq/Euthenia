all: $(BIN)/sort_base.o
$(BIN)/sort_base.o: sort_base.cpp $(INCLUDE)/sort_base.h  $(INCLUDE)/module.h $(INCLUDE)/quick_sort.hpp $(INCLUDE)/merge_sort.hpp $(INCLUDE)/radix_sort.hpp $(INCLUDE)/counting_sort.hpp
	$(G++)  -c -fPIC -g  -I$(INCLUDE)   -o $@  $<;
	

