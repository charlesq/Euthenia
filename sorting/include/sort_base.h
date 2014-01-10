#ifndef __SORT_H__
#define __SORT_H__
#include <cstddef>
#include <compare.h>
#include <string>
#include <iostream>
using namespace std;
class sort_context;
static int int_compare(const void*, const void*);
static void print_int_element(const void *e)
{
    std::cout<< *reinterpret_cast<const unsigned int *>(e) << " ";
}
class sort_base
{
protected:
    void *e_start;
    size_t num_elems;
    size_t ele_size;
    compare cmp;
public:
    virtual const char * get_description(void) const = 0;
    /* for comparison-based sorting altorithms */
    virtual void operator()(void *base, size_t count, size_t size, compare  c) {};
    /* for non-comparison-based sorting algorithms */
    virtual void operator()(void *base, size_t count, size_t size) {};
    virtual bool is_comparison_based(void) const = 0;
    compare get_compare(void) const { return cmp; };
public:
    void eswap(void *a, void *b, size_t esize)
    {
        if (a != b)
        {
            char *_a = static_cast<char *>(a), *_b = static_cast<char*>(b);
            unsigned int i = 0;
            while(i ++ < esize)
                std::swap(*_a++, *_b++);
        }
    };
    const void *get_element(const void *start, size_t size,  size_t nth)const
    {
        return static_cast<const void *>(static_cast<const char *>(start) + nth *size);
    };
    void *get_element(void *start, size_t size, size_t nth)
    {
       return static_cast<void*>(static_cast<char *>(start) + nth * size);
    };
    bool is_sorted(void) const
    {
        compare c = get_compare();
        if (c == NULL)
            c = int_compare;
        for (unsigned int i = 1 ; i < num_elems; i ++)
        {
            if (c(get_element(e_start, ele_size, i), get_element(e_start, ele_size, i-1)) >= 0 )
                continue;
            return false;
        }
        return true;
    };
};
class sort_context
{
    std::string algorithm_name;
    bool recursion;
    compare cmp;
    unsigned int runtime;
    sort_base *algorithm;
    bool verbose;
    print_element printe;
public:
    sort_context(void): cmp(NULL),recursion(false), runtime(0), algorithm(NULL), verbose(false), printe(NULL) {};
    void select_algorithm(string name);
    const sort_base *get_algorithm(void)const { return algorithm; };
    void  sort(void *base, size_t nelement, size_t size, compare  c);
    void  sort(void *base, size_t nelement, size_t size);
    unsigned int get_runtime(void) const  { return runtime ;};
    const char *get_description(void) const  {return algorithm->get_description();};
    const string& get_name(void){ return algorithm_name;};
    void set_verbose(bool v, print_element p){verbose = v; printe = p;};
    void print_elements(const void *base, size_t cnt, size_t esize) const
    {
        if (verbose)
        {
            const print_element p = (printe == NULL)? print_int_element: printe;
            for( size_t i = 0; i < cnt; i ++)
                p(algorithm->get_element(base, esize, i));
        }
    }; 
private:
    void fisher_yates_shuffle(void *base, size_t count, size_t size);
public:
    void int_test(size_t sample_set_size);
};

static int int_compare(const void *a, const void *b)
{
    const int &aa = *static_cast<const int *>(a), &bb = *static_cast<const int*>(b);
    return (aa == bb)? 0: ((aa < bb)? -1: 1);
}
#endif
