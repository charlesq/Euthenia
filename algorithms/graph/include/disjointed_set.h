#ifndef __DISJOINED_SET_H__
#define __DISJOINED_SET_H__
#include <stdint.h>
#include <stddef.h>
/* disjointed_set will be used in kruskal minimal spanning tree algorithm
   to quickly identify whether vertices of an edge has already been 
   added to mst  vertices set. 

   The three friend functions are crucial to it usage.
*/
class disjointed_set;
/* APIs to operate on disjointed_set */
disjointed_set *make_union(disjointed_set *ds1, disjointed_set *ds2);
disjointed_set *make_set(size_t e);
const disjointed_set *find_set(const disjointed_set *ds);

class disjointed_set
{
    disjointed_set* ancestor;
    const size_t v;
    /* no public constructor */
    disjointed_set(size_t e):ancestor(NULL), v(e) {};
    friend disjointed_set *make_set(size_t e)
    {
        return new disjointed_set(e);
    };
    friend disjointed_set *make_union(disjointed_set *ds1, disjointed_set *ds2)
    {
        if (ds1 == NULL || ds2 == NULL)
            return ds1 == NULL? ds2: ds1;
        if (ds1 == ds2)
            return ds1;
        disjointed_set *an1 = const_cast<disjointed_set *>(find_set(ds1)), 
                      *an2 = const_cast<disjointed_set *> (find_set(ds2));
        disjointed_set *an, *ds;
        if (an1 != an2)
        {
            if (an1 != NULL)
                an = an2->ancestor = an1;
            else
                an = an2->ancestor = an1;
        } 
        /* propagate ancestor infor to all its decendents */
        while(ds1 != an)
        {
            ds = ds1->ancestor;  
            ds1->ancestor = an;
            ds1 = ds;
        }
        while(ds2 != an)
        {
            ds = ds2->ancestor;
            ds2->ancestor = an;
            ds2 = ds;
        }
        
    };
    friend const disjointed_set *find_set(const disjointed_set *ds)
    {
        if (ds == NULL)
            return NULL;
        if (find_set(ds->ancestor) == NULL)
            return ds;
        ds = ds->ancestor;
        return find_set(ds); 
    }; 
};

#endif
