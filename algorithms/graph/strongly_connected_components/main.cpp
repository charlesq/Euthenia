#include <iostream>
#include <fstream>
#include <forward_list>
#include <cstdint>
#include <vector>
#include <cstring>
#include <stack>
#include <queue>
#include <deque>

using namespace std;
class edge
{
    const unsigned int from;
    const unsigned int to; 
    const int wt;
public:
    edge (unsigned int f, unsigned int t, int w):from(f), to(t), wt(w) {};
    unsigned int f(void) const { return from; };
    unsigned int t(void) const { return to; };
    int w(void) const {return wt; }; 
};

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
void printv(unsigned char v)
{
    std::cout << " " << v ; 
}
class SCC
{
    typedef void (*print_v)(unsigned char v);
    typedef stack<unsigned int, vector<unsigned int>> Stack;
    const dgraph &g;
    dgraph *tg;
    vector<bool> discovered;
    Stack  sk;
    deque<unsigned int> q;
    bool scan(const dgraph &gh, bool stack_locked, unsigned int v, print_v p)
    {
        bool  found_new = false;
        const forward_list<const edge *> adj = gh.adj(v);
        for (auto l: adj)
        {
            if (discovered[l->t()])
                continue;
            else
            {
                if (!found_new)
                    found_new = true; 
              //  if (stack_locked)
                q.push_front(l->t());
                dfs(gh, stack_locked, p);
            }
        }
        return found_new;
    };
    void dfs(const dgraph &gh, bool stack_locked,  print_v p )
    {
         unsigned int v;
         /* in 2nd pass, starting vertex are fetched always from stack, specified by caller or 
            from scaning  */
         if (q.empty())
             return;          
         v = q.front();
         q.pop_front();
         if (!discovered[v])
         {
             if (p != NULL)
                 p (v + '0');
             discovered[v] = true;
             if (!stack_locked)
                 sk.push(v); 
             if (!scan(gh,stack_locked,v, p) && stack_locked)
             {
                 if (p)
                     p('\n');
             }
         }
         dfs(gh, stack_locked, p);
    };
public:
    void run(void)
    {
        if (g.v() == 0)
            return;
        discovered.resize(g.v());
        /* run a dfs on original graph, and store the discovery sequence on a stack */
        for (unsigned int v = 0; v < g.v(); v ++)
        {
            if (!discovered[v])
            {
               q.push_back(v);
               dfs(g,false, NULL);         
            }
        }
        discovered.flip();
        //discovered.resize(tg->v());
        /* all dfs shall start from vertice on the queue */
        while (!sk.empty())
        {
          q.push_front(sk.top());
          sk.pop();
        } 
        dfs(*tg, true,printv);
    };
 
public:
    SCC(const dgraph &gh): g(gh)
    {
        tg = new dgraph(g, true);
        discovered.resize(g.v()); 
        
    };
    void operator()(void)
    {
        run();
    };
    ~SCC()
    {
        delete tg;
    };
};

int main()
{
   dgraph g("graph.txt");
   SCC scc(g);
   scc.run();
   return 0;
}
