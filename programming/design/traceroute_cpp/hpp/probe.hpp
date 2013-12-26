#ifndef __PROBE_HPP__
#define __PROBE_HPP__
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>
#include <any_addr.h>
#include <unistd.h>
#include <stddef.h>
#include <spoll.hpp>
using namespace std;
class probe
{
public:
    int done;
    int final;
    any_addr res;
    double send_time;
    double recv_time;
    int send_ttl;
    int recv_ttl;
    int sk;
    int seq;
    char *ext;
    char err_str[16];	/*  assume enough   */ 

    static probe *create_probe(void)
    {
        probe * p = new probe();
        p->final = 0;
        p->done = 0;
        p->recv_time = 0;
        p->send_ttl = 0;
        probes.push_back(p);
        return p;
    };
    static void remove_probes(void)
    {
        while(probes.size() > 0)
        {
           delete probes.back();
            probes.pop_back();
        };
    };
    static probe * get_by_sock(int s)
    {
        for (auto it = probes.begin(); it != probes.end(); it ++)
        {
            if ((*it)->sk == s)
            return *it;
        }    
        return NULL;
    };
    static probe *get_by_seq(int n)
    {
        for (auto it = probes.begin(); it != probes.end(); it ++)
        {
            if ((*it)->seq == n)
                return *it;
        }
        return NULL;
    };
    static void probe_done (probe *pb, spoll *sp) 
    {
#if 1 

        if (pb->sk != -1) {
            sp->cleanup_polls();
            close (pb->sk);
           pb->sk = -1;
        }
        pb->seq = 0;
#endif
        pb->done = 1;
    };
    ~probe(void)
    {
        if( ext != NULL)
            delete [] ext;
    };
    static const vector<probe *> & get_probes(void )
    {
        return probes;
    
    };

private:
   probe(void): ext(NULL){}; 
   static vector<probe *> probes;
};


#endif
