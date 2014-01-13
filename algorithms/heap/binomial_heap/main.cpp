#include <binomial_heap.hpp>
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

