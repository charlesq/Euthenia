#include <include/prim.h>
#include <iostream>
using namespace std;
bool  prim::scan(size_t v)
{
   size_t  ep;
   if (colors[v] == EXPLORED)
       return false;
   const auto &adj = g->Adj(v);
   for (const auto *eg: adj)
   {
      ep = v == eg->E()? eg->O(): eg->E(); 
      pq.push(eg); 
      
   }
   colors[v] = EXPLORED;
   return true;
}

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
   scan(s);
   while(!pq.empty())
   {
       const edge *eg = pq.top();
       pq.pop();
       size_t ep = colors[eg->O()] == EXPLORED? eg->E(): eg->O(); 
       if (colors[ep] != UNDISCOVERED)
           continue;
       colors[ep] = DISCOVERED;
       mstree.insert(eg);
       scan(ep);
   }
}
