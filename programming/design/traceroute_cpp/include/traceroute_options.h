#ifndef __TRACEROUTE_OPTIONS_H__
#define __TRACEROUTE_OPTIONS_H__
#include <stddef.h>
#ifdef __cplusplus
extern "C"
{
#endif
typedef struct _traceroute_options traceroute_options;
struct _traceroute_options
{
    int is_af_inet4; // true indicates ip v4; false indicats ip v6 is preferred
                      //;an advisory setting
    int ip_no_frag;
    int sock_debug;
    char *interface;      // specifies the network interface through which traceroute
                      // should send packets. By default, the interface is
                      // selected according to the routing table.
    int ipv6_flow_label;
    int ip_dont_route;
    int ip_max_ttl;         // specifies the maximum number of hops traceroute will probe.
    int ip_first_ttl;
    char *ip_gateway;
                     // the default is 30
    int squeries;   // specifies the number of probe packets sent out simutaneously.
                     // Sending several probes concurrently can speed up traceroute considerably;
                     // the default value is 16.
    int nqueries;     // sets the number of probe packets per hop.

    int ip_mtu_disc;     // discover MTU along the path being traced. Implies -F -N 1 
    int map_to_hostname; // specifies whether to map IP addresses to host name when
                             //  displaying them.
    int dest_port;        // For UDP tracing, specifies the destination port base traceroute will use.
                      // For ICMP tracing, specifies the initial ICMP sequence value.
                      // For TCP specifies just the (constant) destination port to connect.
    char tos;    //

    double  waittime; // Set the time (in seconds) to wait for a response to a probe (default 5.0 dec);
    char *src_addr; //Choose an alternative source address.

    int send_wait;  // Minimal time interval (in ms) between probes.
    int show_icmp_ext; // Show ICMP extensions(rfc 4884).
    int perform_path_lookups; // Perform AS path lookups in routing registries and print results directly after the corresponding addresses.
    int source_port; // Chooses the source port to use. Implies -N 1. Normally source ports are chosen by the system.

    char * firewall_mask; // set the firewall mark for outgoing packets.

    char *trace_method; // icmp, udp, tcp, udplite, raw,tcpconn
    int icmp_raw; // true means raw sockets only (the traditional way). while false means use only dgram icmp sockets.
    int tcp_sack;
    int tcp_timestamps;
    int tcp_window_scaling;
    int tcp_sysctl;
    int backward_hops; // Pring the number of backward hops when it seems difere nt with the forward direction.
    int tcp_syn;
    int tcp_ack;
    int tcp_fin;
    int tcp_rst;
    int tcp_psh;
    int tcp_urg;
    int tcp_ece;
    int tcp_cwr;
    char tcp_flags;
    int tcp_ecn;
    int tcp_mss;
    int tcp_reply_info;
    int raw_protocol;
    int bypass_normal_routing_table;
    int udplite_coverage;
    size_t ip_packet_len;
    int resolve_name;
    size_t udp_port_seq;
};
extern traceroute_options tro;
void init_options(void);
const char *get_version(void);
#ifdef __cplusplus
}
#endif
#endif
