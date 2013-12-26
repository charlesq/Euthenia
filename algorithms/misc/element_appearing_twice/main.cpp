#include <vector>
#include <iostream>

int FindTwiceElement(std::vector<int> &v)
{
        int ones = 0, twos = 0, notThrees = 0;
        for(int &a : v)
        {
                twos |= ones & a;
                ones ^= a;
                notThrees = ~(ones & twos);
                ones &= notThrees;
                twos &= notThrees;
        }
        return twos;
}

int main(int argc, char* argv[])
{
        int a[] = {1, 3, 5, 11, 11, 5, 1 , 11, 3, 5, 1};
        std::vector<int> v(a, a + 11);
        int x = FindTwiceElement(v);
        std::cout << "the number appears twice is " << x  << std::endl;
        v.clear();
        return 0;
}

