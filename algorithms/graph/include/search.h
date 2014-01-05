#ifndef __SEARCH_H__
#define __SEARCH_H__
#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <include/action.h>
#include <include/graph.h>
/* abstract search class is defined to fields/functions common to both 
   DFS, and BFS algorithms
*/
class search
{
protected:
    enum {UNDISCOVERED = 0, DISCOVERED, EXPLORED};
    const graph *g;
    action *discovered;
    action *explored;
    std::vector<int> colors; /* track color for each vertice */ 
    search(const graph *gh): g(gh), discovered(NULL), explored(NULL)
    {
         int color = UNDISCOVERED;
         colors.resize(g->V(), color);
    };
public:
    virtual void traverse(size_t s) = 0;
    virtual void setAction(action *a, bool disc) final
    {
       if (disc)
          discovered = a; 
       else
          explored = a;
    };
    virtual void operator()(void) final
    {
        for (size_t i = 0; i < g->V(); ++i)
            traverse(i);
    };
};
#endif
