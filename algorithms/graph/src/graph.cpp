#include <fstream>
#include <include/graph.h>


using namespace std;
/* construct a graph from a text file 

  The file format  must follow the example below and the comments are for explanation only and 
  and should be omitted in the actual file content.
   
   5  #total number of vertices and all vertices are denoted as an ordinal number in [0, 5)
   4  #total number of edges in the graph; for undirected graph, an edge shall appear only once
   0 1 3   #0 and 1 are the vertex endpoint of an edge, for directed graph, 0 is the starting endpoint
            while 1 is the ending endpoint. 3 is the weight of the edge
   1 2 5
   1 3 6
   4 1 5 
   
*/
graph::graph(string fn, bool directed)
{
   fstream is(fn); 
   if (!is.is_open())
   {
       v = 0;
       return;
   }
   /* obtain the total number of vertices */
   is >> v;
   adj.resize(v);
   /* obtain the total number of edges */
   is >> e;
   size_t n = e; 
   size_t ep1, ep2;
   int wt;
   while(e-- != 0)
   {
      is >> ep1;
      is >> ep2;
      is >> wt; 
      edge * eg = new edge (ep1, ep2, wt);
      edges.push_front(eg);
      adj[ep1].push_front(eg);
      if (!directed)
          adj[ep2].push_front(eg);
   }
   
}
/* constructed a graph from an existing directed graph object, and 
   by setting transpose, the new graph shall be tranposed from
   the given graph*/
graph::graph(const graph *g, bool transpose)
{
   v = g->V();  
   e = g->E();
   adj.resize(v);
   edge *aedge;
   for (const edge * ge: g->Edges())
   {
      aedge = new edge(ge->O(), ge->E(), ge->W());
      edges.push_front(aedge);
      adj[aedge->E()].push_front(aedge);
   }
}


