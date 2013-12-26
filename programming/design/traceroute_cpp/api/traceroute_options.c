#include <stdlib.h>
#include <stddef.h>
#include <traceroute_options.h>
#include <traceroute_const.h>
traceroute_options tro;
void init_options(void)
{
    tro.ipv6_flow_label = 0;
    tro.is_af_inet4 = 1;
    tro.ip_no_frag = 0;
    tro.sock_debug = 0;
    tro.interface = NULL;
    tro.ip_max_ttl = DEF_HOPS;
    tro.ip_first_ttl = 1;
    tro.ip_gateway = NULL;
    tro.squeries = DEF_SIM_PROBES;
    tro.nqueries =DEF_NUM_PROBES;
    tro. ip_mtu_disc = 0;
    tro. map_to_hostname = 1;
    tro.dest_port = 80;
    tro.tos = 0;
    tro.waittime = DEF_WAIT_SECS; 
    tro.src_addr = NULL;
    tro.send_wait = 0;
    tro.show_icmp_ext = 0;
    tro.perform_path_lookups = 0;
    tro.source_port = 0;
    tro.firewall_mask = NULL;
    tro.trace_method = NULL;
    tro.icmp_raw = 1;
    tro.tcp_ack = -1;
    tro.tcp_timestamps = 0;
    tro.tcp_window_scaling = 0;
    tro.tcp_sysctl = 1;
    tro.backward_hops = 0;
    tro.tcp_syn = 0;
    tro.tcp_ack = 0;
    tro. tcp_fin = 0;
    tro.tcp_rst = 0;
    tro.tcp_psh = 0;
    tro.tcp_urg = 0;
    tro.tcp_ece = 0;
    tro.tcp_cwr = 0;
    tro.tcp_ecn = 0;
    tro.tcp_mss = 0;
    tro.tcp_reply_info = 0;
    tro.bypass_normal_routing_table = 0;
    tro.udplite_coverage = -1;
    tro.ip_dont_route = 0;
    tro.ip_packet_len = 0;
    tro.resolve_name = 1;
    tro.udp_port_seq = 0;
}
static const char *version = "0.9";
const char *get_version(void)
{
    return version;
}
void uninit(void) __attribute__((destructor));
void uninit(void)
{
    if (tro.src_addr != NULL)
        free(tro.src_addr);
    if (tro.firewall_mask != NULL)
        free(tro.firewall_mask);
    if (tro.interface != NULL)
        free(tro.interface);
    if (tro.ip_gateway != NULL)
        free(tro.ip_gateway);
    if (tro.trace_method != NULL)
        free(tro.trace_method);

}
