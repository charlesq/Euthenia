#include <sort_base.h>
#include <ctime>
#include <sys/time.h>
#include <functional>
#include <cstdlib>
#include <iostream>
#include <quick_sort.hpp>
#include <merge_sort.hpp>
#include <heap_sort.hpp>
#include <counting_sort.hpp>
#include <radix_sort.hpp>

struct algorithm_map
{
   string name;
   sort_base  *algo; 
};
static const struct algorithm_map map[] = 
{
    // comparison-based sorting algorithms 
    { string("quick sort"), new quick_sort},
    { string("merge sort"), new merge_sort},
    { string("heap sort"), new heap_sort},
    // non-comparison-based sorting algorithms
    { string("radix sort"),new radix_sort},
    { string("counting sort"), new counting_sort},
     // this item must be kept as the last entry
    { string(""), NULL},
};
void sort_context::select_algorithm(string name)
{
    algorithm_name = name;
    algorithm = NULL;
    runtime = 0;
    for (auto &x: map)
    {
        if (x.name.compare(name) == 0)
        {
            algorithm = x.algo;
            break;
        }
    }
 }
void sort_context:: sort(void *base, size_t nelement, size_t size, compare  c)
{
    struct timeval ts, te;
    std::cout << "\n\n********" << algorithm_name.c_str() <<  "***********\narray size is " << nelement << std::endl;
    if (verbose)
        std::cout << "element sequence before sorting\n";
    print_elements(base, nelement, size);
    cout << std::endl << "start sorting \n.....\n";
    gettimeofday(&ts, NULL);
    if (algorithm->is_comparison_based())
    {
        if (c == NULL)
        {
            std::cout << "severe error: compare function shall be specified " << std::endl;
            return;
        }
        (*algorithm)(base, nelement, size, c);
    }
    else
    {
        (*algorithm)(base, nelement, size);
    }
    gettimeofday(&te, NULL);
    runtime = te.tv_sec * 1000000 + te.tv_usec - ts.tv_sec * 1000000 - ts.tv_usec;
    std::cout << "soring finished, runtime is " << runtime << " us" <<  std::endl;
    if (verbose)
        std::cout << "element sequence after sorting\n";
    print_elements(base, nelement, size);
    if (algorithm->is_sorted())
        std::cout << "\nThe resulting array is sorted !";
     else
        std::cout <<"\nThe resulted array is not sorted !!";
    std::cout << std::endl << "***********************************\n";
};
void sort_context::fisher_yates_shuffle(void *base, size_t count, size_t size)
{
    srand(time(NULL));
    for (size_t i = 1; i < count; i ++)
    {
        unsigned int  pos = rand() % (i+1);
        if(pos != i)
        {
            for (unsigned int j = 0; j < size; j ++)
                std::swap(static_cast<char*>(base)[i * size + j], static_cast<char *>(base) [pos * size + j]);
        }
    }
}
void sort_context::int_test(size_t sample_set_size)
{
    unsigned int *a = new unsigned int[sample_set_size];
    srand(time(NULL));
    for(size_t i = 0; i < sample_set_size; i ++)
    {
        a[i] = (algorithm_name.compare("counting sort") == 0)?rand()%sample_set_size: rand();
        
    }
    fisher_yates_shuffle(a, sample_set_size, 4);
    if (algorithm->is_comparison_based())
        sort(a, sample_set_size, 4, int_compare);
    else
        sort(a, sample_set_size, 4, NULL);
    delete [] a;
}

static void map_destructor(void) __attribute__((destructor));
static void map_destructor(void)
{
    for (auto &item: map)
        delete item.algo;
}
