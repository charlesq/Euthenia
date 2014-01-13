#include <node.h>
/* recursive implementation of BST traversals */
void traverse_in_order(const struct _node <int> *n)
{
    if (n == NULL)
        return;
    traverse_in_order(n->l);
    print_node(n);
    traverse_in_order(n->r);
} 
void traverse_pre_order(const struct _node <int> *n)
{
    if (n == NULL)
        return;
    print_node(n);
    traverse_pre_order(n->l);
    traverse_pre_order(n->r);
}

void traverse_post_order(const struct _node <int> *n)
{
    if (n == NULL)
        return;
    traverse_post_order(n->l);
    traverse_post_order(n->r);
    print_node(n);
}








