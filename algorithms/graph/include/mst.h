#ifndef __MST_H__
#define __MST_H__
#include <include/graph.h>
#include <include/edge.h>
#include <unordered_set>
#include <include/compare.h>
#include <queue>
/* abstract mst class defins common fields and functions for Prim and Kruskal algorithms.
 * In this way, the respective concrete class can focus algorithm implementation; 
 */ 
class mst
{

protected:
   const graph *g; 
   std::priority_queue<const edge *, std::vector<const edge *>, compare > pq; /* the heap holds candiate edges of mst */
   std::unordered_set<const edge *> mstree;
   mst(const graph *gh):g(gh) {};
   virtual void  do_work(size_t s)= 0;
public:
   virtual const std::unordered_set<const edge*>  operator () (size_t s = 0) final
   {
       mstree.clear();
       do_work(s);
       return mstree;
   
   };
   virtual ~mst(void)
   {
       mstree.clear();
   };
};


#endif
