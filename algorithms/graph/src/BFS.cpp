#include <include/BFS.h>
void BFS::traverse(size_t s)
{
    size_t ep;
    l.push_front(s);
    while(!l.empty())
    {
        s = l.front();
        l.pop_front();
        switch(colors[s])
        {
        case EXPLORED: /* do nothing */
            break; 
        case UNDISCOVERED:
            colors[s] = DISCOVERED;
            if (discovered != NULL)
               discovered->act(s);
            /* fall through to scan neighbors */
        case DISCOVERED:
            const auto &adj = g->Adj(s); 
            for (const auto eg: adj)
            {
                ep = eg->E() == s? eg->O(): eg->E();
                /* examine color */
                if (colors[ep] != UNDISCOVERED)
                   continue;
                l.push_front(ep);  
            }
            colors[s] = EXPLORED;
            if (explored != NULL)
                explored->act(s);
        }
    }

     
}

