
#ifndef __BACKSTORE_HPP__
#define __BACKSTORE_HPP__
#include <delay.h>
#include <unordered_map>

using namespace std;
template<class T>
class generator
{
public:
    T operator()(void)
    {
        return T(0);
    };
};
template <class tag, class T, class G>
class backstore_proxy
{
    unsigned int latency;
    unordered_map<tag, T> main_store;
    G g;
   
public:
    T from_backstore(const tag &t)
    { 
        delay(latency); 
        for (auto it = main_store.begin(); it != main_store.end(); it++)
        {
            if (it->first == t)
            return it->second;
        }
        main_store[t] = g();
        return main_store[t];
    };
    void to_backstore(const tag &t, T &v) 
    {
        main_store[t] = v; 
        delay(latency); 
    };
    backstore_proxy(void):latency(0){};
    void set_latency(unsigned int ms)
    {
        latency = ms;
    };
};

#endif
