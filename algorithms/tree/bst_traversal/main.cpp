#include <iostream>
#include <cstdint>
#include <stack>
#include <queue>
#include <stddef.h>
#include <vector>

template <typename T>
struct _node 
{
    struct _node *l;
    struct _node *r;
    T v;
};
template <typename T>
struct _anode
{
    struct _anode *p;
    struct _anode *c;
    struct _anode *sib;
    size_t sib_num;
    T v;
};
/*utility functions */
void print_node(const struct _node<int> *n)
{
    std::cout << n->v << "  ";
    
}
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

/* iterative implementation of traversal */
void traverse_in_order_it(const struct _node<int> *n)
{
    if (n == NULL)
        return;
    std::stack<const struct _node <int>*, std::vector<const struct _node<int> *>> sk; /* stoes nodes to be processed */
    std::stack<bool> esk; /* coresponding to the nodes in sk, indicates whether                              the node needs to to explored */ 
    sk.push(n);
    esk.push(false);
    while(!sk.empty())
    {
        n = sk.top();    
        sk.pop();
        if (esk.top())
        {
            print_node(n);
            esk.pop();
            continue;
        }
        esk.pop(); 
        if (n->r != NULL)
        {
           sk.push(n->r);
           esk.push(false);
        }
        sk.push(n);
        esk.push(true);
        if (n->l != NULL)
        {
            sk.push(n->l);
            esk.push(false);
        } 
    }  
}

void traverse_pre_order_it(const struct _node <int> *n)
{
    if (n == NULL)
        return;
    std::stack<const struct _node<int> *, std::vector<const struct _node <int> *>> sk;
    sk.push(n);
    while(!sk.empty())
    {
        n = sk.top();
        sk.pop();
        print_node(n);
        if (n->r != NULL)
            sk.push(n->r);
        if (n->l != NULL)
            sk.push(n->l);
    }
}

void traverse_post_order_it(const struct _node <int> *n)
{
    if (n == NULL)
        return;
    std::stack<const struct _node <int>, std::vector<const struct _node <int>*>> sk;
    std::stack<bool> esk;  
    sk.push(n);
    esk.push(false);
    while(!sk.empty())
    {
        n = sk.top();
        if (esk.top())
        {
            esk.pop();
            sk.pop();
            print_node(n);
            continue;
        }
        esk.pop();
        esk.push(true);
        if (n->r != NULL)
        {
           sk.push(n->r);
           esk.push(false);
        }
        if (n->l != NULL)
        {
            sk.push(n->l);
            esk.push(false);
        }
    }
}
void traverse_by_level(const struct _node <int> *n)
{
    if (n == NULL)
        return;
    std::queue<const struct _node <int> *> q;
    q.push(n);
    size_t cur = 1, next = 0;
    while(!q.empty())
    {
        if (cur == 0)
        {
            cur = next;
            next = 0;
            std::cout << std::endl;
        }
        n = q.front();
        q.pop(); 
        --cur;
        print_node(n);
        if (n->l != NULL)
        {
            q.push(n->l);
            ++next;
        }
        if (n->r != NULL)
        {
            q.push(n->r);
            ++next;
        }
    }
       
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
void traverse_contour(const struct _node <int> *root, const bool no_bottom = false)
{
   if (root == NULL)
       return; 
   print_node(root);
   std::cout << "\n";
   size_t cur1 = 0, cur2 = 0, next1 = 0, next2 = 0;
   std::queue<const struct _node<int> *, std::deque<const struct _node <int> *>> q1, q2;
   if (root->l != NULL)
   {
      q1.push(root->l);
      cur1 ++;
   }
#if 1 
   /*(q2 is activated only if no_bottom is true */
   if (root->r != NULL)
   {
       if (!no_bottom)
       {
           q1.push(root->r);
           ++cur1;
       }
       else
       {
           q2.push(root->r);
           ++cur2;
       }
   }
#endif
   while(cur1 + cur2 + next1 + next2 != 0)
   {
       if (cur1 + cur2 == 0)
       {
           cur1 = next1;
           cur2 = next2;
           next1 = 0;
           next2 = 0;
           std::cout<< std::endl;
       }
       if (cur1 != 0)
       {
           root = q1.front();
           q1.pop();
           if (next1 == 0 || (root->l == NULL && root->r == NULL) || cur1 == 1 )
               print_node(root); 
           else
              std::cout << "   ";
           --cur1;
           if (root->l != NULL)
           {
               q1.push(root->l);
               ++next1;
           }
           if (root->r != NULL && !no_bottom)
           {
               q1.push(root->r);
               ++next1;
           }
       }
       if (cur2 != 0)
       {
          root = q2.front(); 
          q2.pop();
          print_node(root);
          --cur2;
          if (root->r != NULL)
          {
              q2.push(root->r);
              ++next2;
          }
       }
   } 
} 
int main()
{
    struct _node <int> *r = build_tree(10, 0);
    std::cout << "recursive in order traversal \n";
    traverse_in_order(r);
    std::cout << "\niterative in order traversal \n";
    traverse_in_order_it(r);
    std::cout << "\nrecursive pre order traversal \n";
    traverse_pre_order(r);
    std::cout << "\niterative pre order traversal\n";
    traverse_pre_order_it(r);
    std::cout << "\nrecursive post order traversal \n";
    traverse_post_order(r);
    std::cout << "\niterative post order travsersal\n";
    traverse_post_order_it(r);
    std::cout <<"\ntraverse by level\n";
    traverse_by_level(r);
    std::cout << std::endl;
    std::cout <<"print contour without bottom\n";
    traverse_contour(r, true);
    std::cout << std::endl;
    std::cout << "print contour with bottom\n";
    traverse_contour(r, false);
    std::cout << std::endl;
    release_tree(r);
    return 0;
}










