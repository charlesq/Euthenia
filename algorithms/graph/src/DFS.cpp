#include <include/DFS.h>

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
    const auto &adj = g->Adj(s);
    for (const auto eg: adj)
    {
        ep = eg->E() == s? eg->O(): eg->E();
        switch (colors[ep])
        {
        case EXPLORED:
        case DISCOVERED:
            break;
        case UNDISCOVERED:
            {
                colors[ep] = DISCOVERED;
                if (discovered != NULL)
                    discovered->act(ep);
            }
            traverse(ep);     
        
        } 
     }
     colors[s] = EXPLORED;  
     if (explored != NULL)
        explored->act(s); 
}
