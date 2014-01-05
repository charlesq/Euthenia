#ifndef __COMPARE_H__
#define __COMPARE_H__
#include <include/edge.h>
class compare
{
public:
    bool operator() (const edge *e1, const edge *e2) const 
    {
        return e1->W() > e2->W();
    };
};


#endif
