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
    /* the three color values, used to mark a vertex during search */
    enum {UNDISCOVERED = 0, DISCOVERED, EXPLORED};
    const graph *g;
    action *discovered;
    action *explored;
    std::vector<int> colors; /* track color for each vertice */ 
    search(const graph *gh): g(gh), discovered(NULL), explored(NULL)
    {
         /* mark all vertices as UNDISCOVERED, before search */
         int color = UNDISCOVERED;
         colors.resize(g->V(), color);
    };
public:
    /* worker function to be implemented */
    virtual void traverse(size_t s) = 0;
    /* function to provision actions on vertices during search */
    virtual void setAction(action *a, bool disc) final
    {
       if (disc)
          discovered = a; 
       else
          explored = a;
    };
    /* common entry function for all search algorithms */
    virtual void operator()(void) final
    {
        /* put a for loop here to make sure all vertices will be explored */
        for (size_t i = 0; i < g->V(); ++i)
            traverse(i);
    };
};
#endif
