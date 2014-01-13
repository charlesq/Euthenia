#include <trie.hpp>
#include <iostream>
int main()
{
    trie pt;
    std::unique_ptr<const char > lcp = pt.find_longest_common_prefix(4, "Wellsfargo", "well", "wellington", "wellS");
    std::cout << lcp.get() << std::endl;
    return 0;
}
