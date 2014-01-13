#ifndef __MERGE_SORT_H__
#define __MERGE_SORT_H__
#include <base_sort.hpp>
/************************Merge Sort Algorithm ******************/
class merge_sort: public base_sort
{
    int *b;
    /* merge the two subarrays into one sorted aggregate array */
    void merge(size_t start, size_t end)
    {
        size_t m = (start + end)/2, i = start, j = m;
        int *p=b;
        while (i < m && j < end)
            *p ++ = a[i] <= a[j]? a[i++]: a[j++];
        while(i < m)
            *p ++ = a[i++];
        while(j < end)
            *p ++ = a[j++];
        /* copy sorted elements from helper array back to original array */
        while(start < end)
            a[--end] = *--p;

    };
    /*sort elements in [start, end) */
    void sort(size_t start, size_t end)
    {
        /* terminating condition */
        if (end <= start + 1)
            return;
        size_t m = (start + end)/2;
        /* arithmatically divide into two sub arrays without sorting
           and continue to divide each recursively */
        sort(start, m);
        sort(m, end);
        /* conquer procedure */
        merge(start, end);
    };
public:
    merge_sort(int *ar, size_t s): base_sort(ar, s) {};
    void operator() (void)
    {
        b = new int [sz];
        /*sort elements in [start, end) */
        sort(0, sz);
        delete [] b;
  };
};
#endif
