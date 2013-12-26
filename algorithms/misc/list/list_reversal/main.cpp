#include <iostream>
#include <stddef.h>
typedef struct _node node;
struct _node 
{
    node *n;
    int v;
}; 
void _reverse_slist_rec(node **nh, node **oh)
{
    if (*oh == NULL)
      return; 
    node *n = (*oh)->n; 
    if (*nh == NULL)
    {
        *nh = *oh;
        (*nh)->n = NULL;
    }
    else
    {
        (*oh)->n = *nh;
        *nh = *oh;
    }
    *oh = n; 
    _reverse_slist_rec(nh, oh);
}
void reverse_slist_rec(node **head)
{
    node *nh = NULL;
    _reverse_slist_rec(&nh, head);
    *head = nh;
}
void reverse_slist(node **head)
{
    node * nh = NULL, *n;
    while (*head != NULL)
    {
        n = (*head)->n;
        if (nh == NULL)
        {
            nh = *head;
            nh->n = NULL;
        }
        else
        {
            (*head)->n = nh;
            nh = *head;
        }
        *head = n;
    }
    *head = nh;
}

void print_node(const node *n)
{
    std::cout << n->v << " ";
}
void print_slist(const node *root)
{
    while(root != NULL)
    {
       print_node(root);
       root = root ->n;
    }
    std::cout<< std::endl;
}
void print_slist_rec(const node *n)
{
    if (n == NULL)
    {
        std::cout<< std::endl;
        return;
    }
    print_node(n);
    print_slist_rec(n->n);
}
/* *n must be NULL */
void build_slist(node **n, int v)
{
   node *p;
   while(v != 0)
   {
      p = new node;
      p->v = v--;
      p->n = NULL;
      if (*n == NULL)
      {
          *n = p; 
      } 
      else
      {
          p->n = *n;
          *n = p;
      }
   } 
}
void free_slist(const node *n)
{
    const node *next;
    while(n != NULL)
    {
        next = n ->n;
        delete n;
        n = next;
    }
}
typedef struct _bnode bnode;
struct _bnode
{
    bnode *next;
    bnode *prev;
    int v;
};

void reverse_dlist(bnode **h)
{
    bnode *nh = NULL, *n;
    while (*h != NULL)
    {
        n = (*h)->next;
        if (nh == NULL)
        {
            nh = *h;
            nh->next = NULL;
            nh->prev = NULL;
        }
        else
        {
           (*h)->next =  nh;
           (*h)->prev = NULL;
            nh -> prev = *h;
            nh = *h;
        }
        *h = n;
        if (n != NULL)
        n->prev = NULL;
    }
    *h = nh;
}
void _reverse_dlist_rec(bnode **nh, bnode **oh)
{
   if ((*oh) == NULL)
       return;
   bnode *cur = (*oh)->next;
   if (cur != NULL)
       cur->prev = NULL;
   if ((*nh) == NULL)
   {
       *nh = *oh; 
       (*nh)->prev = NULL;
       (*nh)->next = NULL;
   }
   else
   {
       (*oh)->next  = *nh;
       (*oh)->prev = NULL;
       (*nh)->prev = *oh; 
       (*nh) = (*oh);
    }
   (*oh) = cur; 
   _reverse_dlist_rec(nh, oh);
}
void reverse_dlist_rec(bnode **h)
{
    bnode *nh = NULL;
    _reverse_dlist_rec(&nh, h);
    *h = nh;
} 
void print_bnode(const bnode *n)
{
    std::cout << n->v << " ";
}
void print_dlist(const bnode *n)
{
    while(n != NULL)
    {
        print_bnode(n);
        n = n->next;
    } 
    std::cout << std::endl;
}
void print_dlist_rec(const bnode *n)
{
    if (n == NULL)
    {
        std::cout << std::endl;
        return;
    }
    print_bnode(n);
    print_dlist_rec(n->next); 
}
void build_dlist(bnode **h, int v)
{
   bnode *n; 
   while(v != 0 )
   {
       n = new bnode; 
       n ->v = v--;
       if (*h == NULL)
       {
           *h = n;
           (*h)->next = n;
           (*h)->prev = n;
       }
       else
       {
           n->next = *h;
           n->prev = (*h)->prev;
           (*h)->prev->next = n; 
           (*h)->prev = n;
       }
   }
   (*h)->prev ->next = NULL;
   (*h)->prev = NULL;
   
}
void free_dlist_rec(const bnode *h)
{
   if (h == NULL)
       return;
   const bnode *next = h->next;
   delete h;
   free_dlist_rec(next);
    
}

int main()
{
     node *r = NULL;
     build_slist(&r, 10);
     print_slist(r);
     reverse_slist(&r);
     print_slist(r);
     reverse_slist_rec(&r);
     print_slist_rec(r);
     free_slist(r);
     bnode *br = NULL;
     build_dlist(&br, 20);
     print_dlist(br);
     reverse_dlist(&br);
     print_dlist(br);
     reverse_dlist_rec(&br);
     print_dlist_rec(br);
     free_dlist_rec(br);  
    return 0;
}
