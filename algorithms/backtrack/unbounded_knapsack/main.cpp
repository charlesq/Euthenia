/*unbounded knapsack problem */
#include <stdint.h>
#include <unordered_map>
#include <iostream>
#include <vector>

/*bt root*/
unsigned long  unbounded_knapsack_bt(std::unordered_map<unsigned int, unsigned int> &item, std::unordered_map<unsigned int, unsigned int>::const_iterator &it, unsigned int W)
{
     
     if (it == item.cend() || W == 0)
         return 0;
      
     unsigned long value, max = 0;
     for (size_t i = 0; it->second * i <= W; i ++)
     {
          /* it->first * i is the first candidate extension */
          /* ++itn is the second candidate extension */
          auto itn = it;
          value = it->first * i + unbounded_knapsack_bt(item, ++itn , W - it->second * i);  
          /* accept */
          if (value > max)
              max = value;
         /* else reject */
     } 
     /* output */
     return max;   
} 

int main()
{
     std::unordered_map<unsigned int, unsigned int>  m = {{4, 5}, { 1, 20}, {6, 2}};
     auto it = m.cbegin();
     std::cout << "max value is " <<  unbounded_knapsack_bt(m, it, 5);
    
     return 0;
}
