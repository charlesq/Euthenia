
#include <map>
#include <vector>
#include <forward_list>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include <queue>
#include <deque>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <set>
template<typename T>
class disjoint_set
{
    disjoint_set * head;
    T v;
    disjoint_set(T va):v(va){ head = this;};
public:
    friend disjoint_set<T> * find_set(disjoint_set<T> *ds)
    {
       while(ds->head != ds)
       {
          ds = ds->head;   
       } 
       return ds;
    };
    friend disjoint_set<T> * make_set(disjoint_set<T> *, T v)
    {
         return new disjoint_set<T>(v);
    };
    friend void make_union(disjoint_set<T> *ds1, disjoint_set<T> *ds2)
    {
        disjoint_set<T> *h;
        h = find_set(ds1);
        h->head = ds2;
    };
};
class edge
{
    unsigned int ep0;
    unsigned int ep1;
    unsigned int w;
public:
   edge(unsigned int v0, unsigned int v1, unsigned int wgt):ep0(v0), ep1(v1), w(wgt){};
   unsigned int get_v(void) const { return ep0;};
   unsigned int get_other(unsigned int v) const { return v == ep0? ep1: ep0; };
   unsigned int get_w (void) const { return w; };
   void print(void) const
   {
       std::cout << "( " << ep0 << ", " << ep1 << " , " << w << ") ";
   };
};
class graph
{
    unsigned int V;
    std::vector<std::forward_list<const edge *>> adj; 
    std::unordered_set<const edge *> edges;
public:
    graph(std::string fn)
    {
       std::ifstream in(fn); 
       unsigned int num, e0, e1, w;
       in >> V >> num;
       adj.resize(V);
       edge *e;
       while(num -- != 0)
       {
           in >> e0 >> e1 >> w;
           e = new edge(e0, e1, w);
           adj[e0].push_front(e);
           adj[e1].push_front(e);
           edges.insert(e);
       }
       in.close();
    };
    inline const std::unordered_set<const edge *> & get_edges(void) const { return edges; };
    inline unsigned int get_v(void) const { return V; };
    inline const std::forward_list<const edge *> & get_adj(unsigned int v) const { return adj[v];}; 
    ~graph()
    {
        for (auto i: edges)
        {
            delete i;
        } 
        for (auto &i : adj)
            i.clear();
        adj.clear();
    };
};
class mst
{
protected:
    const graph &g;
    std::unordered_set<const edge *> mstree;
    mst(const graph &gr):g(gr){};
    virtual void run(unsigned int s) = 0;
    virtual void clear(void) {};
public:
   const std::unordered_set<const edge *>  operator ()(unsigned int s = 0)
   {
       run(s);
       return mstree;
   };
   void print_mst(void) const
   {
      std::cout << "Produced MST is composed of edges: " << std::endl;
      for(const edge  *e: mstree) 
         e->print();
   };
   virtual ~mst()
   {
       mstree.clear();
   };
};
class compare
{
public:
    bool operator ()(const edge * a, const edge *b) const
    {
        return a->get_w() >= b->get_w();
    };
};
class kruskal:public mst
{
   std::priority_queue<const edge *, std::vector<const edge*>, compare> pq;
   std::unordered_map<unsigned int, disjoint_set<unsigned int>*> mset;
   std::unordered_set<disjoint_set<unsigned int>*> uds; 
   void clear(void)
   {
      for (auto i:uds)
          delete i;
      uds.clear(); 
   };
public:
   kruskal(const graph &g):mst(g)
   {
       auto edges = g.get_edges();
       for (auto it: edges)
           pq.push(it);
       for (unsigned int i = 0; i < g.get_v(); i ++)
       {
           mset[i] = make_set(mset[0], i); 
           uds.insert(mset[i]);
       }
   };
   void run(unsigned int s)
   {
      const edge *eg;
      disjoint_set<unsigned int> *ds1,  *ds2; 
      while(!pq.empty())
      {
          eg = pq.top();  
          pq.pop();
          ds1 = mset[eg->get_v()];
          ds2 = mset[eg->get_other(eg->get_v())];
          if ( find_set(ds1)  != find_set(ds2))
          {
              make_union(ds2, ds1);
              mstree.insert(eg);
          }
      }   
   };
   ~kruskal()
   {
       clear();
   };
}; 
class prim: public mst
{
    std::priority_queue<const edge *, std::vector<const edge*>, compare> pq; 
    std::set<unsigned int> cs;
    void scan(unsigned int v)
    {
        const std::forward_list<const edge *> &adj = g.get_adj(v); 
        for (const edge *e: adj)
        {
            unsigned int vo = e->get_other(v);
            if (cs.find(vo) == cs.end())
               pq.push(e); 
            
        } 
    };
public:
    prim(const graph &g): mst(g)
    {
    };
    void run(unsigned int s)
    {
         cs.insert(s);
         scan(s);  
         while(cs.size() != g.get_v())
         {
             const edge *e = pq.top();
             pq.pop();
             unsigned int v1 = e->get_v(), v2 = e->get_other(v1);  
             auto it1 = cs.find(v1), it2 = cs.find(v2); 
             if (it1 != cs.end() && it2 != cs.end())
                 continue;
             mstree.insert(e);
             
             if (it1 == cs.end())
             {
                 cs.insert(v1);
                 scan(v1);
             } 
             else
             {
                 cs.insert(v2);
                 scan(v2);
             }
         }
    };
    ~prim()
    {
       cs.clear(); 
       while(!pq.empty())
           pq.pop();
    };
};
#include <iostream>
int main()
{
    graph g("graph.txt"); 
    std::unique_ptr<mst> um (new kruskal(g));
    (* um)();
    um->print_mst(); 
    um = std::unique_ptr<mst>(new prim(g));
    (*um)(1);
    um->print_mst();
    return 0;
}
