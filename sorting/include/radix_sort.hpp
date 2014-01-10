#ifndef __RADIX_SORT_H__
#define __RADIX_SORT_H__
#include <sort_base.h>
#include <vector>
#include <functional>
class radix_sort: public sort_base
{
    size_t radix_pos;
    size_t base;
    std::vector<unsigned char> count;
    void *aux;
    bool halt;
public:
    radix_sort(void):radix_pos(0), base(10), aux(NULL), halt(false) {};
    const char *get_description(void) const
    {
        const char *desc = "Radix sort:\n\
A non-comparison-based sorting algorithms, which sort an array by iteratively sorting values of radix bits in elements.\n\
while doing this, the common practice is either begin with the most significant bit or the least significant bit and contiues with adjacent bit untill the array is sorted. \n\
a variety of algorithms maybe used to radix bit sorting, in this example, counting sort is selected.\n\
runtime complexity: O(base*n), space complexity: O(n) "; 
        return desc; 
    };
    bool is_comparison_based(void) const final {return false; };
    void operator ()(void *b, size_t cnt, size_t size)
    {
        e_start = b;
        num_elems = cnt;
        ele_size = size;
        count.resize(base, 0);
        aux = static_cast<void*>(new unsigned char [size * cnt]);
        sort();
        delete [] static_cast<unsigned char *>(aux);
    }; 
    unsigned long long get_element_value(const void *e)
    {
        unsigned long long v = 0;
        size_t shift = 0;
        const unsigned char *eb = static_cast<const unsigned char*>(e), *ed = static_cast<const unsigned char *>(e) + ele_size; 
        while(eb != ed)
        {
             v += ((*eb++) << shift);
             shift += 8;
        }
        return v;
    };
    void sort(void)
    {
        unsigned long long value, div =1, q;
        while(!halt)
        {
            for (unsigned char &item: count)
                item = 0;
            halt = true;
            div = base << radix_pos ++;
            for(size_t i = 0; i < num_elems; i ++)
            {
                void * e = get_element(e_start, ele_size, i);  
                value = get_element_value(e);
                if ((q = value/div) > 0)
                    halt = false; 
                count[q%base] ++; 
            }
            if (halt)
                continue;
            counting_sort(div);
        }
    };
    void counting_sort(unsigned long long div)
    {
        //make counting cumulative 
        for (unsigned char i = 1; i < base; i++)
            count[i] += count[i-1];
        for (size_t i = num_elems -1; i < num_elems; i --)
        {
            unsigned long long v = get_element_value(get_element(e_start, ele_size, i));
            set_element_value(aux, --count[v/div%base], v);
        }
        std::copy(static_cast<unsigned char*>(aux), static_cast<unsigned char *>(aux) + num_elems * ele_size, static_cast<unsigned char *>(e_start));
    };
    void set_element_value(void *b, size_t n, unsigned long long v)
    {
        unsigned char *e = static_cast<unsigned char *>(get_element(b, ele_size, n));
        unsigned char *p = reinterpret_cast<unsigned char *>(&v);
        std::copy(p, p + ele_size, e);
    };
     
};

#endif

