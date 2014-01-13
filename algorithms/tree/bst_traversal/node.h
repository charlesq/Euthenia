#ifndef __NODE_H__
#define __NODE_H__
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
void print_node(const struct _node<int> *n);
struct _node <int> *build_tree(int h, int l);
void release_tree(const struct _node <int> *r);
#endif









