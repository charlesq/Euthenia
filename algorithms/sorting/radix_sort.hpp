#ifndef __RADIX_SORT_H__
#define __RADIX_SORT_H__
#include <base_sort.hpp>
/********************** Radix Sort Algorithm *************************/
class radix_sort: public base_sort
{
    int *b;
    size_t  *acc;
    #define BASE_16 16
    #define BASE_10 10
    int base;
public:
    radix_sort(int *ar, size_t s): base_sort(ar, s)
    {
        base = BASE_10;
        acc = new size_t [base];
        b = new int [sz];
    };
    void operator() (void)
    {
        int denom = 1;
        bool halt = false;
        size_t  zero = 0;
        while(true)
        {
            halt = true;
            std::fill(acc, acc + base, zero);
            /* apply counting sort on each radix */
            /* counting */
            for (size_t i = 0; i < sz; ++i)
            {
                size_t j = a[i] / denom % base;
                if (halt && j != 0)
                    halt = false;
                ++acc[j];
            }
            /* if other than on zero bit, no bits recorded non-zero , break out*/
            if (halt)
                break;
            /* accumulating */
            for (size_t i = 1; i < base; ++i)
                acc[i] += acc[i-1];
            for (size_t i = sz - 1; i != -1; --i)
            {
                b[--acc[a[i] / denom % base]] = a[i];
            }
            std::copy(b, b + sz, a);
            denom *= base;
        }
        /* unreached area */
    };
    ~radix_sort()
    {
        delete [] b;
        delete [] acc;
    };
};

#endif
