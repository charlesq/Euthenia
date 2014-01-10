#ifndef __DELAY_H__
#define __DELAY_H__
#include <sys/time.h>
#include <sched.h>
#include <unistd.h>
static void delay(unsigned int ms)
{
   struct timeval tv1, tv2;
   gettimeofday(&tv1, NULL);
   unsigned int delta = 0;
   while(delta <= ms)
   {
       if (ms - delta > 10) sched_yield();
       gettimeofday(&tv2, NULL);
       delta =  tv2.tv_usec / 1000 + tv2.tv_sec * 1000 -  tv1.tv_usec / 1000
                - tv1.tv_sec * 1000;
   }
   return;
}
#endif
