#ifndef __UDP_ENG_HPP__
#define __UDP_ENG_HPP__

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
#include <probe.hpp>
#include <sock_eng.hpp>
#include <netinet/udp.h>
#include <stdio.h>
using namespace std;
#define MIN_COVERAGE	(sizeof (struct udphdr))
#ifndef IPPROTO_UDPLITE
#define IPPROTO_UDPLITE	136
#endif

#ifndef UDPLITE_SEND_CSCOV
#define UDPLITE_SEND_CSCOV	10
#define UDPLITE_RECV_CSCOV	11
#endif
class default_udp_eng: public sock_eng
{
protected:
    int protocol;
    bool increment;
    size_t coverage; 
    size_t port_seq;
public:
    void set_coverage (int sk) {
        int val = MIN_COVERAGE;

        setsockopt (sk, IPPROTO_UDPLITE, UDPLITE_SEND_CSCOV,
					    &coverage, sizeof (coverage));

        setsockopt (sk, IPPROTO_UDPLITE, UDPLITE_RECV_CSCOV,
					    &val, sizeof (val));
    };
    void set_protocol(int p)
    {
        protocol = p;
    };
     probe *recv_reply(int sk, int revents)
    {
        if (!(revents &(POLLIN | POLLERR)))
            return NULL;
        return sock_eng::recv_reply(sk, !!(revents & POLLERR)); 
    }; 
    void expire_probe(probe *pb, spoll *sp)
    {
        probe::probe_done(pb, sp);
    };
    probe *retrieve_probe_rec(int sk, bool err, char * buf, int n)
    {
        
        probe *pb;
        pb = probe::get_by_sock (sk);
        if (!pb)  return NULL;
        if (pb->seq != from.sin.sin_port) return NULL; 
	if (!err)
        {
	    pb->final = 1;
        }
	return pb;
    };
    size_t generate_data(void)
    {
        for (size_t i = 0; i < data_len; i ++)
            buf[i] = 0x40 + (i &0x3f);
        return data_len;
    };
public:
    default_udp_eng(const traceroute_options *to):sock_eng(to)
    {
        increment = true;
        header_len = sizeof(struct udphdr);
        set_protocol(IPPROTO_UDP);
    };
public:
     virtual void init(const char *h, spoll *&p)
     {
       if (!o->udp_port_seq) port_seq = DEF_UDP_PORT;
        sock_eng::init(h, p);
        if (o->ip_packet_len > header_len)
        {
            data_len = o->ip_packet_len - header_len;
        }
        else
        {
            data_len = DEF_DATA_LEN - header_len;
        }
       // data_len = 20;
        data_len = generate_data();
    };
    virtual void send_probe(int ttl)
    {
        probe *pb = probe::create_probe();
        int sk;
        pb->send_ttl = ttl;
	int af = dest_addr.sa.sa_family;
        sk = socket(af, SOCK_DGRAM, protocol);
        if (sk == -1)
        {
            printf("\n socket error");
            return;
        }

        if (o->interface != NULL)
            sock_lib::associate_interface(o->interface, sk);
        if (coverage) set_coverage(sk);
        tune_sock(sk);
        sock_lib::set_ttl(af, ttl,sk);
        sock_lib::use_recv_err(sk, af);
        connect(sk);
        pb->send_time = sock_lib::get_time();
        sock_lib::send(sk, buf, data_len);
        pb->seq = dest_addr.sin.sin_port;
        pb->sk = sk;
        sockpoll->add_poll(sk, POLLIN | POLLERR);    
        if (increment)
        {
            port_seq ++;         
            dest_addr.sin.sin_port = htons(port_seq);
        }

    };
    friend  unique_ptr<sock_eng> create_sock_eng(const char, const traceroute_options *, spoll*);
    friend class udp_eng;
    friend class udp_lite_eng;

    
};

class udp_eng: public default_udp_eng
{
public:
    udp_eng(const traceroute_options *to):default_udp_eng(to)
    {
        increment = false;
    };
    
    friend  unique_ptr<sock_eng> create_sock_eng(const char, const traceroute_options *, spoll*); 
};



class udp_lite_eng: public default_udp_eng
{
public:
    udp_lite_eng(const traceroute_options *to):default_udp_eng(to)
    {
        increment = false;
        set_protocol(IPPROTO_UDPLITE);
        coverage = MIN_COVERAGE;
    };
    friend  unique_ptr<sock_eng> create_sock_eng(const char, const traceroute_options *, spoll*);
};
 
#endif
