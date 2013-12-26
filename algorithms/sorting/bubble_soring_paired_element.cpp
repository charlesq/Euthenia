#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <stdint.h>
#include <forward_list>
#include <stdlib.h>

typedef bool (*compare)(const std::pair<int, size_t> *, const std::pair<int, size_t> *);
#define nth_element(a,es, n)  reinterpret_cast <std::pair<int, size_t>*>(reinterpret_cast<char *>(a) + (es) * (n) ) 
#define nth_element_const(a,es, n) reinterpret_cast<const std::pair<int, size_t>*>(reinterpret_cast<const char *>(a) + (es) * (n) )
void bubble_sort(std::pair<int, size_t> *a, size_t sz, compare  c)
{
     for (size_t i = sz -1; i != -1; i --)
     {
         for (size_t j = 0; j < i; j ++)
         {
             if (c(nth_element_const(a, sizeof(std::pair<int, size_t>), j + 1), nth_element_const(a, sizeof(std::pair<int, size_t>), j )))
                 std::swap(*nth_element(a, sizeof(std::pair<int, size_t>), j+ 1), *nth_element(a, sizeof(std::pair<int, size_t>) , j)); 
         } 
     }       
} 

bool comp(const std::pair<int, size_t> *a, const std::pair<int, size_t> *b)
{
    return a->first < b->first;
}
std::pair<int, size_t> * generate_array(size_t sz)
{
    std::pair<int, size_t> *ret = new std::pair<int, size_t >[sz], *p = ret;
    while(sz-- != 0)
    {
       p->first = sz;
       p->second = rand()%300;
       p ++;
    }
    return ret;
}

void fisher_yates_shuffle(std::pair<int, size_t> *a, size_t sz)
{
    for (size_t i = 1; i < sz; i ++)
    {
        size_t j = rand()%(i + 1);
        std::swap(*nth_element(a, sizeof (std::pair<int, size_t>), j), *nth_element(a, sizeof(std::pair<int, size_t>), i));
    }
}


void print_element(const std::pair<int, size_t> *e)
{
     std::cout << "(" << e->first << "," << e->second << ")  ";
}


void print_array(const std::pair<int, size_t> *a, size_t sz)
{
   const std::pair<int, size_t> *e;
   for (size_t i = 0; i < sz; i ++)
   {
        print_element(nth_element_const(a, sizeof(std::pair<int ,size_t>), i)); 
   }
}

void free_array(const std::pair<int, size_t> *a)
{
    delete [] a;
}
int main()
{
    size_t sz = 10;
    std::pair<int, size_t> *a = generate_array(sz);
    fisher_yates_shuffle(a, sz);
    bubble_sort(a, sz, comp);  
    print_array(a, sz);
    std::cout << std::endl;
    free_array(a);
    return 0;
}
