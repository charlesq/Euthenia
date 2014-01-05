#ifndef __KRUSKAL_H__
#define __KRUSKAL_H__
/*
   kruskal class implements Kruskal Algorithm to compute minimal spanning tree. 
   It employs two key data structures to achieve it.

   To extract a minimal-weight edge from remaining edge set, it maitains a priority queue.

   To qucikly decide whether the minimal-weight edge should constriute the minimal 
   spanning tree set,  it prebuilds a dijoinsted set for each vertex, and 
   when running the algorith, for each new edge extracted from the priority queue, it
   examines whether the disjointed sets associated to endpoins of the edge belong to 
   the same set. If yes, the edge will be dropped; otherwise, the edge stays and
   contributes to the minimal spanning tree, then the two disjointed sets are jointed.
  
   Unlike Prim Algorithms, at each step, the selected edges does not necessarily form 
   a tree. 
  
   Time Complexity O(ElogE). 

*/
#include <include/mst.h>
#include <include/disjointed_set.h>
#include <vector>
#include <stdint.h>
using namespace std;
class kruskal: public mst
{
    vector<disjointed_set *> v;
public:
    kruskal(const graph *g): mst(g) {};
    virtual void do_work(size_t);
    virtual ~kruskal(void)
    {
        for (auto ds: v)
           delete ds;
        v.clear();
    };
};

#endif
