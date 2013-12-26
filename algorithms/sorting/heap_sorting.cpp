#include <iostream>
#include <utility>
#include <algorithm>
#include <stdint.h>
#include <forward_list>
#include <stdlib.h>
#include <time.h>

typedef bool (*compare)(const void *a, const void *b);

void swap(void *a, void *b, size_t es)
{
    char *ca = static_cast<char *>(a), *cb = static_cast<char *>(b);
    while(es-- != 0)
    {
        std::swap(*ca++, *cb++);
    }
}

#define nth_element(a, es, n) static_cast<void *>(static_cast<char *>(a) + (es) * (n))
#define nth_element_const(a, es, n) static_cast<const void *>(static_cast<const char *>(a) + (es) * (n))

void sift_down(void *a, size_t esz, compare &c,  size_t sz, size_t n)
{
    std::forward_list<size_t> fl;
    fl.push_front(n);    
    while(!fl.empty())
    {
        n = fl.front();
        fl.pop_front();
        if (n * 2 + 1 > sz -1)
            continue;
        if (n * 2 + 2 > sz -1)
        {
            if (c(nth_element_const(a, esz, n), nth_element_const(a, esz, n *2 +1))) 
            {
               swap(nth_element(a, esz, n), nth_element(a, esz, n * 2 + 1), esz); 
               fl.push_front(n * 2 + 1);
            }
            continue;
        }
        size_t max = c(nth_element_const(a, esz, n*2 + 1), nth_element_const(a, esz, n * 2 + 2))? n * 2 + 2: n * 2 + 1;
        if (c(nth_element_const(a, esz, n), nth_element_const(a, esz, max)))
        {
           swap(nth_element(a, esz, n), nth_element(a, esz, max), esz);
           fl.push_front(max);  
        } 
    }
}

void heapify(void *a, size_t esz, compare &c, size_t sz)
{
    size_t n = sz/2;
    while(n != -1)
    {
       sift_down(a, esz, c, sz, n--); 
    } 
}


void heap_sort(void *a, size_t esz, size_t sz, compare c)
{
    heapify(a, esz, c, sz);    
    while(sz--  != 1)
    {
        swap(nth_element(a, esz, sz), nth_element(a, esz, 0), esz);
        sift_down(a, esz, c, sz, 0);
    }
}

bool comp(const void *a, const void *b)
{
    return *static_cast<const int*>(a) < *static_cast<const int *>(b);
}


int *generate_array(size_t sz)
{
   int *a = new int[sz], *ret = a; 
   while(sz -- != 0)
       *a ++ = sz;   
  return ret;
   
}

void free_array(const int *a)
{
    delete [] a;
}

void fisher_yates_shuffle(int *a, size_t sz)
{
    for (size_t i = 1; i < sz; i ++)
    {
        int e = rand() % (i + 1);
        std::swap(*(a + i), *(a + e));
    }     
}

void print_element(const int *a)
{
    std::cout << *a << " ";
}

void print_array(const int *a, size_t sz)
{
    while(sz -- != 0)
        print_element(a ++);
}


int main()
{
   size_t sz = 30;
   int *a = generate_array(sz);
   fisher_yates_shuffle(a, sz);
   print_array(a, sz);
   std::cout << std::endl;
   heap_sort(a, sizeof(int), sz, comp); 
   print_array(a, sz);
   std::cout << std::endl;
   free_array(a);
}
