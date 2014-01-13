#ifndef __QUICK_SORT_H__
#define __QUICK_SORT_H__
#include <base_sort.hpp>
/********************** Quick Sort ********************************/
class quick_sort: public base_sort
{
    size_t partition(size_t start, size_t end)
    {
        size_t i = start, j = i;
        while(++j < end)
        {
            if (a[j] < a[start])
                std::swap(a[++i], a[j]);
        }
        std::swap(a[i], a[start]);
        return i;
    };
    /* sort elements in [start,end) */
    void sort(size_t start, size_t end)
    {
       /* recursion terminating condition */
       if (end <= start +1)
           return;
       /* randomly pick a pivot element */
       size_t p = rand() % (end - start) + start;
       std::swap(a[start], a[p]);
       /*divide into two sub arrays, sorting procedure occurs inside */
       p = partition(start, end);
       /* conquer each sub array ? */
       sort(start, p);
       sort(p+1, end);
    };
public:
    quick_sort(int *ar, size_t s):base_sort(ar, s) {};
    void operator () (void)
    {
       sort(0, sz);
    };
};
#endif
