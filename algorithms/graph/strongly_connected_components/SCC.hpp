#ifndef __SCC_H__
#define __SCC_H__ 
#include <dgraph.hpp>
#include <iostream>
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
#endif
