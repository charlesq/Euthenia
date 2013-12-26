#ifndef __SOCK_ENG_HPP__
#define __SOCK_ENG_HPP__

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
#include <sock_lib_args.h>
#include <traceroute_const.h>
using namespace std;
class sock_eng
{
public:
    int  icmp_sk; // receiving icmp reply 
    int af;  // address family AF_INET or AF_INET6;
    any_addr dest_addr;
    any_addr src_addr;
    int src_af;
    const traceroute_options *o;
    char * rtbuf;
    size_t rtbuf_len;
    unsigned char buf[500];
    size_t data_len;
    spoll *sockpoll;
    char *msg_buf;
    size_t msg_buf_len;
    char *msg_control;
    size_t msg_control_len;
    any_addr from;
    size_t header_len;
    struct msghdr msg; 
protected:
    sock_eng(const traceroute_options *t)
    {
        icmp_sk = -1;
        af = AF_INET;
        dest_addr = {{0},};
        o = t;
        rtbuf = NULL;
        rtbuf_len = 0;
        header_len = 0 ;
        data_len = 0;
        msg_control_len = 1500;
        msg_buf_len = 1500;
        msg_buf = new char [msg_buf_len];
        msg_control = new char [msg_control_len];
    };
friend unique_ptr<sock_eng> create_sock_eng(const char , const traceroute_options *, spoll*);
public:
    virtual size_t get_data_len(void) const
    {
        return data_len;
    };
    const  any_addr &get_dest_addr(void) const
    {
        return dest_addr;
    };
    virtual size_t get_header_len(void) const
    {
        return header_len;
    };
    virtual void init(const char *h, spoll *&sp)
    {
        sockpoll = sp;
        set_host(h);
        header_len += (af == AF_INET)? sizeof(struct iphdr): sizeof (struct ip6_hdr);
        if (o->src_addr != NULL)
        {
            sock_lib::get_addr(o->src_addr, &src_addr, src_af);
        }
        else
        {
            src_af = 0;
            memset(&src_addr, 0, sizeof(src_addr));
            src_addr.sa.sa_family = af;
        }

    };
    void set_af(int a)
    {
        af = a;
    };
    virtual size_t generate_data(void) { return 0;};
    virtual void expire_probe(probe *pb, spoll *sp) = 0;
    virtual void create_recv_sock(void) {};
    virtual void send_probe(int ttl)=0;
    int connect(int sk)
    {
        if (::connect(sk,&dest_addr.sa, sizeof(dest_addr)) < 0)
        {
            return -1; 
        }
        return 0;
    };
    int set_host(const char *host)
    {
        return sock_lib::get_addr(host, &dest_addr, af);
    };
    void close(void)
    {
        if (icmp_sk == -1)
            return;
        ::close(icmp_sk);
    };
    virtual probe * recv_reply(int sk, int err)
    {
        struct iovec iov;
        char * bufptr = msg_buf;
        size_t hlen, n;
        memset (&msg, 0, sizeof (msg));
	msg.msg_name = &from;
	msg.msg_namelen = sizeof (from);
	msg.msg_control = msg_control;
	msg.msg_controllen = msg_control_len;
	iov.iov_base = msg_buf;
	iov.iov_len = msg_buf_len;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	n = recvmsg (sk, &msg,(err != 0) ? MSG_ERRQUEUE : 0);
        if(n ==  0|| n > 0xffff)
        {
            return NULL;
        }
        if (!err && af == AF_INET && header_len == 0)
        {
            struct iphdr *ip = reinterpret_cast<struct iphdr*>(bufptr); 
            if(n < sizeof (struct iphdr)) return NULL; 
            hlen = ip->ihl << 2;
            if (n < hlen) return NULL;
            bufptr += hlen;
            n -= hlen;
        } 
        probe * p = retrieve_probe_rec(sk, err, bufptr, n);
        if ( p == NULL)
            return NULL;
        struct sock_lib_args ca;
        ca.pb = p;
        ca.data_len = &data_len;
        ca.extension = o->show_icmp_ext; 
        ca.header_len = &header_len;
        ca.af = af;
        ca.err = err;
        ca.bufp = bufptr;
        ca.n = n;
        ca.from = &from;
        ca.sp = sockpoll; 
        ca.mtudisc = o->ip_mtu_disc;
        sock_lib::parse_cmsg(msg, ca); 
        return p; 
    };
    virtual probe *retrieve_probe_rec(int sk, bool err, char * bufp, int n)=0;
    void tune_sock(int sk)
    {
        sock_lib_args ca;
        ca.af = af;
        ca.rtbuf = rtbuf;
        ca.rtbuf_len = rtbuf_len;
        ca.dest_addr = &dest_addr;
        ca.src_addr = &src_addr;
        
        
        sock_lib::tune_socket(sk, o, ca);
    };
    virtual ~sock_eng(void)
    {
        close();
        if (rtbuf != NULL)
           delete [] rtbuf; 
        delete [] msg_buf;
        delete [] msg_control;
    }; 
    void set_gateways(int sk) const
    {
       // TBD 
    };
};
#endif
