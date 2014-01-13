#ifndef __SORTING_H__
#define __SORTING_H__
#include <stdint.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <quick_sort.hpp>

#include <radix_sort.hpp>
#include <merge_sort.hpp>
#include <bubble_sort.hpp>
#include <heap_sort.hpp>
#include <insert_sort.hpp>

/* Entrance class for sorting algorith testing, kind of a facade pattern example
   in this program, elements are ordered by their natural order
 */
class sorting
{
   int *a;/* array to be sorted, allocated on the heap */
   size_t sz;/* number of elements  */
   /* utility function, randomize elements order */
   void fisher_yates_shuffle()
   {
       for (size_t i = 1; i < sz; i ++)
       {
          int j = rand() % (i + 1);
          std::swap(a[i], a[j]);
       }
   };
   /* utility function, print elements in the array */
   void print_array(void) const
   {
       for (size_t i = 0; i < sz; ++i)
           std::cout << std::setfill(' ') <<  std::setw(3) << a[i];
        std::cout << std::endl;
   };
   /* adopt xUnit methodology and  provide two fixture procedures, before and after respectively */
   void before(std::string tn)
   {
       std::cout << "***************" << tn << "************" << std::endl;
       fisher_yates_shuffle();
       std::cout << "Prior to sorting: " << std::endl;
       print_array();
   };
   void after(void) const
   {
      std::cout  << "Post sorting: " << std::endl;
      print_array();
      if (is_sorted())
         std::cout << "All elements are sorted in non-descending order";
      else
          std::cout << "failed to sort elements in non-descending order";
      std::cout << std::endl;
   };
   void generate_elements(size_t size)
   {
      a = new int[size];
      for (size_t i = 0; i < size; i ++)
          a[i] = i;
   };
   /* verify whether array is sorted */
   bool is_sorted(void) const
   {
       for (size_t i = 1; i < sz; ++i)
           if (a[i] < a[i-1])
               return false;
       return true;
   };
/*****************************************************************************************/
public:
   sorting(size_t len)
   {
      sz = len;
      generate_elements(sz);
   };
/* test methods for respective implemented  sorting algorithms*/
   void runQuicksort(void)
   {
      before("Quick sorting");
      quick_sort qs(this->a, this->sz);
      qs();
      after();
   };
   void runHeapsort(void)
   {
      before("Heap sorting");
      heap_sort hs(this->a, this->sz);
      hs();
      after();
   };
   void runBubblesort(void)
   {
      before("Bubble sorting" );
      bubble_sort bs(this->a, this->sz);
      bs();
      after();
   };
   void runRadixsort(void)
   {
      before("Radix sorting");
      radix_sort rs(this->a, this->sz);
      rs();
      after();
   };
   void runMergesort(void)
   {
     before("Merge sorting");
     merge_sort ms(this->a, this->sz);
     ms();
     after();
   };
   void runInsertsort(void)
   {
       before("Insert sorting");
       insert_sort is(this->a, this->sz);
       is();
       after();
   };
   virtual ~sorting()
   {
      delete [] a;
   };
private:
  /* make base_sortt a friend class, so it can access the elements information */
  friend class base_sort;

};
#endif
