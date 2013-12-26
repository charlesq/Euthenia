#ifndef __MERGE_SORT_H__
#define __MERGE_SORT_H__
#include <sort_base.h>
#include <utility>

static const char *description_merge_sort =  "Merge sort \n\
it divides the array into to two aproximate equivalent-sized subsets recursively until a set is eeither empty or has only one element; \n\
then in reverse, it conducts merging operations on every two subset to product sorted merged set until the whole array is sorted.\n\
Computational complexity: space complexity o(n); time complexity: O(nlogn)\n";
class merge_sort: public sort_base
{
    void *mem;
    void *half;
    const char *get_description(void) const {return description_merge_sort; };
    void operator()(void *base, size_t nelem, size_t elesize, compare c)
    {
        mem = static_cast<void*>(new char[nelem * elesize]);
        half = static_cast<void*>(static_cast<unsigned char*>(mem)+nelem/2 );
        e_start = base;
        num_elems = nelem;
        ele_size = elesize;
        cmp  = c;
        sort(base, num_elems);
       delete [] static_cast<char *>( mem);
    };
    bool is_comparison_based(void) const final { return true; };
    void sort(void *start, size_t cnt)
    {
        if (cnt == 1 || cnt == 0)
            return;
        sort(start, cnt/2);
        void *p = get_element(start, ele_size, cnt/2);
        sort(p, (cnt + 1)/2);
        merge(start, cnt/2, p, (cnt + 1)/2);
    };
    void merge(void *f, size_t sf, void *s, size_t ss)
    {
        void *ff = f, *ssf = s, *bp = mem;
        size_t s1 = 0, s2 = 0;
        while(s1 < sf  && s2 < ss )
        {
            ff = get_element(f, ele_size, s1);
            ssf = get_element(s, ele_size, s2);
            if(cmp(ff, ssf)<= 0)
            {
                std::copy(static_cast<char*>(ff), static_cast<char*>(ff) + ele_size, static_cast<char *>(bp));
                ++s1;
            }
            else
            {
                std::copy(static_cast<char*>(ssf), static_cast<char*>(ssf) + ele_size, static_cast<char *>(bp));
                ++s2;
            }
            bp = static_cast<void*>(static_cast<char *>(bp) + ele_size);
        }
        while(s1 < sf)
        {
             ff = get_element(f, ele_size, s1);
             std::copy(static_cast<char*>(ff), static_cast<char*>(ff) + ele_size, static_cast<char *>(bp));
             ++s1;
             bp = static_cast<void*>(static_cast<char *>(bp) + ele_size);
        }
        while(s2 < ss)
        {
             ssf = get_element(s, ele_size, s2);
             std::copy(static_cast<char*>(ssf), static_cast<char*>(ssf) + ele_size, static_cast<char *>(bp));
             ++s2;
             bp = static_cast<void*>(static_cast<char *>(bp) + ele_size);
        }
        std::copy(static_cast<char*>(mem), static_cast<char*>(mem) + (sf + ss) * ele_size, static_cast<char *>(f));
    }; 
};

#endif
