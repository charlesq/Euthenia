#include <stdint.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>
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
    /* base sort class, all sorting algorithm classes will derive from it */
    class base_sort
    {
    protected:
        int *a;/* array is allocated from sorting class, do not deallocate */
        size_t sz;
        base_sort(sorting *s):a(s->a), sz(s->sz) {};
    public:
       /* abstract functor method */
        virtual void operator()(void) = 0;
    };

    /* algorithm classes are defined below */
/********************** Quick Sort ********************************/
    class quick_sort: public base_sort
    {
        size_t partition(size_t start, size_t end)
        {
            size_t i = start, j = i;
            while(++j < end)
            {
                if (a[j] < a[start])
                    std::swap(a[++i], a[j]); 
            }
            std::swap(a[i], a[start]);
            return i;
        };
        /* sort elements in [start,end) */
        void sort(size_t start, size_t end)
        {
           /* recursion terminating condition */
           if (end <= start +1)
               return; 
           /* randomly pick a pivot element */
           size_t p = rand() % (end - start) + start;
           std::swap(a[start], a[p]);
           /*divide into two sub arrays, sorting procedure occurs inside */
           p = partition(start, end);
           /* conquer each sub array ? */
           sort(start, p);
           sort(p+1, end);
        };
    public:
        quick_sort(sorting *st):base_sort(st) {};
        void operator () (void)
        {
           sort(0, sz); 
        };
    }; 
/********************Heap Sort Algorithm **************************/
    class heap_sort:public base_sort
    {
        /* this is a recursive procedre */
        void sift_down(size_t i, size_t len)
        {
            size_t l = i * 2 + 1, r = l + 1; 
            /* terminating condition 1*/
            if(l >= len)
               return; 
            /* terminting condition 2*/
            if (r >= len)
            {
                if (a[i] < a[l])
                    std::swap(a[i], a[l]);
                return;
            }
            l = a[l] >= a[r]? l: r;
            /* terminating condition 3 */
            if (a[i] >= a[l])
                return;
            std::swap(a[l], a[i]);
            sift_down(l, len);
        };
        void heapify(void)
        {
            for (size_t i = sz/2; i != -1; --i)
            {
                sift_down(i, sz); 
            }
        };
    public:
         heap_sort(sorting *st):base_sort(st) {};
         void operator() (void)
         {
             heapify();
             for (size_t i = sz -1; i != 0; i --)
             {
                 std::swap(a[0], a[i]);
                 sift_down(0, i);
             }
         };
    };
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
        merge_sort(sorting *st): base_sort(st) {};
        void operator() (void)
        {
            b = new int [sz];
            /*sort elements in [start, end) */
            sort(0, sz);
            delete [] b;
        };
    };
/*********************Bubble Sorting Algorithm ****************************/
    class bubble_sort: public base_sort
    {
    public:
        bubble_sort(sorting *st): base_sort(st) {};
        void operator() (void)
        {
            for(size_t l = sz -1; l != 0; --l)
                for (size_t i = 0; i < l; ++i)
                    if (a[i] > a[i+1])
                        std::swap(a[i], a[i+1]);
        };
    };
/********************** Radix Sort Algorithm *************************/
    class radix_sort: public base_sort
    {
        int *b;
        size_t  *acc; 
        #define BASE_16 16
        #define BASE_10 10 
        int base;    
    public:
        radix_sort(sorting *st): base_sort(st) 
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
    class insert_sort: public base_sort
    {
    public:
        insert_sort(sorting *st): base_sort(st) {};
        void operator () (void)
        {
            for (size_t i = 1; i < sz; ++i)
                for (size_t j = i; j != 0; --j)
                    if (a[j] < a[j-1])
                        std::swap(a[j], a[j-1]);
        };
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
      quick_sort qs(this);
      qs();
      after();
   };
   void runHeapsort(void)
   {
      before("Heap sorting");
      heap_sort hs(this);
      hs();
      after();
   }; 
   void runBubblesort(void)
   {
      before("Bubble sorting" );
      bubble_sort bs(this);
      bs();
      after();
   };
   void runRadixsort(void)
   {
      before("Radix sorting");
      radix_sort rs(this);
      rs();
      after();
   }; 
   void runMergesort(void)
   {
     before("Merge sorting");
     merge_sort ms(this);
     ms();
     after();
   };
   void runInsertsort(void)
   {
       before("Insert sorting");
       insert_sort is(this);
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


int main()
{
    sorting instance(20);
    instance.runQuicksort();
    instance.runHeapsort();
    instance.runBubblesort();
    instance.runMergesort();
    instance.runRadixsort();
    instance.runInsertsort();
    return 0;
}
