#include <include/prim.h>
#include <iostream>
/* scan the vertice for edges with unexplored  */  
bool  prim::scan(size_t v)
{
   size_t  ep;
   if (colors[v] == EXPLORED)
       return false;
   const auto &adj = g->Adj(v);
   for (const auto *eg: adj)
   {
      ep = v == eg->E()? eg->O(): eg->E(); 
      if (colors[ep] == EXPLORED)
          continue;
      /* if newly discovered, will scan it later */
      pq.push(eg); 
      
   }
   colors[v] = EXPLORED;
   return true;
}
/* worker function */
void  prim::do_work(size_t s)
{
   if (colors[s] == EXPLORED)
   {
 //      cout << std::endl <<  "no new mst will be produced from vertex " << s << endl;
       return; 
   }
   if (colors[s] == UNDISCOVERED)
   {
       colors[s] = DISCOVERED;
   }
   /*scanning of the starting vertex */ 
   scan(s);
   /* examine minimal weight edges on the queue until empty */
   while(!pq.empty())
   {
       const edge *eg = pq.top();
       pq.pop();
       size_t ep = colors[eg->O()] == EXPLORED? eg->E(): eg->O(); 
       if (colors[ep] != UNDISCOVERED)
           continue;
       /* accept only edges with undiscovered vertex endpoints, change color of the endpoint */
       colors[ep] = DISCOVERED;
      /* accumulate edge weight */
       weights += eg->W();
       mstree.insert(eg);
     /* scan the new endpoint, different from Kruskal Algorithm, new edegs may be added to the
        queue in scanning */ 
       scan(ep);
   }
}
