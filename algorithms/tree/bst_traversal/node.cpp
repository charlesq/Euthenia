#include <iostream>
#include <cstdint>
#include <stack>
#include <queue>
#include <stddef.h>
#include <vector>
#include <node.h>

/*utility functions */
void print_node(const struct _node<int> *n)
{
    std::cout << n->v << "  ";
    
}

struct _node <int> *build_tree(int h, int l)
{
    struct _node <int> *r;
    int v = (h + l)/2;
    r = new struct _node <int>;
    r->v = v;
    r->l = NULL;
    r->r = NULL;
    if (l != v)  
        r->l = build_tree(v -1, l);
    if ( h != v)
        r->r = build_tree( h, v + 1);
    return r;
}
void release_tree(const struct _node <int> *r)
{
    if (r == NULL)
        return;
    release_tree(r->l);
    release_tree(r->r);
    delete r;
}






