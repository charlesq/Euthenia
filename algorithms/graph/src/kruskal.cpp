#include <include/kruskal.h>
/* worker function for kruskal algorithm */
void kruskal::do_work(size_t)
{
    /* rest in case of re-run */
    reset(); 
    /* generate a disjointed set for each vertext */
    for (size_t i = 0; i < g->V(); ++i)
        v.push_back(make_set(i));
    /* push all edges to the prioirty queue */
    for(const auto edge:  g->Edges())
    {
       pq.push(edge);
    }
    /* repeatedly examine minimal-weight edges on queue until empty */
    while(!pq.empty())
    {
        const auto edge = pq.top();
        pq.pop();
        /* if endpoints of the edge belong to separate sets, joint the
           two sets and  add it to minimal spanning tree forest */
        if (find_set(v[edge->O()]) != find_set(v[edge->E()]))
        {
            weights += edge->W();
            mstree.insert(edge);
            make_union(v[edge->O()], v[edge->E()]);
        }
    }
}
