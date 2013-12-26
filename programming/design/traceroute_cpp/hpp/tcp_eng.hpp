#ifndef __TCP_ENG_HPP__
#define __TCP_ENG_HPP__

#include <utility>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <alloca.h>
#include <netinet/ip6.h>
#include <unistd.h>
#include <traceroute_options.h>
#include <memory>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <sock_lib.hpp>
#include <spoll.hpp>
#include <any_addr.h>
#include <probe.hpp>
#include <sock_eng.hpp>
using namespace std;
class tcp_eng: public sock_eng
{
    int mtu;
    int tcp_flags;
    struct tcphdr *th;
    size_t csum_len;
    int dest_port;
    any_addr src;
public:
    void set_flags(void)
    {
         tcp_flags = 0;
         if (o->tcp_ecn)
             tcp_flags |= FL_ECN;
         if (o->tcp_sack)
             tcp_flags |= FL_SACK;
         if (o->tcp_timestamps)
             tcp_flags |= FL_TSTAMP;
         if (o->tcp_window_scaling)
             tcp_flags |= FL_WSCALE; 
         if ((tcp_flags & (FL_FLAGS | 0xff)) == 0)
         {
             tcp_flags |= TH_SYN;
             if(tcp_flags & FL_ECN)
                 tcp_flags |= TH_ECE | TH_CWR;
         }
        // tcp_flags |= (FL_SACK | FL_WSCALE | TH_SYN);
         tcp_flags = 7170;
    };
    probe* recv_reply(int sk, int err)
    {
        return sock_eng::recv_reply(sk,!!( err & POLLERR));
    };
    void expire_probe(probe *pb, spoll *sp)
    {
        probe::probe_done(pb, sp);
    };
    probe *retrieve_probe_rec(int sk, bool err, char * buf, int n)
    {
        
        
        probe *pb;
        struct tcphdr *tcp = (struct tcphdr *) buf;
        u_int16_t sport, dport;
        if (n < 8)  return NULL;    /*  too short   */
	if (err) {
            sport = tcp->source;
            dport = tcp->dest;
        } else 
        {
            sport = tcp->dest;
            dport = tcp->source;
        }
        if (!sock_lib::equal_addr(&dest_addr,&from))
            return NULL;
        if (dport != dest_port)
            return NULL;
        pb = probe::get_by_seq (sport);
        if (!pb)  return NULL;
	if (!err)
        {
	    pb->final = 1;
	    if (o->tcp_reply_info)
                pb->ext = sock_lib::names_by_tcp_flags (TH_FLAGS(tcp));
        }
	return pb;
    };
    size_t generate_data(void)
    {
        size_t len;
        unsigned char *ptr = buf;
        if (af == AF_INET)
        {
            len = sizeof (src.sin.sin_addr);
            memcpy (ptr, &src.sin.sin_addr, len);
            ptr += len;
            memcpy (ptr, &dest_addr.sin.sin_addr, len);
            ptr += len;
        }
        else
        {
            len = sizeof (src.sin6.sin6_addr);
            memcpy (ptr, &src.sin6.sin6_addr, len);
            ptr += len;
            memcpy (ptr, &dest_addr.sin6.sin6_addr, len);
            ptr += len;
        }
        u_int16_t * lenp = reinterpret_cast<u_int16_t *>(ptr);
        ptr += sizeof (u_int16_t);
        *(u_int16_t *)ptr = htons ((u_int16_t) IPPROTO_TCP);
        ptr += sizeof (u_int16_t);
        th = reinterpret_cast<struct tcphdr *> (ptr);
        th->source = 0;	    /*  temporary   */
        th->dest = dest_port;
        th->seq = 0;	    /*  temporary   */
        th->ack_seq = 0;
        th->doff = 0;	    /*  later...  */
        u_int8_t * TH_FLAGS = reinterpret_cast<u_int8_t*>(th) + 13;
        *TH_FLAGS = tcp_flags & 0xff;
        th->window = htons (4 * mtu);
        th->check = 0;
        th->urg_ptr = 0;
	/*  Build TCP options   */
        ptr = (u_int8_t *) (th + 1);
	if (tcp_flags & TH_SYN)
        {
             *ptr++ = TCPOPT_MAXSEG;	/*  2   */
             *ptr++ = TCPOLEN_MAXSEG;	/*  4   */
             *((u_int16_t *) ptr) = htons (o->tcp_mss ? o->tcp_mss : mtu);
             ptr += sizeof (u_int16_t);
        }

        if (tcp_flags & FL_TSTAMP) 
        {
            if (tcp_flags & FL_SACK) 
            {
                *ptr++ = TCPOPT_SACK_PERMITTED;	/*  4   */
                *ptr++ = TCPOLEN_SACK_PERMITTED;/*  2   */
	    } 
            else 
            {
                *ptr++ = TCPOPT_NOP;	/*  1   */
                *ptr++ = TCPOPT_NOP;	/*  1   */
	    }
            *ptr++ = TCPOPT_TIMESTAMP;	/*  8   */
            *ptr++ = TCPOLEN_TIMESTAMP;	/*  10  */

            *((u_int32_t *) ptr) = sock_lib::random_seq ();	/*  really!  */
            ptr += sizeof (u_int32_t);
	    *((u_int32_t *) ptr) = (tcp_flags & TH_ACK) ? sock_lib::random_seq () : 0;
            ptr += sizeof (u_int32_t);
        }
        else if (tcp_flags & FL_SACK)
        {
            *ptr++ = TCPOPT_NOP;	/*  1   */
            *ptr++ = TCPOPT_NOP;	/*  1   */
            *ptr++ = TCPOPT_SACK_PERMITTED;	/*  4   */
            *ptr++ = TCPOLEN_SACK_PERMITTED;	/*  2   */
        }

	if (tcp_flags & FL_WSCALE)
        {
            *ptr++ = TCPOPT_NOP;	/*  1   */
            *ptr++ = TCPOPT_WINDOW;	/*  3   */
            *ptr++ = TCPOLEN_WINDOW;	/*  3   */
            *ptr++ = 2;	/*  assume some corect value...  */
        }
        csum_len = ptr - buf;
	len = ptr - reinterpret_cast<u_int8_t *>(th);
	*lenp = htons (len);
	th->doff = len >> 2;
        return len;
    };
public:
    tcp_eng(const traceroute_options *to):sock_eng(to)
    {
    };
    friend  unique_ptr<sock_eng> create_sock_eng(const char, const traceroute_options *, spoll*);
public:
    void init(const char *h, spoll *&p)
     {
        sock_eng::init(h, p);
        if (o->ip_packet_len > 0)
        {
            data_len = DEF_DATA_LEN - header_len;
        }
        create_recv_sock();
        set_dest_port();
        tune_sock(icmp_sk);
        connect(icmp_sk);
        size_t len = sizeof(src);
        getsockname(icmp_sk, &src.sa, &len);
        if (getsockopt(icmp_sk, af == AF_INET? SOL_IP:SOL_IPV6,
                af == AF_INET? 14: IPV6_MTU, &mtu, &len )< 0)
            mtu = 576;
        mtu -= af == AF_INET? sizeof(struct iphdr): sizeof (struct ip6_hdr);
        mtu -= sizeof (struct tcphdr);  
        sock_lib::use_recv_err(icmp_sk, af);
        sockpoll->add_poll(icmp_sk,POLLIN | POLLERR);
        set_flags();
        data_len = generate_data();
    };
    void set_dest_port(void)
    {
        if(o->dest_port != -1)
        {
            dest_port = htons(o->dest_port);
        } 
        else
            dest_port = htons(sock_lib::random_seq());
        
    };
    void create_recv_sock(void)
    {
        icmp_sk = socket (af, SOCK_RAW, IPPROTO_TCP); 
        
    }; 
    void send_probe(int ttl)
    {
        static int last_ttl = -1;
        int sk;
	int af = dest_addr.sa.sa_family;
	any_addr addr;
	socklen_t len = sizeof (addr);
        probe *pb = probe::create_probe();
        pb->send_ttl = ttl;
        sk = socket(af, SOCK_STREAM, 0);
        if (sk == -1)
        {
          printf("socket creation error\n");
          return;
        }
        sock_lib::addr_bind_sock(sk, &src_addr);
        if (o->interface != NULL)
            sock_lib::associate_interface(o->interface, sk);
        getsockname(sk, &addr.sa, &len);
        th->source = addr.sin.sin_port;

        th->seq = sock_lib::random_seq ();

        th->check = 0;
        th->check = sock_lib::in_csum (buf, csum_len);


        if (ttl != last_ttl)
        {
            sock_lib::set_ttl (af, ttl, icmp_sk);
            last_ttl = ttl;
	} 
        pb->send_time = sock_lib::get_time();
        sock_lib::send(icmp_sk, th, th->doff<<2);
        pb->seq = th->source;
        
        pb->sk = sk;
        sockpoll->add_poll(sk, POLLERR| POLLHUP| POLLOUT);


    };

    
};
#endif
