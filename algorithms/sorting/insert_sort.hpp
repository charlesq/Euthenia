#ifndef __INSERT_SORT_H__
#define __INSERT_SORT_H__
#include <base_sort.hpp>
class insert_sort: public base_sort
{
public:
    insert_sort(int *ar, size_t s): base_sort(ar, s) {};
    void operator () (void)
    {
        for (size_t i = 1; i < sz; ++i)
            for (size_t j = i; j != 0; --j)
                if (a[j] < a[j-1])
                    std::swap(a[j], a[j-1]);
    };
};


#endif
