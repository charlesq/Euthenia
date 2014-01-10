#ifndef __CACHE_IMP_HPP_

#define __CACHE_IMP_HPP__
#include <mutex>
#include <abstract_cache.hpp>
#include <backstore.hpp>
#include <policy.hpp>
#include <delay.h>
#include <memory>
#include <iostream>
using namespace std;


template <class tag, class T, class G>
class cache_imp: public abstract_cache<tag, T>
{
    template <class _tag,  class _T>
    struct cache_line
    {
        _tag t;
        _T v;
        cache_flag flag;
    };
    using abstract_cache<tag, T>::reference_total;
    using abstract_cache<tag, T>::hit;
    using abstract_cache<tag, T>::read_latency;
    using abstract_cache<tag, T>::write_latency;
    using abstract_cache<tag, T>::evict_latency;
    vector<struct cache_line<tag, T>>cache;
    shared_ptr<backstore_proxy<tag, T,G>>  bs;
    shared_ptr<evict_policy<tag>> policy; 
    unsigned int cache_size;
public:
    std::mutex mtx;
public:
    cache_imp(shared_ptr<evict_policy<T>> p, shared_ptr<backstore_proxy <tag, T,G >>b, unsigned int size)
    :policy(p), bs(b)
    {
        cache_size = size;
        cache.resize(cache_size, {tag(0), T(0), eInvalidated});
        policy->set_cache_size(cache_size);
    };
    T read_datum(const tag & a)
    {
        mtx.lock(); 
        reference_total ++;
        int k = cache_size;
        for (auto i = 0; i < cache_size; i ++)
        {
            if (cache[i].t == a && cache[i].flag != eInvalidated)
            {/* a cach hit! */
                hit++;
                policy->mark_reference(cache[i].t, eReferenced);
                delay(read_latency);
                mtx.unlock();
                cout << "      a cache read hit " << endl;
                return cache[i].v;
            }
            if (k == cache_size && cache[i].flag == eInvalidated)
                k = i; 
        }

        /* a cache miss!!! */ 
        if (k != cache_size)
        {
            cache[k].t = a;
            cache[k].flag = eReferenced;
            cache[k].v = bs->from_backstore(cache[k].t);
            policy->mark_reference(cache[k].t, eReferenced);
            delay(write_latency);
            mtx.unlock();
            cout << "      a cache read miss (no eviction), fetched from backstore and loaded to cache" << endl; 
            return cache[k].v;
         }
         //scenario two: no vacancy, need to find a line to evict
         unsigned int index = policy->get_index_to_evict();  
         cout << "      a cache read miss, evicted " << cache[index].v << " at " << cache[index].t << endl; 
         policy->mark_reference(cache[index].t, eInvalidated); 
         delay(evict_latency);
         cache[index].t = a;
         cache[index].v = bs->from_backstore(a); 
         cache[index].flag = eReferenced;
         policy->mark_reference(a, eReferenced);
         delay(write_latency);
         mtx.unlock();
         cout << "      (fetched from backstore and loaded  to cache)" << endl;

         return cache[index].v;
        
    };
    void write_datum(const tag &a, T & value)
    {
        mtx.lock();
        for (auto &e: cache)
        {
            if (e.t == a)
            {
               bs->to_backstore(a, value); 
               policy->mark_reference(a, eModified); 
               delay(read_latency);
               e.v = value;
               delay(write_latency);
               mtx.unlock();
               cout << "      a cache write hit ( updated cache and wrote through to backstore)" << endl;
               return;
            }
        }
        delay(read_latency);
        unsigned int index = policy->get_index_to_evict();
        cout << "     a cache write miss, evicted " << cache[index].v << " at " << cache[index].t ;  
        cache[index].t = a;
        cache[index].v = value;
        cache[index].flag = eReferenced;
        policy->mark_reference(a, eModified);
        bs->to_backstore(a, value);
        delay(write_latency);
        mtx.unlock();
        cout << "      (wrote through to backstore and loaded to cache)"<< endl;
        return;
    };
    void set_latency(unsigned int rl, unsigned int wl)
    {
        write_latency = wl;
        read_latency = rl;
    };

    ~cache_imp(void)
    {
    };
private:

};


#endif
