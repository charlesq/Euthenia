#ifndef __TRACEROUTE_HPP__
#define __TRACEROUTE_HPP__
#include <memory>
#include <message.h>
#include <deque>
#include <queue>
#include <traceroute_options.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sock_eng.hpp>
#include <tcp_eng.hpp>
#include <probe.hpp>
#include <spoll.hpp>
#include <udp_eng.hpp>
using namespace std;
vector<probe *> probe::probes;

unique_ptr<sock_eng> create_sock_eng(const char *hn, const traceroute_options *to, spoll *p)
{
   sock_eng * e = NULL;
   if (to->trace_method == NULL)
      e = new default_udp_eng(to); 

   else if (strcmp(to->trace_method, "tcp") == 0)
       e = new tcp_eng(to);
   else if (strcmp(to->trace_method, "udp") == 0)
       e = new udp_eng(to);
   else if (strcmp(to->trace_method, "udplite") == 0)
       e = new udp_lite_eng(to);
       
   if (e!= NULL)
        e->init(hn, p);
    return unique_ptr<sock_eng>(e);
}

class traceroute
{
    string hostname;
    const traceroute_options &to;
    spoll  sp;
    unique_ptr<sock_eng> se;
    queue<const struct message *,deque<const struct  message*> > message_queue;  
    unsigned int num_probes;
    traceroute(const char *n, traceroute_options &o):hostname(n), to(o){};
    void print_addr(const any_addr *a) const
    {
        if (!a->sa.sa_family)
            return;
        std::string str = sock_lib::addr2str(a);
        if (to.resolve_name > 0)
        {
            char buf[1024];
            buf[0] = '\0';
            getnameinfo(&a->sa, sizeof(any_addr), buf, sizeof(buf), 0, 0, NI_IDN);
            printf(" %s (%s)", buf[0]?buf:str.c_str(), str.c_str());
        } 
        else
            printf("%s", str.c_str());
    };
    void print_header(void) const
    {
        printf( "traceroute to %s (%s), %u hops max, %zu byte packets\n",
               hostname.c_str(), sock_lib::addr2str(&se->get_dest_addr()).c_str(), to.ip_max_ttl, se->get_data_len() + se->get_header_len());
 
    };
public:
    static unique_ptr<traceroute> get_instance(const char *name, traceroute_options& tr)
    {
        if (tr.ip_mtu_disc == 1)
        {
            tr.squeries = 1;
            tr.ip_no_frag = 1;
            if (tr.ip_packet_len == 0)
                tr.ip_packet_len = MAX_PACKET_LEN;
        }
        auto u =  unique_ptr<traceroute>(new traceroute(name, tr));
        return u;  
    };
    void init(void)
    {
        se = create_sock_eng(hostname.c_str(), &to, &sp);
        num_probes = to.nqueries * to.ip_max_ttl;

    };
    bool print_probe(probe *pb) const
    {
        if (pb == NULL)
            return false;
         printf("\n%2u ", pb->send_ttl);
         print_addr(&pb->res);
    //     printf("<%s>", pb->ext);
#if 0

        const vector<probe *> ps = probe::get_probes();
        any_addr pa;  
        pa.sa.sa_family = 0;
        bool hit = false;
        
        for (auto x =  ps.cbegin(); x != ps.cend(); x ++)
        {
            if ((*x)->send_ttl != pb->send_ttl)
            {
                if (hit) break;
                continue;
            }
            hit = true;
            if ((*x)->res.sa.sa_family)
            {
                printf(" *"); 
                continue;
            }
            if (sock_lib::equal_addr(&pa, &((*x)->res)))
            {
                if (pb->ext != (*x)->ext && 
                       !(pb->ext && (*x)->ext && 
                         strcmp(pb->ext, (*x)->ext)
                         )
                     )
                    
                     
                 {
                     print_addr(&(*x)->res);
                     printf("<%s>", (*x)->ext);
                 }
            }
            else
            {
                if (pa.sa.sa_family == 0)
                    pa = (*x)->res;
                print_addr(&(*x)->res);
            }
            if ( to.backward_hops && pb->recv_ttl != (*x)->recv_ttl)
            {
                int hops = ttl2hops((*x)->recv_ttl);
                if (hops != pb->send_ttl)
                    printf(" '-%d'", hops);
            }
#endif
            printf("  %.3f ms", (pb->recv_time - pb->send_time) * 1000);  
            if (pb->recv_time != 0)
               return true;
            return false;
#if 1 
            if (pb->err_str[0])
               printf("  %s", pb->err_str);
#endif

            fflush(stdout);
    };
    void do_work(void) 
    {
        poll_result pr;
        print_header();
        fflush(stdout);
        int attemps = (to.ip_first_ttl == 0)? 0: (to.ip_first_ttl - 1) * to.nqueries;
        int end = to.ip_max_ttl * to.nqueries;
        probe *p = NULL;
        double last_send, current_time;  
        last_send = current_time = 0;
        bool _exit = false; 
        while (attemps < end && !_exit)
        {
            int num = 0;
            current_time = sock_lib::get_time();
         
            if(num < to.squeries)
            {
                if (last_send == 0 || current_time - last_send >= to.send_wait)
                {
                    last_send = current_time;
                    num ++;
                    attemps ++;
                    se->send_probe(attemps / to.nqueries);
                }
       
            }
            
            pr = sp.do_poll(to.waittime * 1000);
            if (pr.size() == 0)
            {
                check_expired();
                continue;
            }
 
            for (auto pp: pr)
            {
#if 1 
               if ((pp.second & (POLLIN |  POLLERR)) == 0)
               {
                   check_expired();
                   continue;
               }
#endif
                p = se->recv_reply(pp.first, pp.second);
              //  if (attemps % (to.nqueries -1) == 0)
                {
                   if (print_probe(p)) 
                   {
                      attemps += ((attemps % to.nqueries  +  to.nqueries - 1) / to.nqueries) * to.nqueries;
                   }
                    printf("\n");
                }
                _exit = check_expired();
            }
            num = 0; 
         }

    };
    bool check_expired(void)
    {
        const vector<probe*> ps = probe::get_probes();
        for (auto it = ps.cbegin(); it != ps.cend(); it++)
        {
            if ((*it)->final)
                return true;
            if ((*it)->sk != -1)
            {
                double t = sock_lib::get_time();
                if (t - (*it)->send_time > 5)
                    probe::probe_done(*it, &sp);
            } 
             
        } 
        return false;
   };
};

#endif
