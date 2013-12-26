#ifndef __COUNTING_SORT_H__
#define __COUNTING_SORT_H__
#include <sort_base.h>
#include <vector>
#include <iostream>
class counting_sort: public sort_base
{
    std::vector<unsigned long long> aux;
    void * array_copy;
    const char *get_description(void) const
    {
        const char * desc = "Counting sort:\n\
a non-comparison sorting algorithm with o(N) time complexity and o(N) space complexity.\n\
counting sort is suitable forarray elements with discrete/enumerable values and in a limited  range.\n\
By itself, counting sort does not have wide application,\
it usually is applied in junction with other sorting method such as radix sorting. \n ";
        return desc;
    };
    bool is_comparison_based(void) const final  { return false; };
    void operator ()(void *base, size_t cnt, size_t size)
    {
        e_start = base;
        num_elems = cnt;
        ele_size = size;
        array_copy = static_cast<void *>(new char [cnt * size]); 
        std::copy(static_cast<char*>(e_start), static_cast<char*>(e_start) + num_elems * ele_size, static_cast<char *>( array_copy)); 
        count();
        produce_results();
        aux.clear();
        delete static_cast<char *>(array_copy);
    };
    void count(void)
    {
        aux.resize(10, 0);
        for (unsigned int i = 0; i < num_elems; i++)
        {
            unsigned long long e = get_element_value(get_element(e_start, ele_size, i));
            if (aux.size() <= e)
            {
                aux.resize(e+1, 0);
            }
      
            ++aux[e];
        }
        /* make the aux counting cumulative */ 
        for (unsigned int j = 1; j < aux.size(); j++)
            aux[j] += aux[j-1];
    };
    void set_element_value(size_t n, unsigned long long v)
    {
        unsigned char *e = reinterpret_cast<unsigned char *> (get_element(e_start, ele_size, n));
        unsigned char *p = reinterpret_cast<unsigned char *>(&v);
        std::copy(p, p + ele_size, e);
    };
    unsigned long long get_element_value(const void *e) const
    {
        unsigned long long v = 0;
        const unsigned char *eb = reinterpret_cast<const unsigned char *>(e);
        const unsigned char *ed = eb + ele_size; 
        unsigned int shift = 0;
        while(eb != ed)
        {
            v += (*eb++) << shift;
            shift += 8; 
        }
        return v;
    };
    void produce_results(void)
    {
        for (size_t  i = num_elems -1; i < num_elems; i --)
        {
              unsigned long long e = get_element_value(get_element(array_copy, ele_size, i));
              set_element_value(--aux[e], e); 
        }
    }; 
};
#endif

