#ifndef __ABSTRACT_CACHE_HPP__
#define __ABSTRACT_CACHE_HPP__
#include <unistd.h>
#include <time.h>
#include <sched.h>
#include <assert.h>
#include <string>
using namespace std;
typedef enum cache_bit { eModified, eInvalidated, eReferenced} cache_flag;  



template<class tag, class T>
class abstract_cache
{
    // size_t cacheline; /*in bytes */
    // int way; 
public:
    unsigned int reference_total;
    unsigned int hit;
    unsigned int read_latency; // in millisecond
    unsigned int write_latency;
    unsigned int evict_latency;

public:

public:
    abstract_cache(void)
    :hit(0), reference_total(0), read_latency(0),
     write_latency(0), evict_latency(0) {};
    double get_hit_rate(void) 
    {
        read_latency ++;
        return (reference_total ==0)? 1: static_cast<double>(hit)/reference_total; 
    };
protected:
    /* default behavior throws an exception*/
    virtual unsigned int compute_cache_index(const tag &r) const throw (string) {throw (string("not implemented"));};
};

#endif

