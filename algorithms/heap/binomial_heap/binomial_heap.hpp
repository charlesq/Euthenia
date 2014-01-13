#ifndef __BINOMIAL_HEAP_H__
#define __BINOMIAL_HEAP_H__
#include <algorithm>
#include <vector>
#include <memory>
#include <stddef.h> 
#include <stdint.h>
#include <iostream>
#include <binomial_node.hpp>

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

#endif
