#ifndef __EDGE_H__
#define __EDGE_H__
class edge
{
    const unsigned int from;
    const unsigned int to;
    const int wt;
public:
    edge (unsigned int f, unsigned int t, int w):from(f), to(t), wt(w) {};
    unsigned int f(void) const { return from; };
    unsigned int t(void) const { return to; };
    int w(void) const {return wt; };
};
#endif
