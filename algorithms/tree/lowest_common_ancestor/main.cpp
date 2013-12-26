#include <iostream>
#include <cstdlib>
using namespace std;
typedef struct _node node;
struct _node
{
    int key;
    node *l;
    node *r;  
};


 
const node * flca(const node *r, const node *a, const node *b)
{
    if (a == b)
       return a;
    if (a == NULL || b == NULL)
        return NULL;
    if (a == r || b == r)
    {
       return r;
    }
    if (r == NULL )
        return NULL;
    const node *p1 = flca(r->l, a, b);  
    const node *p2 = flca(r->r, a, b);
    if (p1 != NULL && p2 != NULL)
        return r;
    p1 = p1 == NULL? p2: p1;
    return flca(p1, a, b); 
    
}

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
int main()
{
   node *r = NULL;
   unsigned int nodes = 15;
   build_tree(&r, nodes);

  while(1)
  {
      srand(time(NULL));
      const node *a = pick_node(r);
      const node *b = pick_node(r);
      const node *ance = flca(r, a, b);
      char ret;
      cout << "Continue(Y/N)?: ";
      cin >> ret; 
      if (ret == 'y' || ret == 'Y')
          continue; 
      break;
 } 
 deleteTree(r);
}
