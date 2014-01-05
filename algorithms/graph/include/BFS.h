#ifndef __BFS_H__
#define __BFS_H__
#include <stdint.h>
#include <include/search.h>
#include <deque>
#include <queue>
class BFS: public search
{
    std::queue<size_t, std::deque<size_t>> q; /*list of vertices to be scanned */ 
public:
    BFS(const graph *g): search(g) {};
protected:
   /* worker bee function */
    virtual void traverse(size_t s);
};
#endif
