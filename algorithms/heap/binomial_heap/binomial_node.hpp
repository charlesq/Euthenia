#ifndef __BINOMIAL_NODE_H__
#define __BINOMIAL_NODE_H__
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
#endif
