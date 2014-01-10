#ifndef __QSORT_H__
#define __QSORT_H__ 
#include <sort_base.h>
static const char *description =  "Quick sort:\n\
recursively choose a pivot element, partition the set into two subset \
, with one subset containing elements less than or equal to the pivot element and the other subset containing the rest;\n\
the partitionings continues until the either subset has less than one element\n\
Computational complexity: space complexity o(1); time complexity: best/average case, O(nlogn), worst case O(n*n)";
class quick_sort: public sort_base
{
public:
    const char *get_description(void) const {return description; };
    void operator() (void *base, size_t count, size_t size, compare c)
    {
        e_start = base;
        num_elems = count;
        ele_size = size;
        cmp = c;
        sort(base, count);
    };
    bool is_comparison_based(void) const final {return true;};
    void sort(void *start, size_t cnt)
    {
       if (cnt == 0 || cnt > num_elems)
           return;
       size_t pivot = rand() % cnt;
       void * p = get_element(start, ele_size, pivot);
       eswap(start, p, ele_size);
       unsigned int e = partition(start, cnt);
       sort(start, e );
       sort(get_element(start, ele_size, e + 1) ,cnt - e - 1);
        
    };
    unsigned int  partition(void *start, unsigned int cnt)
    {
        int i = 0;
        int j = 0;
        while(++i <= cnt)
        {
            void *p = get_element(start, ele_size, i);
            if (cmp(p, start) >= 0)
                continue;
            eswap(p, get_element(start, ele_size, ++j), ele_size);
        }
        eswap(get_element(start, ele_size, j), start, ele_size);
        return j;
    };
};


#endif
