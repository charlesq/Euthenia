#ifndef __HEAP_SORT_H__
#define __HEAP_SORT_H__
#include <base_sort.hpp>
/********************Heap Sort Algorithm **************************/
class heap_sort:public base_sort
{
    /* this is a recursive procedre */
    void sift_down(size_t i, size_t len)
    {
        size_t l = i * 2 + 1, r = l + 1;
        /* terminating condition 1*/
        if(l >= len)
           return;
        /* terminting condition 2*/
        if (r >= len)
        {
            if (a[i] < a[l])
                std::swap(a[i], a[l]);
            return;
        }
        l = a[l] >= a[r]? l: r;
        /* terminating condition 3 */
        if (a[i] >= a[l])
           return;
       std::swap(a[l], a[i]);
        sift_down(l, len);
    };
    void heapify(void)
    {
        for (size_t i = sz/2; i != -1; --i)
        {
            sift_down(i, sz);
        }
    };
public:
     heap_sort(int *ar, size_t s ):base_sort(ar, s) {};
     void operator() (void)
     {
         heapify();
         for (size_t i = sz -1; i != 0; i --)
         {
            std::swap(a[0], a[i]);
             sift_down(0, i);
         }

    };
};
#endif
