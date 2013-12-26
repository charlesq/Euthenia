#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <utility>
using namespace std;
void print_elements(const int *a, unsigned int );
int kthse(int *a, unsigned int size, unsigned int k)
{
    if (k > size || a == NULL || size == 0)
        throw int(-1);
    if(size ==  1 && k == 1)
        return *a; 
    cout <<"\nAt beginning: " << "k = " << k << " " << endl;
    print_elements(a, size);
    int pivot = rand() % size;
    cout << "pivot location = "<< pivot << endl;
    std::swap(*a, *(a + pivot));
    cout << "\nAfter swapping " << endl;
    print_elements(a, size);
    // selection sorting 
    unsigned int i = 0, j = 0; 
    while(j ++ < size -1)
    {
        if (*a < *(a + j))
        {
            continue;
        }
        std::swap( *(a + ++i), *(a + j)) ;
    }
    std::swap(*a, *(a + i));
    cout << "\ni = " << i << endl;
    print_elements(a, size);
    cout << endl;
    if (i == (k -1))
        return *(a + i);
    if (i < k -1)
        return kthse(a + i+1, size - i -1, k - i -1);
    return kthse(a, i, k);
} 
int * generate_array(unsigned int size)
{
    srand(time(NULL));
    if (size == 0)
        return NULL;
    int *a = new int[size];
    for (int *p = a; p < (a + size); p ++)
        *p = rand() % 20000;
    return a;
}

void print_elements(const int *a, unsigned int size)
{
    if (a == NULL)
        return;
    unsigned int m = 0;
    while(size-- != 0)
    {
        cout << " " << *a ++;
        if (++m % 20 == 0)
        cout << endl; 
    }

}

int main()
{
  int *a = NULL;
  unsigned int size = 20;
  while(1)
  {
      a = generate_array(size); 
      if ( a == NULL)
          break;
      unsigned int k = rand() % size;
      int h = kthse(a, size, k);  
      std::sort(a, a + size); 
      print_elements(a, size);
      cout << endl << k << "th smallest element is " << h << endl;
      delete [] a;
      char ret;
      cout << "Continue (Y/N)?: ";
      cin >> ret; 
      if (ret == 'y' || ret == 'Y')
          continue; 
      break;
 } 
}
