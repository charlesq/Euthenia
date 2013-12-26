#include <map>
#include <vector>
/* bt root */
unsigned int bounded_knapsack(std::vector<unsigned int> &item_inventory, std::vector<unsigned int> &weight_table,
                              std::vector<unsigned int> &size_table,  unsigned  int i, unsigned int size_limit)
{
    unsigned int max_w = 0, w = 0, limit;
    /* accept scenarion */
    if (i == item_inventory.size())
        return 0;
    for (unsigned int j  =  item_inventory[i]; j <= item_inventory[i] ; j --)
    {
        limit = size_limit;
        /* reject */
        if (j * size_table[i] > size_limit)
           continue;
        /* first candidate extension */
        w = j * weight_table[i];
        /* next candidate extension */
        limit -=  j * size_table[i];
        if (limit != 0)
            w += bounded_knapsack(item_inventory, weight_table, size_table, i + 1, limit);
        /* output */
        if (w > max_w)
            max_w = w;
    } 
    return max_w;
    
}

#include <algorithm>
#include <iostream>
int main()
{
    std::vector<unsigned int> weight_table(4), item_inventory(4), size_table(4);
    unsigned int w[] = {1 , 3, 5, 7};
    unsigned int items [] = {1,  3, 2, 5};
    unsigned int sizes [] = {3, 2, 9, 8}; 
    std::copy(w, w + 4, weight_table.begin());
    std::copy(items, items + 4, item_inventory.begin());
    std::copy(sizes, sizes + 4, size_table.begin());
    unsigned int max_w = bounded_knapsack(item_inventory, weight_table, size_table, 0, 5);
    std::cout<< max_w << std::endl;
    return 0;
}
