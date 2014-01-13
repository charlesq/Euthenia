#include <node.h>
#include <iostream>
#include <cstdlib>
using namespace std;
 
void build_tree(node ** r, unsigned int &remaining) 
{
    if (remaining == 0)
        return; 
    
    if (*r == NULL)
    {
        *r = new node;
        (*r)->l = NULL;
        (*r)->r = NULL;
        remaining --;
        build_tree(r, remaining);
    }
    else
    {
        unsigned int rn = rand();
        if (rn %2 == 0)
            (*r)->l = new node;
        if (--remaining != 0)
        {
            (*r)->r = new node;
            node *p = (*r)->r;
            p->l = NULL;
            p->r = NULL;
            remaining--; 
        }
        if (remaining == 0)
            return;
        if ((*r)->r != 0)
        {
            unsigned int rl = remaining /2;
            unsigned int rr = remaining - rl;
            build_tree(&(*r)->l, rl); 
            build_tree(&(*r)->r, rr);
            return;
        }
       build_tree(&(*r)->l, remaining);
 
    } 
   
}
const node * pick_node(const node *n) 
{
   const node *ret = NULL;
   if (n == NULL)
       return NULL;
   if (rand() % 2  == 0)
   {
       return n;
   } 
   ret = pick_node(n->l);
   if (rand()% 2 == 0)
       return ret;
   return pick_node(n->r);
}
void deleteTree(const node *r)
{
    if (r == NULL)
        return;
    deleteTree(r->l);
    deleteTree(r->r);
    delete r;
}
