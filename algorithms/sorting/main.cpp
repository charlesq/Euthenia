#include <iostream>
#include <utility>
#include <memory>
#include <stdint.h>
#include <stddef.h>
#include <algorithm>
#include <queue>
typedef std::pair<int , int> value_type;

class compare
{
public:
    bool operator () (const value_type &a, const value_type &b) const
    {
        return a.first < b.first;    
    };
};

std::pair<int, int> * generate_array(size_t s)
{
    value_type *a = new std::pair<int, int>[s];
    while(s -- != 0)
    {
        a[s] = std::make_pair<int, int> (rand()%300, s);
    } 
    return a;
};
size_t partition(value_type *a, size_t size, const compare &c)
{
   size_t i = 0, j = 0; 
   while(++j < size)
   {
       if (c(a[0], a[j]))
           continue;
       std::swap<value_type>(a[++i], a[j]);
   }
   std::swap<value_type>(a[i], a[0]); 
   return i;
}
void sift_down(value_type *a, size_t s, compare c, size_t i)
{
    size_t k = 2 * i + 1, j = k + 1;
    if (k >= s)
       return;
    if (j >= s)
    {
        if (c(a[i], a[k]))
        {
            std::swap(a[i], a[k]);
            sift_down(a, s, c, k);
        }
        return;
    }
    k = c(a[j], a[k])? k: j; 
    if (c(a[i], a[k]))
    {
        std::swap(a[i], a[k]);
        sift_down(a, s, c, k);
    }
}
 
void heapify(value_type *a, size_t s , compare c)
{
     for (size_t i = s/2; i != -1; i --)
         sift_down(a, s, c, i);
}
void heap_sort(value_type *a, size_t s, compare c)
{
      heapify(a, s, c);// max heap 
      for (size_t i = s -1; i != 0; i --)
      {
          std::swap(a[0], a[i]);
          sift_down(a, i, c, 0);
      }
}
void print_array(const value_type *a, size_t s)
{
    while (s-- != 0)
    {
        std::cout << "(" << (*a++).first << ", "  << (*a).second << ") ";
    } 
}
void quick_sort(value_type *a, size_t size, compare c)
{
    print_array(a, size);
    std::cout << std::endl; 
    if (size <= 1)
        return;
    size_t p = rand() % size;
   std::swap(a[p], a[0]); 
    p = partition(a, size, c);
    quick_sort(a,  p , c);
    quick_sort(a + p + 1, size - p - 1, c);  
}
typedef std::pair<value_type, size_t> value_index;
class compare_h
{
public:
    bool operator () (value_index a, value_index b) const
    {
        return a.first.first > b.first.first;
    };  
};
#include <forward_list>
#include <deque>
void merge(value_type *a, size_t s, size_t m, size_t e, compare_h c)
{
    
    std::forward_list<value_type> fl; 
    std::priority_queue<value_index, std::deque<value_index>, compare_h> pq;
    size_t mm = m + 1, start = s;;
    if (s <= m)
        pq.push(std::make_pair(a[s++], 0));
    if (mm <= e)
        pq.push(std::make_pair(a[mm++], 1));
    value_index i;
    while(!pq.empty())
    {
        i = pq.top();
        pq.pop();
        fl.push_front(i.first);
        if (i.second == 0 && s <= m)
            pq.push(std::make_pair(a[s++], 0));
        else
        {   
            if (mm > e)
                continue;
            pq.push(std::make_pair(a[mm ++], 1));
        }
    }
    while(!fl.empty())
    {
        a[e--] = fl.front();
        fl.pop_front();
    }
}
void merge_sort(value_type *a, size_t s, size_t e, compare_h c)
{
    if (s == e)
       return;
    size_t m = (s + e)/2;
    merge_sort(a, s, m, c); 
    merge_sort(a, m + 1, e, c);
    merge(a,s, m, e, c); 
} 
void bubble_sort(value_type *a, size_t s, compare c)
{
    if ( s <= 1)
       return;
    for (size_t i = 0; i < s -1; i ++)
    {
        if (c(a[i +1], a[i]))
            std::swap(a[i +1], a[i]);
    }
    bubble_sort(a, s-1, c);
} 
int main()
{
   size_t s = 20;
//   print_array(ar, 3);
   value_type *a = generate_array(s);
//   std::cout << a[0].first << a[0].second << a[1].first << a[1].second << a[2].first << a[2].second;
   print_array(a, s);
   std::cout << std::endl;
   //quick_sort(a, s,compare ());
   //heap_sort(a, s, compare());
  //  merge_sort(a, 0, s -1, compare_h());
   bubble_sort(a, s, compare());
   print_array(a, s);
   std::cout << std::endl;
   delete [] a; 
   return 0;
};
