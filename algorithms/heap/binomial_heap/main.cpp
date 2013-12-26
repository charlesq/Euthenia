#include <algorithm>
#include <vector>
#include <memory>
#include <stddef.h> 
#include <stdint.h>
#include <iostream>
namespace detail 
{
    template <typename T> struct binomial_node;
    template <typename T>
    std::shared_ptr<struct binomial_node<T>> clone_binomial_tree(std::shared_ptr<struct binomial_node<T>> root);
    template <typename T>
    void merge_binomial_heap(std::vector<std::shared_ptr<struct binomial_node<T>>> &a,
                             std::vector<std::shared_ptr<struct binomial_node<T>>> &b);
    template <typename T>
    void release_node(std::shared_ptr<struct binomial_node<T> > &n);
    template <typename T>
    class compare
    {
        typedef struct binomial_node <T> node;
    public:
        bool operator () (std::shared_ptr<node> a, std::shared_ptr<node> b) const
        {
            if (b.use_count() == 0)
                return true;
            if (a.use_count() == 0)
                return false;
            
            return a->value < b->value;
        };
    };

};

template <typename T>
class binomial_heap
{
    typedef detail::binomial_node<T> node;
    std::vector<std::shared_ptr<node>> mtrees;
    std::weak_ptr<node> min_node; 
    size_t msize;
    void swap(binomial_heap &other)
    {
        std::swap(other.msize, msize);
        std::swap(mtrees, other.mmtrees); 
        min_node = other.min_node;
    };    
    void merge(binomial_heap &o)
    {
        if (o.size() == 0)
            return;
        if (msize == 0)
        {
            swap(o);
            return;
        }
        msize += o.size();
        if (c(o.min_node.lock(), min_node.lock()))
            min_node = o.top();
        detail::merge_binomial_heap(mtrees, o.mtrees);
        o.mtrees.clear();
        o.msize = 0;
    };

public:
    binomial_heap(void):min_node (std::shared_ptr<node> ()), msize(0){};
    binomial_heap(const binomial_heap &bh):msize(bh.msize)
    { 
         for (size_t i = 0; i < bh.mtrees.size(); i ++)
         {
            mtrees.push_back(detail::clone_binomial_tree<T>(bh.mtress[i]));
            if (min_node && bh.mtrees[i].get() == bh.min_node.lock().get())
            {
                min_node = mtrees[i]; 
            }
         }
    };
    inline size_t size(void) const { return msize; };
    inline T&   top() {return min_node.lock()->value;}; 
    binomial_heap & operator = (binomial_heap &o)
    {
        binomial_heap bh(o);
        swap(bh);
        return *this;
    };
    void reset(void)
    {
        binomial_heap bh;
        swap(bh);
    };
    void push(const T & v)
    {
       std::shared_ptr<node> n(new node(v)); 
       if (msize ++ == 0)
       {
           mtrees.push_back(n);
           min_node = std::weak_ptr<node>(n);
           return;
       }
       std::vector<std::shared_ptr<node>> vs;
       vs.push_back(n); 
       detail::merge_binomial_heap(mtrees,vs);
       detail::compare<T> c;
       if (c(n, min_node.lock()))
       {
           min_node = std::weak_ptr<node>(n);
       }
 
    };
    inline bool empty(void) const { return msize == 0; };
    void pop(void)
    {
        size_t  order; 
        detail::compare<T> c;
        for (order = 0; order < mtrees.size(); order ++)
        {
            if (mtrees[order].get() == min_node.lock().get())
            {
                break;
            }
        } 
        if (order == mtrees.size())
            return;
        std::vector<std::shared_ptr<node>> trees;
        std::shared_ptr<node> n = mtrees[order]->mChild; 
        while(n)
        {
           trees.push_back(n); 
           n = n->mRight;
        }
        mtrees[order].reset();
        detail::merge_binomial_heap(mtrees, trees); 
        std::shared_ptr<node> m = mtrees[0];;
        for (auto &e: mtrees)
        {
            if (c(e, m))
                m = e;
        }
        min_node = m;
        --msize;
        
    };

    ~binomial_heap()
    {
      for (auto &e: mtrees)
          detail::release_node(e);
       mtrees.clear(); 
    };
};

namespace detail
{
    template <typename T> struct binomial_node
    {
        std::shared_ptr<struct binomial_node > mChild;
        std::shared_ptr<struct binomial_node >mRight;
        T value;
        binomial_node(T v, std::shared_ptr<struct binomial_node> l = std::shared_ptr<struct binomial_node> (), std::shared_ptr<struct binomial_node> r = std::shared_ptr<struct binomial_node>() )
        {
            value = v;
            mChild = l;
            mRight = r; 
        };
    };
    template <typename T>
    std::shared_ptr<struct binomial_node<T>>  merge_trees(std::shared_ptr<struct binomial_node<T>> &a, 
                                                          std::shared_ptr<struct binomial_node<T>> &b)
    {
        if (compare<T>()(b, a))
        {
            std::swap(a, b);
        }
        b->mRight = a->mChild;
        a->mChild = b;
        return a;
    } 
    template <typename T>
    void merge_binomial_heap(std::vector<std::shared_ptr<struct binomial_node<T>>> &a, 
                             std::vector<std::shared_ptr<struct binomial_node<T>>> &b)
    {
        std::vector<std::shared_ptr<struct binomial_node<T>>> result;
        std::shared_ptr<struct binomial_node<T>> carry;
        if (a.size() > b.size())
            b.resize(a.size());
        else
           a.resize(b.size());
        if (a.size() == 0)
            return;
        for (size_t i = 0; i < a.size(); i ++)
        {
            std::vector<std::shared_ptr<struct binomial_node<T>>> trees;
            if (false || carry)
                trees.push_back(carry);
            if (false || a[i])
                trees.push_back(a[i]);
            if (false || b[i])
                trees.push_back(b[i]);
            switch(trees.size())
            {
            case 0:
                result.push_back(std::shared_ptr<struct binomial_node<T>>());
                break;
            case 2:
                result.push_back(std::shared_ptr<struct binomial_node<T>>());
                carry = merge_trees(trees[0], trees[1]);
                break;
            case 1:
                 result.push_back(trees.front());
                 carry.reset();
                break;
            case 3:
                 result.push_back(trees.front());
                 carry = merge_trees(trees[1], trees[2]);
            }
            trees.clear();
        }
        if (carry)
            result.push_back(carry);
        a.clear();
        a.resize(result.size());     
        std::copy(result.cbegin(), result.cend(), a.begin());
        result.clear();
        b.clear();
    }
    template <typename T>
    void release_node(std::shared_ptr<struct binomial_node<T>> &n)
    {
          if (n.use_count() == 0)
              return;
          auto &sibling = n->mRight;
          while(sibling)
          {
              auto &s = sibling->mRight;
              release_node(sibling);
              sibling = s; 
          }
          release_node(n->mChild);
          n.reset();
    }
    template <typename T>
    std::shared_ptr<struct binomial_node<T>> clone_binomial_tree(std::shared_ptr<struct binomial_node<T>> root)
    {
       typedef struct binomial_node<T> node;
       std::shared_ptr<struct binomial_node<T>> clone; 
       if (root.use_reference() == 0)
           return clone; 
       return std::shared_ptr<node>(new struct binomial_node<T> (root->value, clone_binomial_tree(root->mChild), 
                                                                              clone_binomial_tree(root->mRight)));   
    }
}
int main()
{
    binomial_heap<int> bh;
    int k = 4;
    bh.push(k);
    k = 3;
    bh.push(k);
    bh.push(-1);
    k =  bh.top();
    bh.pop();
    k = bh.top();
    bh.pop();
    k = bh.top();
    bh.pop();
    if ( bh.empty())
       std::cout << "empty!" << std::endl; 
    return 0;
}
