#ifndef __NODE_H__
#define __NODE_H__
typedef struct _node node;
struct _node
{
    int key;
    node *l;
    node *r;  
};
void build_tree(node ** r, unsigned int &remaining);
void deleteTree(const node *r);
const node * pick_node(const node *n);

#endif
