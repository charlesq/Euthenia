#include <cache_imp.hpp>
#include <stdlib.h>
#include <memory>
#include <iostream>
using namespace std;
class int_generator
{
public:
    int operator()(void)
    {
        return rand();
    };
    int_generator(void)
    {
        srand(time(NULL));
    };


};
int main(int argc, char *argv[])
{
   shared_ptr<evict_policy<int>> e(new evict_policy<int>);
   shared_ptr<backstore_proxy<int, int, int_generator>> c ( new backstore_proxy<int, int , int_generator>) ; 
   shared_ptr<cache_imp<int, int, int_generator >>  p (new cache_imp<int, int, int_generator>( e, c, 3)); 
   cout << "*****************************************" << endl;
   cout << "CACHE EASTABLISHED WITH SIZE 3" << endl; 
   cout << "*****************************************" << endl;

   int tag = 12;
   cout << " to read from tag " << tag << " .... " <<endl;
   int v = p->read_datum(tag);
   cout << "value at " << tag <<  " is " << v << endl; 
   v = -20;
   cout << " to write " << v << " to tag " << tag << endl;
   p->write_datum(tag, v);


   tag = 15;
   cout << " to read from tag " << tag << " .... " <<endl;
   v = p->read_datum(tag);
   cout << "value at " << tag <<  " is " << v << endl;

   tag = 44;
   cout << " to read from tag " << tag << " .... " <<endl;
   v = p->read_datum(tag);
   cout << "value at " << tag <<  " is " << v << endl;

   tag = 50 ;
   cout << " to read from tag " << tag << " .... " <<endl;
   v = p->read_datum(tag);
   cout << "value at " << tag <<  " is " << v << endl;

  
   tag = 15;
   cout << " to read from tag " << tag << " .... " <<endl;
   v = p->read_datum(tag);
   cout << "value at " << tag <<  " is " << v << endl;
 
   tag = 44;
   v = -50;
   cout << " to write " << v << " to tag " << tag << endl;
   p->write_datum(tag, v);
  

   return 0;
}
