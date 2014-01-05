#ifndef __EDGE_H__
#define __EDGE_H__
/* Edge class defines link between two neighboring vertices.
   It contains three memeber variables, namely e, o, w 
   respectively. 
   The Edge class serves both directed and 
   undirected graph. For directed graph, e should be the starting
   vertex while o should be the ending vertext. For undirected 
   graph, there is no such distinction.

   Note taht To describe weightless graph, the w field can be omitted.
*/ 

#include <stdint.h>
class edge
{
   const  size_t e;/*one endpoint */
   const  size_t o;/* the other endpoint */ 
   const  int w; /* weight */
public:
   edge (size_t ep1, size_t ep2, int wgt = 0): e(ep1), o(ep2), w(wgt) {};
   inline size_t E(void) const { return e; };
   inline size_t O(void) const { return o; };
   inline int W(void) const { return w; };

};


#endif
