#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <sstream>
#include <assert.h>
#include <numeric>
#include <iterator>
#include "stack.hpp"
#include <memory>
#include <algorithm>
#include <functional>
#include <regex>
#include <atomic>
#include <type_traits>
#include <stdlib.h>

using namespace std::placeholders;

class C
{
  char i;
public:
     C() { std::cout << "constructor_" << std::endl; } 
     C(int i_): i(i_) { std::cout << " constructor with argments" << std::endl; }
    ~C() { std::cout << "destructor_" << std::endl; }
    void * operator new  (size_t size)
    {
        std::cout << "overloaded new: " << size <<  std::endl;
        return ::malloc(size);; 
    }
    void * operator new [] (size_t n)
    {
        std::cout << "new an array: " << n << "  "  << std::endl;
        return ::malloc(sizeof (C) * n );
    }
    void operator delete (void *p)
    {
         std::cout << "to delete " << std::endl;
         free( p);
    }
    void operator delete [] (void *p)
    {
        std::cout << "delete an array " << std::endl;
        free( p);
    }


};

int main(int argc, char *argv[])
{
    C * t = new  C(3);
    C * a = new C[1];
    std::cout << std::hex << t << "  " << std::hex << a << std::endl;
    C *t1 = new C();
    C *a1 = new C[1]; 
    std::cout << std::hex << t1 << "  " << std::hex << a1 << std::endl;
    delete t;
    delete [] a; 
    return 0;
}


