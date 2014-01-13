#include <sorting.hpp>
int main()
{
    sorting instance(20);
    instance.runQuicksort();

    instance.runHeapsort();
    instance.runBubblesort();
    instance.runMergesort();
    instance.runRadixsort();
    instance.runInsertsort();

    return 0;
}
    
