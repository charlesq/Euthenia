#ifndef __BASE_SORT_H__
#define __BASE_SORT_H__

 /* base sort class, all sorting algorithm classes will derive from it */
class base_sort
{
protected:
    int *a;/* array is allocated from sorting class, do not deallocate */
    size_t sz;
    base_sort(int *ar , size_t s):a(ar), sz(s) {};
public:
   /* abstract functor method */
    virtual void operator()(void) = 0;
};
#endif
