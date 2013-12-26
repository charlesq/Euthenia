#include <memory>
#include <cstring>
#include <algorithm>
#include <stddef.h>
#include <iostream>
std::unique_ptr<int []> compute_prefix_failure_func(const char *pattern)
{
    int *kmp = new int[strlen(pattern)];
    if (kmp == NULL)
        return std::unique_ptr<int[]>();
    int v = 0;
    std::fill(kmp, kmp + strlen(pattern), v);
    kmp[0] = -1; 
    size_t i = 0, j = 2; 
    while(*(pattern + j) != '\0')
    {
        if (pattern[j -1] == pattern[i])
        {
            i++;
            kmp[j] = i;
            j++;  
             
            continue;
        }
        if (i > 0)
            i = kmp[i];
         else
         {
             kmp[j++] = 0; 
         }
    }
   return std::unique_ptr<int[]>(kmp);
}

const char * string_match(const char *str, const char *p)
{
    if (str == NULL || p == NULL)
       return NULL;
    std::unique_ptr<int[]> pft = compute_prefix_failure_func(p); 
    int i = 0, j = 0;
    while(str[i] != '\0')
    {
        if (str[i] == p[j])
        {
            if (p[j + 1] == '\0')
                return str + i - strlen(p) +1;
            i++;
            j++;
            continue;
        }
        i += j- pft[j]; 
        if (pft[j] >= 0)
        {
            j = pft[j];
        }
        else
            j = 0;
    }
    return NULL;
}


int main()
{
   while(true)
   {
      char str[100];
      char pattern[100];
      std::cout << "Input a string: ";
      std::cin >> str;
      std::cout << std::endl<< "specify a pattern: ";
      std::cin >> pattern;
      std::cout << string_match(str, pattern) << std::endl;
      std::cout << "Halt(Y/N)?";
      std::cin >> str[0];
      std::cout << std::endl;
      if (str[0] == 'Y' || str[0] == 'y')
          break; 
   
   }    
   return 0;
}
