#include <iostream>
#include <stddef.h>
#include <stdint.h>
#include <algorithm>
#include <utility>
#include <forward_list>
#include <stdlib.h>
#include <sys/time.h>
typedef bool (*compare)(const void *a, const void *b);

bool cmp(const void *a, const void *b)
{ 
    return std::less<int>()(*reinterpret_cast<const int*>(a), *reinterpret_cast<const int *>(b));
} 
#define nth_element(s, es, n) (reinterpret_cast<char *>(s) + ((n)-1)*(es)) 
#define nth_element_const(s, es, n) (reinterpret_cast<const char *>(s) + ((n)-1)*(es))

void swap(void *s, size_t es, size_t oa, size_t ob)
{
    if (oa == ob)    return;
    char *pa = nth_element(s, es, oa), *pb = nth_element(s, es, ob); 
    while(es-- != 0) std::swap(*pa++, *pb ++);
} 
size_t partition(void *s, size_t esize, size_t so, size_t eo, compare c)
{

    size_t po = so, scan = so + 1;
    while(scan <= eo)
    {
       if (c(nth_element(s, esize, so), nth_element(s,esize,scan)))
       {
           scan ++;
           continue;
       }
       swap(s, esize, ++po, scan++); 
    }
    swap(s, esize,  so, po); 
    return po;
}
void quick_sort(void * s, size_t esize, size_t size, compare c)
{
    /* offset of the begining and ending elements of the subarrray to process */
    std::forward_list<std::pair<size_t, size_t>> fl;
    fl.push_front(std::make_pair<size_t, size_t>(0, size -1));
    while(!fl.empty())
    {
        auto p = fl.front();
        fl.pop_front();
        if (p.first >= p.second)
            continue;
        /* randomize the pivot elemtn */
       size_t peo = rand()% (p.second - p.first + 1) + p.first;  
       swap (s, esize, p.first,  peo);
       peo =  partition(s, esize, p.first, p.second, c);
       if (peo != p.first)
           fl.push_front(std::make_pair<size_t, size_t>(std::move(p.first), peo - 1));
       fl.push_front(std::make_pair<size_t, size_t>(peo + 1, std::move(p.second))); 

    }
} 
void * generate_elements(size_t s)
{
    std::pair<int, unsigned int> *a;
    a = new std::pair<int, unsigned int>[s]; 
    auto p = a;
    while(s-- != 0)
    {
        p->first = s;
        p++->second = rand() %300; 
    }
    return a; 
}
void fisher_yates_shuffle(void *s, size_t es, size_t sz)
{
    for (size_t e = 1; e < sz; e++)
    {
        size_t t = rand() % (e + 1);
        swap(s, es, t, e);
    } 
}
void release_elements(const void *s)
{
    delete [] static_cast<const std::pair<int, size_t>*>(s);
}

void print_element(const void *s)
{
    const std::pair<int, unsigned int> *p = static_cast<const std::pair<int, unsigned int>*>(s);
    std::cout << "(" << p->first << "," << p->second << ") ";
}

void print_array(const void *s, size_t sz)
{
    while(sz -- != 0)
    {
        print_element(nth_element_const(s, sizeof(std::pair<int, unsigned int>), sz));
    }
    std::cout << std::endl;
}
int main()
{
    size_t sz = 20;
    std::pair<int, unsigned int> *a = static_cast<std::pair<int, unsigned int>*>(generate_elements(sz));
    fisher_yates_shuffle(a, sizeof(std::pair<int, unsigned int>),sz);
    quick_sort(a, sizeof(std::pair<int, unsigned int>), sz, cmp);
    print_array(a, sz);
    release_elements(a); 
    return 0;
}
