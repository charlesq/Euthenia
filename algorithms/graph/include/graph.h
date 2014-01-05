#ifndef __GRAPH_H__
#define __GRAPH_H__
/*  graph class is defined to represent a graph, directed or undirected.
    Adjacency list is employed to describe neighbors and linke of a vertex,
    which is efficient in computational complexity for most graph algorithms.
    
    A graph may be constructed in seveal ways.  It may be read from a graph 
    description file configured in a fixed format. or it may be constructed
    from an existing graph object.
    
    When constructed from a text file, a user may specify whether the graph is
    directed or not in the constructor.

    when constructed from an existing graph object, a user may specify whether
    the constructed graph to be transposed from the given one.
*/ 
    

#include <forward_list>
#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <include/edge.h>
class graph
{
    size_t v; /* designate the totoal number of vertices in the graph */ 
    size_t e; /* total number of edges in the graph */ 
    std::vector<std::forward_list<const edge*>> adj; /*vector of adjancy lists, to access 
                                                adjacency list for a specific vertext v,  
                                                use adj[v] */
    std::forward_list<const edge*> edges; /* store all edges in the graph, mainly for purpose of freeing 
                                             memory of all edges in graph destructor */ 
public:
    graph(std::string fn, bool directed = false); 
    graph (const graph *g, bool transposed = false);
    inline size_t V(void) const { return v; };
    inline size_t E(void) const { return e; };
    inline const std::forward_list<const edge *> & Adj (size_t ve ) const throw (int)  { if (ve < v) return adj[ve]; throw -1;};
    inline const std::forward_list <const edge *> & Edges(void) const { return edges; };
    ~graph(void)
    {
       for (const auto eg: edges)
           delete eg; 
       edges.clear();
    };
};

  
#endif
