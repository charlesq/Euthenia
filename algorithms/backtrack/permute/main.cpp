#include <iostream>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <stdint.h>

void print(const std::string &s)
{
    static unsigned int line = 0;
    if (s.length() == 0)
    {
        line = 0;
        return;
    }
    std::cout << s << " "; 
    if (++line == 10)
    {
        line = 0;
        std::cout << std::endl;
    }
}
void _permute_l(std::vector<unsigned int> v, unsigned int size, std::string str)
{
   /* accept */
   if (str.length() == size)
   {
       print(str);
       return;
   }
   for (unsigned int i = 0; i < v.size(); i ++)
   {
       std::stringstream ss;
       ss << str;
       if (v[i] != 0)
       {
          /* first extension i */
          ss << static_cast<char>(i);
          v[i] --;
          /* next extension --v[i]*/
          _permute_l(v,size, ss.str());
          v[i] ++;
       }
   }
}
void permute_l(char *str)
{
    unsigned int size = strlen(str);
    std::vector<unsigned int> v(128, 0);
    std::string  ss;
    /* count occurences of characters */
    while(*str != '\0')
        v[*str++]++;  
    _permute_l(v,size, ss);
   
}
/* swap-based backtracking */
void _permute(char *str, unsigned int &l)
{
   /* accept */
   if (*str  == '\0')
   {
       print(str  - l);
       return;
   }
   char *p = str;
   while(*p != '\0')
   {
       /* first extension str */
       std::swap(*str, *p );
       /* next extension is str + 1 */
       _permute(str + 1 , l);
       std::swap(*str, *p ++);
   }
}

void permute(char *str)
{
    size_t len = strlen(str);
    _permute(str, len);
}

int main()
{
    char p[] = "hlos";
    std::cout << "Swap-based permutation: \n";
    permute(p);
    std::cout << std::endl << std::endl;
    print("");
    std::cout <<"counting-based permutation:\n";
    permute_l(p);
    std::cout << std::endl;
    return 0;
}
