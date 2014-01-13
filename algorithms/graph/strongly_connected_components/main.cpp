#include <dgraph.hpp>
#include <SCC.hpp>

int main()
{
   dgraph g("graph.txt");
   SCC scc(g);
   scc.run();
   return 0;
}
