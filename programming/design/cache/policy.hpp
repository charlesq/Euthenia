#ifndef __POLICY_HPP__
#define __POLICY_HPP__
#include <vector>
#include <utility>
#include <sys/time.h>
#include <unistd.h>
#include <abstract_cache.hpp>
#define REF_MAX -1
using namespace std;
template <class T>
class evict_policy
{
     vector<pair<T, unsigned long long>> ref;  
     unsigned long long time_base_us;
     unsigned int cache_size;
public:
    evict_policy(void)
    {
       cache_size = 0;
       struct timeval tv;
       gettimeofday(&tv, NULL);
       ref.resize(cache_size, make_pair(T(0), 0));

       time_base_us = tv.tv_usec + tv.tv_sec * 1000000;
    };
    void set_cache_size(unsigned int s)
    {
        cache_size = s;
       
        ref.resize(cache_size, make_pair(T(0), 0));
    };
    unsigned int get_index_to_evict(void)
    {
        unsigned int k = cache_size;
        unsigned long long least = REF_MAX;
        for (unsigned int i = 0; i < cache_size; i++)
        {
            if (least > ref[i].second)
            {
                k = i;
                least = ref[i].second;
            }
        }
        return k;
    };



    void mark_reference(const T& t , cache_flag f)
    {
        unsigned int k = cache_size;
        unsigned long long least = REF_MAX;
        for (unsigned int i = 0; i < cache_size; i++)
        {
            /*scenario one: there is an any entry for t */
            if (ref[i].first == t && f!= eInvalidated)
            {
                /* the flag is not eInvalidated */   
                 ref[i].second = get_timestamp(); 
                 return;
            }
            if (ref[i].first == t)
            {
                ref[i].first = 0;
                ref[i].second = 0;
                return;
            }
            if (ref[i].first == 0)
            {
                ref[i].first = t;
                ref[i].second = get_timestamp();
                return;
            }
            if (least > ref[i].second)
            {
               k = i; 
               least = ref[i].second;
            } 
        }
        /* scenarion two: all entries have been occupied */ 
        if (f == eInvalidated) /* already eInvalidated, do nothing */
            return;
        /* evict the least recently referenced entry */
        ref[k].first = t;
        ref[k].second = get_timestamp();
        return;
        
    };
    unsigned long long get_timestamp(void)
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_usec + tv.tv_sec * 1000000 - time_base_us; 
    }; 
};
#endif
