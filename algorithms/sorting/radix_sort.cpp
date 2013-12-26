#include <iostream>
#include <algorithm>



void radix_sort(unsigned int *a, size_t sz)
{
#define BASE 10
     unsigned int *b = new unsigned int [sz];
     unsigned int count [BASE];  
     unsigned int divider = 1;
     bool halt = false;
     while(true)
     {
         halt = true;
         unsigned int vz = 0;
         std::fill(count ,count + BASE, vz);
         /* counting sort */
         for (size_t i = 0; i < sz; i ++)
         {
             ++count[*(a + i)/divider % BASE];
             if (halt && *(a+i)/divider % BASE != 0)
                 halt = false;
         }
         if (halt)
             break;
         for (size_t i = 1; i < BASE; i ++)
             count[i] += count[i-1];
         for (size_t i = sz - 1; i != -1; i --)
         {
             b[--count[a[i] / divider % BASE]] = a[i]; 
         }
         std::copy(b, b + sz, a); 
         /* check the next bit */
         divider *= BASE;         
     }
    


    delete [] b;
}




unsigned int * generate_array(size_t sz)
{
    unsigned int *a = new unsigned int [sz], *ret = a;
    while(sz -- != 0)
        *a++ = rand() % 70000;
    return ret;
}



void release_array(const unsigned int *a)
{
    delete [] a;
}

void print_array(const unsigned int *a, size_t sz)
{
    while(sz -- != 0)
      std::cout << *a ++ << "  ";
    std::cout << std::endl;
}


int main()
{
    unsigned int sz = 20;
    unsigned int *a = generate_array(sz);
    print_array(a, sz);
    radix_sort(a, sz);
    print_array(a, sz);
    release_array(a);
    return 0;
}
