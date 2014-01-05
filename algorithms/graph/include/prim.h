#ifndef __PRIM_H__
#define __PRIM_H__
/* prim class implements the PRIM minimal spanning tree algorithms
 * it falls into the category of greedy algorithms. 
   
   The algorithm maintains a priority queue, which is indeed the candate 
   edge set to form the minimal spanning tree.  
   
   The priority queue is provisioned with edges connected to the source vertex
   specified with the algorithm is invoked, then run a breadth first search on
   trees formed with edges on the queue.

   Specifically in each iteration of its run, the algorithm pops an edge from 
   the queue, if the edge has one endpoint undiscovered, the edge will be 
   contributed to the minimal spanning tree set, then adjancency edges of the
   undiscovered endpoint will be decided whether to push to the queue. if no
    unddiscovered endponit exists on the edge, it will be abandanced. 

   The invariant in after each iteration is that the edges in the existing minimal
   spanning tree edge set constitutes the minimal spannint tree for the subgraph 
   which has been discovered/explored. 
    
   Prim algorithm itself may only discover one minimal spanning tree originated 
   from the source vertex. However, it is trivial to call it multiple times to 
   produce all mst forests in the graph.

  Time Complexity: if we presumes that binary heap is underlying priority queue, the complexity magnitude is O(Elog(V)). 
    
 */
#include <stdint.h>
#include <include/mst.h>
#include <include/action.h>
#include <vector>

class prim: public mst 
{
   enum {UNDISCOVERED = 0, DISCOVERED, EXPLORED};
   //std::priority_queue<const edge *, std::vector<const edge *>, compare > pq; /* the heap holds candiate edges of mst */
   std::vector<int> colors;
   bool  scan(size_t v);
public:
    prim(const graph *g): mst(g) 
    {
        int c = UNDISCOVERED;
        colors.resize(g->V(), c);
    };
    virtual void  do_work(size_t s);
<<<<<<< HEAD
    virtual ~prim(void)
    {
        colors.clear();
    };
=======
>>>>>>> 2feaa2afd1a103a0fae49d68ff0d83e6f3d37fbb
};

#endif
