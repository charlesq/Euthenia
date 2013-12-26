#ifndef __HEAP_SORT_H__
#define __HEAP_SORT_H__
#include <sort_base.h>

class heap_sort: public sort_base
{
    const char *get_description(void) const
    {
        const char *p = "Heap Sort:\n \
It treats the array as a balanced binary tree and starts from the leaves and up to make sure each  \n\
internal node has value bigger than values of all nodes below it. so as to create a heap. \n \
thereafter, it swaps the heap top node and the heap bottom leaf and conducts a sift-down operation to maintain the heap property on the array excluding the \n \
node being swapped down. this continues until the heap is reduced to one node, hence the array is sorted.\n time complexity is nlongn and space complexity is o(1)\n";

       return p;
    };
    void operator ()(void *base, size_t nelems, size_t esize, compare c)
    {
        e_start = base;
        num_elems = nelems;
        ele_size = esize;
        cmp = c;
        heapify();
        for (size_t i = num_elems -1; i != 0;  i --)
        {
            eswap(get_element(e_start, ele_size, 0), get_element(e_start, ele_size,i), ele_size);
            sift_down(0, i);
        }
    };
    bool is_comparison_based(void) const final { return true; };
    void heapify(void)
    {
        for (size_t i = num_elems / 2 + 1; i < num_elems; i --)
        {
                sift_down(i, num_elems);
        }
    };
    void sift_down(size_t i, size_t hsize)
    {
        if (2 * i + 1 >= hsize)
            return;
        if (2 * i + 2 >= hsize)
        {
            void *e1 = get_element(e_start, ele_size, 2 * i + 1);
            void *e = get_element(e_start, ele_size, i);
            if (cmp(e1, e) <= 0)
                    return;
            eswap(e1, e, ele_size);
            sift_down(2 * i + 1, hsize);
            return;
        }
       void *e1 = get_element(e_start, ele_size, 2 * i + 1);
       void *e2 = get_element(e_start, ele_size, 2 * i + 2);
       void *e = get_element(e_start, ele_size, i);
       if (cmp(e1, e) <= 0 && cmp(e2, e) <= 0)
           return;
       if (cmp(e1, e2) <= 0)
       {
           eswap(e2, e, ele_size);
           sift_down(2*i + 2, hsize);
       }
       else
       {
           eswap(e1, e, ele_size);
           sift_down(2 * i + 1, hsize);
       }
    };
};
#endif
