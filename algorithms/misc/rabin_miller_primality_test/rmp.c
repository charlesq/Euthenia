#include <stdlib.h>
#include <time.h>


int rabin_miller_test(unsigned int k , unsigned long long n)
{
    if (n <= 3)
        return 1;
    if (n%2 == 0) return 0;
    unsigned long long s=0, d =n -1;
    while (d%2 == 0) 
    {
        s++;
        d /= 2; 
    }
    unsigned long long int a, x, ss;
    srand(time(NULL));
    while(k-- != 0)
    {
       a = rand()% (n-1);
       unsigned long long dd =d ;
       x = 1;
       while (dd-- != 0) { x *= a; x = x % n;} 
       if (x == 1 || x == n -1) continue; 
       ss = s;
       while(ss -- != 1)
       {
            x = (x * x )% n;
            if (x == 1) return 0;
            if (x == n -1) break; 
       }
       return 0;
    }
    return 1; 
}
