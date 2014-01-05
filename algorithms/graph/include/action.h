#ifndef __ACTION_H__
#define __ACTION_H__
#include <stdint.h>
/* Abstract Action class is the base class to define actions taken 
   in graph search, e.g. breadth first search and depth first search
   , either upon discovery of a vertex or when all neighbors of a 
  vertex have been discovered.
*/
class action
{
public: 
    virtual void act(size_t v) = 0;
};

#endif
