#include <include/kruskal.h>

void kruskal::do_work(size_t)
{
    v.clear();
    /* generate a disjointed set for each vertext */
    for (size_t i = 0; i < g->V(); ++i)
        v.push_back(make_set0(i));
    for(const auto edge:  g->Edges())
    {
       pq.push(edge);
    }
    while(!pq.empty())
    {
        const auto edge = pq.top();
        pq.pop();
        if (find_set(v[edge->O()]) != find_set(v[edge->E()]))
        {
            mstree.insert(edge);
            make_union(v[edge->O()], v[edge->E()]);
        }
    }

}
