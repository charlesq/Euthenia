#ifndef __BFS_H__
#define __BFS_H__
#include <stdint.h>
#include <forward_list>
#include <include/search.h>
using namespace std;
class BFS: public search
{
    forward_list<size_t> l; /*list of vertices to be scanned */ 
public:
    BFS(const graph *g): search(g) {};
protected:
    virtual void traverse(size_t s);
};
#endif
