#ifndef __DFS_H__
#define __DFS_H__
#include <include/search.h>
class DFS: public search
{
public:
   DFS(const graph *g): search(g) {};
   virtual void traverse(size_t s);
};


#endif
