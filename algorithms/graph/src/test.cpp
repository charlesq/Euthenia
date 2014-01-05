#include <include/graph.h>
#include <include/search.h>
#include <include/BFS.h>
#include <include/DFS.h>
#include <iostream>
#include <include/action.h>
#include <unordered_set>
#include <include/prim.h>
#include <include/kruskal.h>
class print: public action
{
    std::string str;
public:
    print(std::string s): str(s) {};
    virtual void act(size_t v)
    {
        std::cout << str << " vertex " << v << std::endl; 
    }
};
void print_mst(const std::unordered_set<const edge *> &mset)
{
    std::cout <<" { ";
    for (const auto eg: mset)
    {
        std::cout << "(" << eg->E() << "," << eg->O() << ") ";
    }
    std::cout << " }";

}
/* test driver */
int main(int argc, char *argv[])
{
    graph g("graph.txt");
    print dis("Discovered"), exp("Explored");
    search *dfs = new DFS(&g);
    dfs->setAction(&dis, true);
    dfs->setAction(&exp, false);
    search *bfs = new BFS(&g);
    bfs->setAction(&dis, true);
    bfs->setAction(&exp, false);
    /* conduct depth first search */
    std::cout << "********************************************" << std::endl;
    std::cout << "A Depth First Search Traversal of the graph" << std::endl; 
    (*dfs)();
    /* conduct breadth first search */
    std::cout << "********************************************" << std::endl;
    std::cout << "A Breadth First Search Traversal of the graph" << std::endl;
    (*bfs)();
    delete dfs;
    delete bfs;
    std::cout << "********************************************" << std::endl;
    mst *prima = new prim(&g);
    std::cout <<"Applying Prim Algorithm to produce minimal spanning tree \n";
    for (size_t i = 0; i < g.V(); ++i)
    {
       std::unordered_set<const edge *> mstree;
       mstree  = (*prima)(i); 
       if (!mstree.empty())
       {
             print_mst(mstree);
       }
    }
    std::cout << "\n Total weights of the spanning tree/forest is " << prima->W()<< std::endl;
    delete prima;
    std::cout << "\n********************************************" << std::endl;
    std::cout <<"Applying Kruskal algorithm to produce minimal spanning tree \n";
    mst *kruskala = new kruskal(&g);
    auto mset = (*kruskala)();
    print_mst(mset);
    std::cout << std::endl;
    std::cout << " Total weights of the spanning tree/forest is " << kruskala->W()<< std::endl; 
    delete kruskala;
    return 0;
}
