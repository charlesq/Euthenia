#include <include/BFS.h>
void BFS::traverse(size_t s)
{
    size_t ep;
    if (colors[s] != UNDISCOVERED)
        return;
    /* mark the starting vertex as discovered */
    colors[s] = DISCOVERED;
    if (discovered != NULL)
        discovered->act(s);
   /* push starting vertex to the queue */
    q.push(s);
    
    /* Each iteration begins with checking the list */ 
    while(!q.empty())
    {
        s = q.front();
        q.pop();
        switch(colors[s])
        {
        case EXPLORED: /* do nothing */
            break; 
        case DISCOVERED:
            /* exploring edges of the vertex */
            const auto &adj = g->Adj(s); 
            for (const auto eg: adj)
            {
                ep = eg->E() == s? eg->O(): eg->E();
                /* examine color */
                if (colors[ep] != UNDISCOVERED)
                   continue;
                /* push newly discovered vertex to the queue */
                colors[ep] = DISCOVERED;
                if (discovered != NULL)
                    discovered->act(ep);
                q.push(ep);  
            }
            /* exploration is complete for the vertex */
            colors[s] = EXPLORED;
            if (explored != NULL)
                explored->act(s);
        }
    }

     
}

