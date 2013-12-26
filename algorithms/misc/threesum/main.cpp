#include <iostream>
#include <vector>
#include <algorithm>
#include <stddef.h>
#include <set>
#include <utility>
#include <tuple>
void print(const int & a, const int & b, const int & c)
{
    std::cout << "(" << a << "," << b <<" ," << c << ") ";
}  
void print_vector(const std::vector<int> &v)
{
    std::vector<int>::const_iterator it = v.cbegin();
    while(it != v.cend())
        std::cout << *it ++ << " ";
    std::cout << std::endl;
}
void threesum_subset(std::vector<int> &v)
{
   if (v.size() < 3)
      return;
   int two_sum;
   bool halt = false;
   std::sort(v.begin(), v.end(), std::less<int>());
   for (size_t i = 0; i < v.size() -1 && !halt; i++)
   {
       for (size_t j = i +1; j < v.size(); j ++)
       {
          two_sum = v[i] + v[j]; 
          if (two_sum > 0)
          {
              break;
          }
          two_sum = - two_sum;
          std::vector<int>::iterator it = std::lower_bound(v.begin() + j + 1, v.end(), two_sum, std::less<int>()); 
          while(it != v.end() && *it == two_sum)
              print(v[i], v[j], *it ++);
       }
   } 
} 
int main()
{
    int a[] = {1, 2, 3, -1, -2, 8, -6}; 
    int b[] = {1, 2, 3, -6, 9, -10};
    std::vector<int> v;
    v.resize(sizeof(a)/sizeof(int));
    std::copy(a, a + v.size(),v.begin()); 
    print_vector(v);
    std::cout << std::endl;
    threesum_subset(v);
    print_vector(v);
    std::cout << std::endl;
    v.resize(sizeof(b)/sizeof(int));
    std::copy(b, b + v.size(), v.begin());
    print_vector(v);
    std::cout << std::endl;
    threesum_subset(v);
    print_vector(v);
    v.clear();
    return 0;
}
