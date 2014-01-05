#include <include/DFS.h>
/* the main function of DFS */
void DFS::traverse(size_t s)
{
    size_t ep;
    if (colors[s] == EXPLORED)
        return;
    if (colors[s] == UNDISCOVERED)
    {
        colors[s] = DISCOVERED;
        if (discovered != NULL)
            discovered->act(s);
    }
    /* explore neighboring vertices in a depth first and exhaustive way */
    const auto &adj = g->Adj(s);
    for (const auto eg: adj)
    {
        ep = eg->E() == s? eg->O(): eg->E();
        switch (colors[ep])
        {
        case EXPLORED: /* do nothing */
        case DISCOVERED: /* do nothing */
            break;
        case UNDISCOVERED:
            {
                /* mark the it as DISCOVERED */ 
                colors[ep] = DISCOVERED;
                if (discovered != NULL)
                    discovered->act(ep);
            }
            /* dive into the discoverd vertex */
            traverse(ep);     
        
        } 
     }
     /* we have examined all vertices which can be rooted to the vertex */
     colors[s] = EXPLORED;  
     if (explored != NULL)
        explored->act(s); 
}
