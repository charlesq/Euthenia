#include <iostream>
#include <stddef.h>
#include <stdint.h>
#include <algorithm>
#include <utility>
#include <forward_list>
#include <stdlib.h>
#include <sys/time.h>
/***************************************************************************
 quick sort with a stdlib qsort api
 void quick_sort(void * s, size_t esize, size_t size, compare c) 

***************************************************************************/
/* compare function prototype */
typedef bool (*compare)(const void *a, const void *b);
/* a customized compare function, only the first component of  the paired element is
   employed in comparison  and ordering
*/
bool cmp(const void *a, const void *b)
{ 
    return std::less<int>()(*reinterpret_cast<const int*>(a), *reinterpret_cast<const int *>(b));
} 
/* macros to access nth elements in the array */
#define nth_element(s, es, n) (reinterpret_cast<char *>(s) + ((n)-1)*(es)) 
#define nth_element_const(s, es, n) (reinterpret_cast<const char *>(s) + ((n)-1)*(es))
/* std::swap based generic elements swap function */
void swap(void *s, size_t es, size_t oa, size_t ob)
{
    if (oa == ob)    return;
    char *pa = nth_element(s, es, oa), *pb = nth_element(s, es, ob); 
    while(es-- != 0) std::swap(*pa++, *pb ++);
} 
/* the quick partition procedure, divide elements in two sets according to the pivot element value 
   return the new pivot element location index 
*/
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
/* the iterative quick sort function, note that it is  not a recursive procedure  */
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
        /*  pick a random pivot element */
       size_t peo = rand()% (p.second - p.first + 1) + p.first;  
       swap (s, esize, p.first,  peo);
       /* a divide and semi sort procedurer */
       peo =  partition(s, esize, p.first, p.second, c);
       if (peo != p.first)
           fl.push_front(std::make_pair<size_t, size_t>(std::move(p.first), peo - 1));
       fl.push_front(std::make_pair<size_t, size_t>(peo + 1, std::move(p.second))); 

    }
} 
/* utility function to generate the array elements, in this example elements are pair-valued */
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
/* utility function to shuffle element order in the array */
void fisher_yates_shuffle(void *s, size_t es, size_t sz)
{
    for (size_t e = 1; e < sz; e++)
    {
        size_t t = rand() % (e + 1);
        swap(s, es, t, e);
    } 
}
/* utility function to release memory allocated from  heap */
void release_elements(const void *s)
{
    delete [] static_cast<const std::pair<int, size_t>*>(s);
}
/* utility function to pretty-print an element */
void print_element(const void *s)
{
    const std::pair<int, unsigned int> *p = static_cast<const std::pair<int, unsigned int>*>(s);
    std::cout << "(" << p->first << "," << p->second << ") ";
}
/* utility function to print elements in the array in reverse order*/
void print_array(const void *s, size_t sz)
{
    size_t i = 0;
    while(i < sz)
    {
        print_element(nth_element_const(s, sizeof(std::pair<int, unsigned int>), i));
        if (++i % 5 == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;
}
/* test driver function */
int main()
{
    size_t sz = 20;
    /* elements are pair valued, the first component in an element  will be used as key for ordering */
    std::pair<int, unsigned int> *a = static_cast<std::pair<int, unsigned int>*>(generate_elements(sz));
    fisher_yates_shuffle(a, sizeof(std::pair<int, unsigned int>),sz);
    quick_sort(a, sizeof(std::pair<int, unsigned int>), sz, cmp);
    /* print sorted array in reverse order, i.e. non-ascending order */
    print_array(a, sz);
    release_elements(a); 
    return 0;
}
