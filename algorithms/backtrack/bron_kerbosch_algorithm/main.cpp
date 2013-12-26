#include <unordered_set>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <forward_list>
#include <algorithm>
/* this snippet demonstrates using bron kerbosch algorithm to list all maximal cliques in a graph,
   which is a NP-hard problem.
   The algorithm is a refinement of the general backtracking algorithm. 
*/
using namespace std;
class graph
{
    unsigned int _v;
    vector<unordered_set<unsigned int>> _adj;
public:
    graph(string fn):_v(0)
    {
       ifstream im(fn); 
       if (!im.is_open())
       {
           cout << "failed to open " << fn << std::endl; 
           return;
       }
       unsigned int ec, ep1, ep2; 
       im >> _v;
       im >> ec;
       _adj.resize(_v);
       while(ec -- != 0)
       {
           im >> ep1 >> ep2;
           _adj[ep1].insert(ep2);
           _adj[ep2].insert(ep1);
       }
       im.close();
    };
    inline const unordered_set<unsigned int> &adj(unsigned int i) const  { if (_v == 0) i = 0; _adj[i]; };
    inline unsigned int v(void) const { return _v; };
    ~graph()
    {
        for(auto &entry: _adj)
           entry.clear();
        _adj.clear();
    };
};
class bron_kerbosch
{
    const graph *_g;
    forward_list<unordered_set<unsigned int>> _mcs; /* stores all maximal cliques */
    void _reset(void)
    {
        for (auto &s: _mcs)
            s.clear();
        _mcs.clear();
    };
    /* apply backtracking  */
    void _bt( unordered_set<unsigned int> &candidate_set, unordered_set<unsigned int> partial_solution)
    {
       if (candidate_set.empty())
       {
          if (!partial_solution.empty())
          { 
              /* accept if not duplicate */
              if (std::find(_mcs.begin(),_mcs.end(), partial_solution) == _mcs.end())
                  _mcs.push_front(partial_solution);
          }
          /* else reject */
          return;
       }
       /* generate candidate extensions */
       for (auto vs: candidate_set)
       {
           unordered_set<unsigned int> cs;
           /* first candidate extension */
           partial_solution.insert(vs);
           /* next candidate extension */
           auto adj = _g->adj(vs); 
           /* check only vertices adjacent to vs and  make sure the clique is a maximal one */
           for (auto ep: candidate_set)
           {
              if (adj.find(ep) != adj.end())
                  cs.insert(ep); 
           }  
           _bt(cs, partial_solution);
          partial_solution.erase(vs);
       }    
    };
public:
    bron_kerbosch(void){};
    const forward_list<unordered_set<unsigned int>> & operator ()(const graph &g)
    {
        _g = &g;
        _reset();
        unordered_set<unsigned int> candidate_set, partial_solution; 
        for (unsigned int v = 0; v < _g->v(); v ++)
            candidate_set.insert(v);
        _bt(candidate_set,partial_solution); 
        return _mcs;
    };
    ~bron_kerbosch()
    {
        _reset();
    };
};

int main()
{
    graph g("graph.txt");
    bron_kerbosch bk;
    auto maximal_clique_set = bk(g);
    size_t i = 0;
    for (auto &clique: maximal_clique_set)
    {
        if (++i % 5 == 0) 
            cout << endl;
        cout  << "   (";
        for (auto ep: clique)
            cout << ep << " ";
        cout << ")  " ; 
    }
    cout << endl;
    
    return 0;
}
