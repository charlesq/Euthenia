#ifndef __DFS_H__
#define __DFS_H__
#include <include/search.h>
/* DFS class implements the Depth First Search algorithm
   it relies on recusion to explore in depth.

   With a simple forward list to queue newly discovered
   vertices, we do in an iterative way. 
 */
class DFS: public search
{
public:
   DFS(const graph *g): search(g) {};
   /* overloaded from the base class */
   virtual void traverse(size_t s);
};


#endif
