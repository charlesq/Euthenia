#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cstring>
#include <iostream>
using namespace std;
class graph
{
   unsigned int V; // denotes number of vertex in the graph
   unsigned int E; // denotes number of edges in the graph 
   vector<vector<unsigned int>> adj;

public:
    graph(string fn)
    {
        char buf[100];
        ifstream in(fn);
        if (!in.good())
           throw -1;
        in.getline(buf, 100, '\n');
        sscanf(buf, "%d", &V);
        in.getline(buf, 100, '\n');
        sscanf(buf, "%d", &E);
        adj.resize(V);
        unsigned int e1, e2;
        for (unsigned int i = 0; i < E; i ++)
        {
            in.getline(buf, 100, '\n');
            sscanf(buf, "%d %d", &e1, &e2); 
            adj[e1].push_back(e2);
            adj[e2].push_back(e1);
        }
        in.close();
        
    };
    inline unsigned int getV(void) const {return V; };
    inline const vector<unsigned int> & get_adj(unsigned int i) const 
    { return adj[i]; };
    ~graph()
    {
        for (vector<unsigned int> &it: adj)
            it.clear();
        adj.clear();
    };
};

class DFS
{
    const graph &g;
    vector<bool> visited;
    
public:
    DFS(const graph &g ):g(g)
    {
       visited.resize(g.getV(), false); 
    }
    void operator()(unsigned int s)
    {
        if (visited[s])
            return;
        cout << " discoverd vertice " << s << endl;
        visited[s] = true;
        const vector<unsigned int> &adj = g.get_adj(s);     
        for (unsigned int a: adj)
            (*this)(a);
    };
    ~DFS(void) { visited.clear();};
};
#include <queue>
class BFS
{
   enum color {eUndiscovered = 0,eDiscovered, eExplored};
   const graph &g;
   vector<unsigned int> clr;
   queue<unsigned int> q; 
public:
   BFS(const graph &g):g(g)
   {
       clr.resize(g.getV(), eUndiscovered);
   }; 
   bool operator () (unsigned int s)
   {
      cout << "begin with " << s << endl;
      q.push(s);
      clr[s] = eDiscovered;
      while(!q.empty()) 
      {
          s = q.front();
          q.pop(); 
          if (clr[s] == eExplored) continue;
          cout << "Exloring " << s << " discovered: " << endl;
          const vector<unsigned int> &adj = g.get_adj(s);
          for (unsigned int ep : adj)
          {
              if (clr[ep] == eUndiscovered)
              {
                  clr[ep] = eDiscovered;
                  q.push(ep);
                  cout << " " << ep;
              }
          }
          clr[s] = eExplored;
          cout << endl<<  " Exploring of " << s << "is over " << endl;
      }
   };
   ~BFS()
   {
       clr.clear();
   };
};
int main()
{
    graph g("graph.txt");
    DFS dfs(g);
    dfs(0); 
    BFS bfs(g);
    bfs(0);
    return 0;
}
