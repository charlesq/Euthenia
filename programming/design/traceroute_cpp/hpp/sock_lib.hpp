#ifndef __SOCK_LIB_HPP__
#define __SOCK_LIB_HPP__
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/ip6.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <string.h>
#include <netdb.h>
#include <netinet/icmp6.h>
#include <netinet/ip_icmp.h>
#include <any_addr.h>
#include <probe.hpp>
#include <icmp_ext.h>
#include <linux/errqueue.h>
#include <sock_lib_args.h>
#include <traceroute_options.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
using namespace std;
#define TH_FLAGS(TH)	(((u_int8_t *) (TH))[13])
#define FL_FLAGS    0x0100
#define FL_ECN      0x0200
#define FL_SACK     0x0400
#define FL_TSTAMP   0x0800
#define FL_WSCALE   0x1000
#define TH_FIN      0x01
#define TH_SYN      0x02
#define TH_RST      0x04
#define TH_PSH      0x08
#define TH_ACK      0x10
#define TH_URG      0x20
#define TH_ECE      0x40
#define TH_CWR      0x80
#define ttl2hops(X)	(((X) <= 64 ? 65 : ((X) <= 128 ? 129 : 256)) - (X))
static struct {
	const char *name;
	unsigned int flag;
} tcp_flag_map[] = {
	{ "fin", TH_FIN },
	{ "syn", TH_SYN },
	{ "rst", TH_RST },
	{ "psh", TH_PSH },
	{ "ack", TH_ACK },
	{ "urg", TH_URG },
	{ "ece", TH_ECE },
	{ "cwr", TH_CWR },
};

class sock_lib
{
public:
    static double get_time(void)
    {
        struct timeval tv;
        double d;
        gettimeofday (&tv, NULL);
        d = ((double) tv.tv_usec) / 1000000.0 + (unsigned long) tv.tv_sec;

        return d;
    };
    static u_int16_t in_csum (const void *ptr, size_t len)
    {
        const u_int16_t *p = (const u_int16_t *) ptr;
        size_t nw = len / 2;
        unsigned int sum = 0;
        u_int16_t res;

        while (nw--)  sum += *p++;
        if (len & 0x1)
            sum += htons (*((unsigned char *) p) << 8);

        sum = (sum >> 16) + (sum & 0xffff);
        sum += (sum >> 16);

        res = ~sum;
        if (!res)  res = ~0;
           return res;
    };
    static void parse_cmsg(struct msghdr & msg, struct sock_lib_args &ca )
    {
        struct cmsghdr *cm;
        double recv_time = 0;
        int recv_ttl = 0;
        struct sock_extended_err *ee = NULL;
	/*  Parse CMSG stuff   */
        for (cm = CMSG_FIRSTHDR (&msg); cm; cm = CMSG_NXTHDR (&msg, cm)) {
        void *ptr = CMSG_DATA (cm);

        if (cm->cmsg_level == SOL_SOCKET) 
        {
            if (cm->cmsg_type == SO_TIMESTAMP) 
            {
                struct timeval *tv = (struct timeval *) ptr;

                recv_time = tv->tv_sec + tv->tv_usec / 1000000.;
             }
	    }
	    else if (cm->cmsg_level == SOL_IP) 
            {
                if (cm->cmsg_type == IP_TTL)
                    recv_ttl = *((int *) ptr);
                    
                else if (cm->cmsg_type == IP_RECVERR)
                {
                    ee = (struct sock_extended_err *) ptr;
                    if (ee->ee_origin != SO_EE_ORIGIN_ICMP)
                        ee = NULL;
                     /*  dgram icmp sockets might return extra things...  */
                     if (ee->ee_type == ICMP_SOURCE_QUENCH ||
                         ee->ee_type == ICMP_REDIRECT) 
                          return;
                }
            }
            else if (cm->cmsg_level == SOL_IPV6)
            {

                if (cm->cmsg_type == IPV6_HOPLIMIT)
                    recv_ttl = *((int *) ptr);
                else if (cm->cmsg_type == IPV6_RECVERR)
                {

                    ee = (struct sock_extended_err *) ptr;

                    if (ee->ee_origin != SO_EE_ORIGIN_ICMP6)
                        ee = NULL;
                 }
             }
        }

        if (!recv_time)
            recv_time = get_time ();

        if (ca.pb == NULL)
        {
            return; 
        }
        if (!ca.err)
            memcpy (&ca.pb->res, &ca.from, sizeof (ca.pb->res));

        ca.pb->recv_time = recv_time;

        ca.pb->recv_ttl = recv_ttl;

        if (ee)
        {
            memcpy (&ca.pb->res, SO_EE_OFFENDER (ee), sizeof(ca.pb->res));
            parse_icmp_data (ca.af, ca.pb, ee->ee_type, ee->ee_code, ee->ee_info);
        }

        if (ee &&
            ca.mtudisc &&
            ee->ee_info >= *ca.header_len &&
            ee->ee_info < *ca.header_len + *ca.data_len) 
        {
            *ca.data_len = ee->ee_info - *ca.header_len;

            probe::probe_done (ca.pb, ca.sp);

            /*  clear this probe (as actually the previous hop answers here)
            but fill its `err_str' by the info obtained. Ugly, but easy...
           */
            memset (ca.pb, 0, sizeof (probe));
            snprintf (ca.pb->err_str, sizeof(ca.pb->err_str)-1, "F=%d", ee->ee_info);

            return;
        }

        if (ee &&
            ca.extension &&
            *ca.header_len + ca.n >= (128 + 8) &&	/*  at least... (rfc4884)  */
            *ca.header_len <= 128 &&	/*  paranoia   */
            ((ca.af == AF_INET && (ee->ee_type == ICMP_TIME_EXCEEDED ||
                  ee->ee_type == ICMP_DEST_UNREACH ||
                  ee->ee_type == ICMP_PARAMETERPROB)) ||
                 (ca.af == AF_INET6 && (ee->ee_type == ICMP6_TIME_EXCEEDED ||
                  ee->ee_type == ICMP6_DST_UNREACH))
            ) ) 
        {
            int step;
            int offs = 128 - *ca.header_len;

            if (ca.n > *ca.data_len)  step = 0;	/*  guaranteed at 128 ...  */
            else
                step = ca.af == AF_INET ? 4 : 8;

             handle_extensions (ca.pb, ca.bufp + offs, ca.n - offs, step);
        }


        probe::probe_done (ca.pb, ca.sp);
    };
 
    static void parse_icmp_data(int af, probe *pb, int type, int code, int info)
    {
        const char *str = NULL;
        char buf[sizeof (pb->err_str)];
        if (af == AF_INET) 
        {
            if (type == ICMP_TIME_EXCEEDED)
            {
                 if (code == ICMP_EXC_TTL)
                     return;
            }
            else if (type == ICMP_DEST_UNREACH)
            {
                switch (code) 
                {
                    case ICMP_UNREACH_NET:
                    case ICMP_UNREACH_NET_UNKNOWN:
                    case ICMP_UNREACH_ISOLATED:
                    case ICMP_UNREACH_TOSNET:
                        str = "!N";
                        break;

                    case ICMP_UNREACH_HOST:
                    case ICMP_UNREACH_HOST_UNKNOWN:
                    case ICMP_UNREACH_TOSHOST:
                        str = "!H";
                        break;

                    case ICMP_UNREACH_NET_PROHIB:
                    case ICMP_UNREACH_HOST_PROHIB:
                    case ICMP_UNREACH_FILTER_PROHIB:
                        str = "!X";
                        break;

                    case ICMP_UNREACH_PORT:
                    /*  dest host is reached   */
                        str = "";
                        break;

                    case ICMP_UNREACH_PROTOCOL:
                        str = "!P";
                        break;

                    case ICMP_UNREACH_NEEDFRAG:
                        snprintf (buf, sizeof (buf), "!F-%d", info);
                        str = buf;
                        break;

                    case ICMP_UNREACH_SRCFAIL:
                        str = "!S";
                        break;

                    case ICMP_UNREACH_HOST_PRECEDENCE:
                        str = "!V";
                        break;

                    case ICMP_UNREACH_PRECEDENCE_CUTOFF:
                        str = "!C";
                        break;

                    default:
                        snprintf (buf, sizeof (buf), "!<%u>", code);
                        str = buf;
                        break;
                }
            }

        }
        else if (af == AF_INET6)
        {
            if (type == ICMP6_TIME_EXCEEDED) 
            {
                if (code == ICMP6_TIME_EXCEED_TRANSIT)
                    return;
        }
        else if (type == ICMP6_DST_UNREACH)
        {

            switch (code) 
            {

                case ICMP6_DST_UNREACH_NOROUTE:
                    str = "!N";
                    break;

                case ICMP6_DST_UNREACH_BEYONDSCOPE:
                case ICMP6_DST_UNREACH_ADDR:
                    str = "!H";
                    break;

                case ICMP6_DST_UNREACH_ADMIN:
                    str = "!X";
                    break;

                case ICMP6_DST_UNREACH_NOPORT:
                /*  dest host is reached   */
                    str = "";
                    break;

                default:
                    snprintf (buf, sizeof (buf), "!<%u>", code);
                    str = buf;
                    break;
                }
            }
            else if (type == ICMP6_PACKET_TOO_BIG) 
            {
                snprintf (buf, sizeof (buf), "!F-%d", info);
                str = buf;
            }
        }


        if (!str) 
        {
            snprintf (buf, sizeof (buf), "!<%u-%u>", type, code);
            str = buf;
        }

        if (*str) 
        {
            strncpy (pb->err_str, str, sizeof (pb->err_str));
            pb->err_str[strlen(str) - 1] = '\0';
        }
        pb->final = 1;

        return;

    };   
    static char *names_by_tcp_flags (unsigned int flags)
    {
        size_t i;
        char str[64];	/*  enough...  */
        char *curr = str;
        char *end = str + sizeof (str) / sizeof (*str);
        for (i = 0; i < sizeof (tcp_flag_map) / sizeof (*tcp_flag_map); i++)
        {
            const char *p;

            if (!(flags & tcp_flag_map[i].flag))  continue;

            if (curr > str && curr < end)  *curr++ = ',';
            for (p = tcp_flag_map[i].name; *p && curr < end; *curr++ = *p++) ;
        }

        *curr = '\0';
        return  strdup (str);
    }; 
    static   unsigned int random_seq (void) 
    {

        /*  To not worry about RANDOM_MAX and precision...  */
        return  (rand () << 16) ^ (rand () << 8) ^ rand () ^ (rand () >> 8);
    };
   static void associate_interface(const char *inf, int sk)
    {
       setsockopt(sk, SOL_SOCKET, SO_BINDTODEVICE, inf, strlen(inf) +1);
    };
    static int set_ttl(int af, int ttl, int sk)
    {
        if (af == AF_INET)
        {
            if (setsockopt(sk, SOL_IP, IP_TTL, &ttl, sizeof (ttl)) < 0)
                return -1;
        }
        else
        {
            if (setsockopt(sk, SOL_IPV6, IPV6_UNICAST_HOPS, &ttl, sizeof(ttl)) < 0)
                return -2;
        }
        return 0;
    };

    static  int get_addr(const char *name, any_addr *addr, int &af)
    {
        int ret;
        struct addrinfo hints,*ai, *res = NULL;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = af;
        hints.ai_flags = AI_IDN;
        ret = getaddrinfo(name, NULL, &hints, &res);
        if (ret)
            return -1;
        for (ai = res; ai; ai = ai->ai_next)
        {
            if (ai->ai_family == af) break;
            if (!af)
            {
                af = ai->ai_family;
                break;
            }
        }
        if (!ai) ai = res;
        if (ai->ai_addrlen > sizeof(*addr))
            return -2;
        memcpy(addr, ai->ai_addr, ai->ai_addrlen);
        freeaddrinfo(res);
        return 0;
    };

    static  std::string addr2str(const any_addr *addr)
    {
        char *as = static_cast<char*>(alloca(INET6_ADDRSTRLEN));

        getnameinfo(&addr->sa, sizeof(*addr), as, INET6_ADDRSTRLEN, 0, 0, NI_NUMERICHOST);
        return as;
    };
    static    int send(int sk, const void *data, size_t len)
    {
        ssize_t ret = ::send(sk, data, len, 0);
        if (ret == static_cast<ssize_t>(len) || ret == EAGAIN || ret == EWOULDBLOCK)
            return 0;
        return -1;
    };
    static  void use_recv_err(int sk, int af)
    {
        int val = 1;
        if (af == AF_INET)
            setsockopt (sk, SOL_IP, IP_RECVERR, &val, sizeof (val));
        else if (af == AF_INET6)
            setsockopt (sk, SOL_IPV6, IPV6_RECVERR, &val, sizeof (val));

    };
    static void use_timestamp(int sk)
    {
        int n = 1;
        setsockopt (sk, SOL_SOCKET, SO_TIMESTAMP, &n, sizeof (n));
    };
    static  void use_recv_ttl(int sk, int af)
    {
        int n = 1;
        if (af == AF_INET)
            setsockopt (sk, SOL_IP, IP_RECVTTL, &n, sizeof (n));
        else if (af == AF_INET6)
            setsockopt (sk, SOL_IPV6, IPV6_RECVHOPLIMIT, &n, sizeof (n));
    };

    static void addr_bind_sock(int sk, any_addr * addr)
    {
        if (bind(sk, &addr->sa, sizeof(*addr))< 0)
        {
           return;
        }

    };
    static int recv_message(int sk, bool err,  char *buf, char *control, any_addr  &from)
    {
        struct msghdr msg;
	struct iovec iov;
	int n;

        memset (&msg, 0, sizeof (msg));
        msg.msg_name = &from;
        msg.msg_namelen = sizeof (from);
        msg.msg_control = control;
        msg.msg_controllen = sizeof (control);
        iov.iov_base = buf;
        iov.iov_len = sizeof (buf);
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        

        n = recvmsg (sk, &msg, err ? MSG_ERRQUEUE : 0);
        return n;

    };
     static int try_extension (probe *pb, char *buf, size_t len)
    {
#if 1
        struct icmp_ext_header *iext = (struct icmp_ext_header *) buf;
        char str[1024];
        char *curr = str;
        char *end = str + sizeof (str) / sizeof (*str);


        /*  a check for len >= 8 already done for all cases   */

        if (iext->version != 2)  return -1;
#if 1
        if (iext->checksum &&
           sock_lib::in_csum (iext, len) != (u_int16_t) ~0)
             return -1;
#endif
        buf += sizeof (*iext);
        len -= sizeof (*iext);


        while (len >= sizeof (struct icmp_ext_object))
        {
            struct icmp_ext_object *obj = (struct icmp_ext_object *) buf;
            size_t objlen = ntohs (obj->length);
            size_t data_len;
            u_int32_t *ui = (u_int32_t *) obj->data;
            int i, n;

            if (objlen < sizeof (*obj) || objlen > len)
                return -1;

            data_len = objlen - sizeof (*obj);
            if (data_len % sizeof (u_int32_t))
                return -1;      /*  must be 32bit rounded...  */

            n = data_len / sizeof (*ui);

  if (curr > (char *) str && curr < end)
                *curr++ = ';';  /*  a separator   */

            if (obj->class_t == MPLS_CLASS &&
                obj->c_type == MPLS_C_TYPE &&
                n >= 1)
            {    /*  people prefer MPLS to be parsed...  */

                do_snprintf (curr, end, "MPLS:");

                for (i = 0; i < n; i++, ui++)
                {
                    u_int32_t mpls = ntohl (*ui);

                    do_snprintf (curr, end, "%sL=%u,E=%u,S=%u,T=%u",
                         i ? "/" : "",
                         mpls >> 12,
                         (mpls >> 9) & 0x7,
                         (mpls >> 8) & 0x1,
                          mpls & 0xff);
                }

            }
            else {      /*  common case...  */

                do_snprintf (curr, end, "%u/%u:", obj->class_t, obj->c_type);

                for (i = 0; i < n && curr < end; i++, ui++)
                    do_snprintf (curr, end, "%s%08x", i ? "," : "", ntohl(*ui));
            }

            buf += objlen;
            len -= objlen;
        }

        if (len)  return -1;

     pb->ext = strdup (str);
#endif
        return 0;
    };


    static void handle_extensions (probe *pb, char *buf, int len, int step)
    {


        if (!step)
            try_extension (pb, buf, len);
        else
        {
            for ( ; len >= 8; buf += step, len -= step)
                if (try_extension (pb, buf, len) == 0)
                    break;
        }
        return;
    };
    static void tune_socket(int sk, const traceroute_options *o,struct sock_lib_args &ca )
    {
        int i = 0;
        if (o->sock_debug)
        {
            i = 1;
            setsockopt (sk, SOL_SOCKET, SO_DEBUG, &i, sizeof (i));
        }
#if 0
        if (o->firewall_mask) {
            setsockopt (sk, SOL_SOCKET, SO_MARK,
               &o->firewall_mask, sizeof (&o->firewall_mask));
        }
#endif
        if (ca.rtbuf_len > 0)
        {
            if (ca.af == AF_INET)
                setsockopt (sk, IPPROTO_IP, IP_OPTIONS, ca.rtbuf, ca.rtbuf_len);
            else if (ca.af == AF_INET6)
                setsockopt (sk, IPPROTO_IPV6, IPV6_RTHDR, ca.rtbuf, ca.rtbuf_len);
        }
     
        sock_lib::addr_bind_sock (sk, ca.src_addr);
        if (o->interface != NULL)
            sock_lib::associate_interface(o->interface, sk);
        if (ca.af == AF_INET)
        {
            i = o->ip_no_frag ? IP_PMTUDISC_PROBE  : IP_PMTUDISC_DONT;
            setsockopt (sk, SOL_IP, IP_MTU_DISCOVER, &i, sizeof(i));
            if (!o->ip_no_frag)
            {
                 i = IP_PMTUDISC_DO;
                 setsockopt (sk, SOL_IP, IP_MTU_DISCOVER, &i, sizeof(i));
            }

            if (o->tos) {
                i = o->tos;
     setsockopt (sk, SOL_IP, IP_TOS, &i, sizeof (i));
            }

        }
        else if (ca.af == AF_INET6)
        {

            i = o->ip_no_frag? IPV6_PMTUDISC_PROBE : IPV6_PMTUDISC_DONT;
            setsockopt (sk, SOL_IPV6, IPV6_MTU_DISCOVER,&i,sizeof(i));
            if(!o->ip_no_frag)
            {
                i = IPV6_PMTUDISC_DO;
                setsockopt (sk, SOL_IPV6, IPV6_MTU_DISCOVER,&i,sizeof(i));
            }
#if 0
            if (o->ipv6_flow_label)
            {
                struct in6_flowlabel_req flr;
                memset (&flr, 0, sizeof (flr));
                flr.flr_label = htonl (flow_label & 0x000fffff);
                flr.flr_action = IPV6_FL_A_GET;
                flr.flr_flags = IPV6_FL_F_CREATE;
                flr.flr_share = IPV6_FL_S_ANY;
                memcpy (&flr.flr_dst, &dst_addr.sin6.sin6_addr,sizeof (flr.flr_dst));
                setsockopt (sk, IPPROTO_IPV6, IPV6_FLOWLABEL_MGR, &flr, sizeof (flr));
            }
#endif
            if (o->tos)
            {
                i = o->tos;
                setsockopt (sk, IPPROTO_IPV6, IPV6_TCLASS, &i, sizeof (i));
            }
#if 0
            if (o->tos || o->ipv6_flow_label)
            {
                i = 1;
                setsockopt (sk, IPPROTO_IPV6, IPV6_FLOWINFO_SEND, &i, sizeof (i));
            }
#endif
        }


        if (o->ip_dont_route)
        {
            i = 1;
            setsockopt (sk, SOL_SOCKET, SO_DONTROUTE, &i, sizeof (i));
        }


        sock_lib::use_timestamp (sk);

        sock_lib::use_recv_ttl (sk,ca.af);

        fcntl (sk, F_SETFL, O_NONBLOCK);

    }; 
    static bool equal_addr(any_addr *a, any_addr *b)
    {
        if (!a->sa.sa_family)
            return false;

        if (!a->sa.sa_family || a->sa.sa_family != b->sa.sa_family)
		return false;

	if (a->sa.sa_family == AF_INET6)
	    return  !memcmp (&a->sin6.sin6_addr, &b->sin6.sin6_addr,
						sizeof (a->sin6.sin6_addr));
	else
	    return  !memcmp (&a->sin.sin_addr, &b->sin.sin_addr,
						sizeof (a->sin.sin_addr));
    };
    
};




#endif
