#include <iostream>
#include <stddef.h>
class rb_tree
{
public:
    enum {RED = 0, BLACK = 1};
    typedef  struct _node 
    {
        int key;
        struct _node *left;
        struct _node *right;
        struct _node *parent;
        int  color; // 0 indicates red, 1 indicates black;
    }node;
private:
    node *new_node(int key)
    {
        node *n = new node;
        n->key = key;
        n->left = n->right = n->parent = &nil_node;
        n->color = RED; 
        return n;
    };
    void delete_tree(node *r)
    {
       if (r != &nil_node)
       {
           delete_tree(r->left);
           delete_tree(r->right);
           delete r;
       } 
    };
    const node *uncle(const node *n) const
    {
        if (n == root)
            return NULL;
        return (n->parent->left == n)? n->parent->right: n->parent->left;
    }; 
    const node * grandparent(const node *n) const 
    {
        if (n->parent == NULL)
            return NULL; 
        return n->parent->parent;
    };

    node * where_to_insert (int key)
    {
        node *n = root,*prev = root;
        while(n != &nil_node)
        {
            prev = n;
            if (n->key < key)
                n = n->left;
            else
                n = n->right;
        }
        return prev;
    };
    void rotate_left(node *n)
    {
       node *p = n->parent; 
       balance_tree(p);
       if (p->parent->left == p)
           p->parent->left = n;
       else
           p->parent->right = n;
       n->parent = p->parent;
       p->parent = n;
       p->right = n->left;
       n->left->parent = p;
       n->left = p;
    }
    void rotate_right(node *p)
    {
       node *g = p->parent;
       p->parent = g->parent;
       if (p->parent->left == g)
           p->parent->left = p;
       else
           p->parent->right = p;
       g->left = p->right;
       g->left->parent = g;
       p->right = g;
       p->color = BLACK;
       g->color = RED;
    }
    void balance_tree(node *n)
    {
        /* self-balance case one: parent is NULL */
        if (n->parent == NULL)
            n->color = BLACK;
        /* self-balance case two: n's parent color is black */
        /* no violation, do nothing */
        if (n->parent->color == BLACK)
            return;
        /* self-balance case three: parent color is red, uncle is RED */ 
        if(uncle(n)->color == RED)
        {
            n->parent->color = BLACK;
            const_cast<node*>(uncle(n))->color = BLACK;
            const_cast<node*>(grandparent(n))->color = RED;
            balance_tree(const_cast<node*>(grandparent(n)));
            return;
        }
        /* self-balance case four: uncle is RED, n is right  child */ 
       if (n->parent->right == n)
       {
           rotate_left(n);
           balance_tree(n);
           return;
       }
       rotate_right(n->parent); 
 
    };
    const node *find_node(int k) const
    {
       const node *n = root;
       while(n != NULL && n != &nil_node)
       {
           if (n->key == k)
               break;
           if (n->key < k)
               n = n->right;
           else
               n = n->left;
       }
       return n;
    };
    const node *predecesor(const node *n) const
    {
        const node *pre = n->left;
        if (n->left != &nil_node)
        {
            while(pre->right != &nil_node)
                pre = pre->right;
        } 
        else
        {
          
           while (n != NULL && n->parent->left == n )
                n = n->parent; 
           if (n != NULL)
           {
               pre = n ->left;
               while(pre->right != &nil_node)
                   pre = pre->right;
           }
        } 
        return pre;

    }; 
    const node *successor(const node *n) const
    {
        const node *succ = &nil_node;
        if (n->right != &nil_node)
        {
            succ = n->right;
            while(succ->left != &nil_node)
                succ = succ->left;
        }
       else
        {
            while(n != NULL && n->parent->right == n)
                n = n->parent;
            if (n != NULL)
            {
                succ = n -> right;
                while(succ->left != &nil_node)
                    succ = succ->left;
            }
                
        }
        return succ;
    };   
    void replace(node *a, node *b)
    {
        if (a == b)
            return;
        std::swap(*a, *b);
        if (a->parent->left == b)
           a->parent->left = a;
        else
           a->parent->right = a;
        if (b->parent->left == a)
           b->parent->left = b;
        else
           b->parent->right = a;
        if (a->left != &nil_node)
            a->left->parent = a;
        if (a->right != &nil_node)
            a->right->parent = a;
        if (b->left != &nil_node)
            b->left->parent = b;
        if (b->right != &nil_node)
            b->right->parent = b;
    };
public:
    rb_tree(void): root(&nil_node) 
    {
        nil_node.color = BLACK; 
        nil_node.left = nil_node.right = NULL;
    };
    bool insert(int key)
    {
         node *n = where_to_insert(key);
         /* scenario of  empty tree  */
         if (n == &nil_node)
         {
            root = new_node(key);
            root->parent = NULL;
            return true;
         }
         /* scenario of  already exists */
         if (n->key == key)
             return false;
         node * anode = new_node(key);
         anode->parent = n;
         /* */
         if (n->key < anode->key)
         {
            n->left = anode; 
         }   
         else
         {
            n->right = anode;
         }
         balance_tree(anode);
         return true;
    };
    bool is_leaf(const node *n) const
    {
        if (n->left  == &nil_node || n->right == &nil_node)
            return true; 
        return false;
    };
    const node *sibling(const node *n) const
    {
        if (n->parent->left == n)
            return n->parent->right;
        return n->parent->left;
    };
    void delete_case3(node *n)
    {
       node * s = const_cast<node*>(sibling(n)); 
       if (s->left->color == BLACK && s->right->color == BLACK)
       {
           s->color = RED;
           delete_case1(n->parent);
       }
       else
           delete_case4(n); 
    };
    void delete_case4(node *n)
    {
        node *s = const_cast<node*>(sibling(n));
        if (s->parent->color == RED)
        {
            s->color = RED;
            n->parent->color = BLACK;
        }
        else
            delete_case5(n);   
    };
    void delete_case5(node *n)
    {
         node *s = const_cast<node*>(sibling(n));
 
         if  (s->color == BLACK) 
         { 
             if ((n == n->parent->left) &&
                (s->right->color == BLACK) &&
                 (s->left->color == RED)) 
             { /* this last test is trivial too due to cases 2-4. */
                 s->color = RED;
                 s->left->color = BLACK;
                 rotate_right(s);
              }
              else if ((n == n->parent->right) &&
                       (s->left->color == BLACK) &&
                       (s->right->color == RED)) 
              {/* this last test is trivial too due to cases 2-4. */
                   s->color = RED;
                   s->right->color = BLACK;
                   rotate_left(s);
              }
          }
          delete_case6(n);
    };
    void delete_case6(node *n)
    {
         node *s = const_cast<node*>(sibling(n));
         s->color = n->parent->color;
         n->parent->color = BLACK;
 
         if (n == n->parent->left) 
         {
             s->right->color = BLACK;
             rotate_left(n->parent);
         } else 
         {
             s->left->color = BLACK;
             rotate_right(n->parent);
         }
    };
    void delete_case2(node *n)
    {
        node *s = const_cast<node*>(sibling(n)); 
        if (s == &nil_node)
            return;  
        if (s->color == RED)
        {
            n->parent->color = RED;
            
            if (n==n->parent->left)
               rotate_left(n->parent);
            else
               rotate_right(n->parent);
        }
        delete_case3(n);
    };
    void delete_case1(node *n)
    {
       if (n->parent != NULL)
           delete_case2(n); 
    };
    void delete_one_child(node *n)
    {
        bool l = n->left != &nil_node? true: false; 
        node *c = l? n->left: n->right;
        replace(n, c);
        if (n->color == BLACK)
            if(c->color == RED)
                c->color = BLACK;
            else
                delete_case1(n);
        if(l) 
            n->left = &nil_node;
        else
            n->right = &nil_node;
        delete c;
        
    };
    bool remove(int key)
    {
        node *n =  const_cast<node *>(find_node(key));
        if (n == NULL || n == &nil_node)
            return false; 
        if (!is_leaf(n))
        {
            node *succ = const_cast<node*>(successor(n));
            replace(n, succ);
            n = succ;
        }
        delete_one_child(n);
        return true;
    }
    int min(void) const throw (int)
    {
        if (root != NULL)
            return root->key;
        throw -1;
    };
    int max(void) const throw (int)
    {
        if (root == NULL)
            throw -1;
    };
    void remove_min(void)
    {
    };
    const node *find(int key) const
    {
    };
    bool remove(const node *n) 
    {
    };
    bool decrease_key(node * n, int new_key)
    {
    };
    ~rb_tree(void)
    {
       delete_tree(root); 
    };
private:
    node *root;
    static node nil_node; 
};


int main()
{
    return 0;
}
