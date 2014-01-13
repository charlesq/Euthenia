#ifndef __BUBBLE_SORT_H__
#define __BUBBLE_SORT_H__
#include <base_sort.hpp>
/*********************Bubble Sorting Algorithm ****************************/
class bubble_sort: public base_sort
{
public:
    bubble_sort(int *ar, size_t s): base_sort(ar, s) {};
    void operator() (void)
    {
        for(size_t l = sz -1; l != 0; --l)
            for (size_t i = 0; i < l; ++i)
                if (a[i] > a[i+1])
                    std::swap(a[i], a[i+1]);
    };
};
#endif

