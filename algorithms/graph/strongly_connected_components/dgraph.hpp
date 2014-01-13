#ifndef __DGRAPH_H__
#define __DGRAPH_H__
#include <iostream>
#include <fstream>
#include <forward_list>
#include <cstdint>
#include <vector>
#include <cstring>
#include <stack>
#include <queue>
#include <deque>
#include <edge.hpp>
using namespace std;
class dgraph
{
    unsigned int V;
    vector<forward_list<const edge *>> Adj; 
    forward_list<const edge *> Edges;
public:
    dgraph(string fn, bool transpose = false)
    {
        V = 0;
        ifstream is(fn); 
        if (is.bad() || !is.is_open()) return;
        is >> V; 
        Adj.resize(V);
        unsigned int e;
        is >> e;
        unsigned int f, t;
        int w;
        while (e -- != 0)
        {
           is >> f >> t >> w; 
           edge *l;
           if (transpose)
               l = new edge(t, f, w);
           else
               l = new edge (f, t, w); 
           Adj[l->f()].push_front(l);
           Edges.push_front(l);
        }
    };
    dgraph(const dgraph &g, bool transpose = false)
    {
        V = g.v(); 
        Adj.resize(V);
        unsigned int f, t;
        int w;
        edge *l;
        for (auto e: g.edges())
        {
            f = e->f(); 
            t = e->t();
            w = e->w();
            if (transpose)
               l = new edge(t, f, w);  
            else
               l = new edge (f, t, w);
            Adj[l->f()].push_front(l);
            Edges.push_front(l);
        }
    };
    const forward_list<const edge *> & adj(unsigned int v) const { return Adj[v]; };
    const forward_list<const edge *> & edges(void) const { return Edges; };
    unsigned int v(void) const {return V; };
    ~dgraph()
    {
        while(!Edges.empty())
        {
            delete Edges.front();
            Edges.pop_front();
        }
    };
};
#endif
