#include <iostream>
#include <cstdlib>
#include <node.h>
using namespace std;
 
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
