#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <traceroute_options.h>
#include <traceroute_api.h>
#include <traceroute.hpp>
static int help = 0;
static struct option long_options [] = 
{
    {"sport", required_argument,0, 's'},
    {"mtu", required_argument, 0, 'u' },
    {"back",  no_argument, &tro.backward_hops, 1},
    {"help", no_argument, 0, 'H'},
    {"sport", required_argument, 0, 'a'},
    {"fwmark", required_argument, 0, 'K'},
    {"UL", no_argument, 0, 'j'},
    {"flags", required_argument, 0, 'b'},
    {"syn", no_argument,&tro.tcp_syn, 1},
    {"ack", no_argument, &tro.tcp_ack, 1},
    {"fin", no_argument, &tro.tcp_fin, 1},
    {"rst", no_argument, &tro.tcp_rst, 1},
    {"psh", no_argument,&tro.tcp_psh, 1},
    {"urg", no_argument,&tro.tcp_urg, 1},
    {"ece", no_argument, &tro.tcp_ece, 1},
    {"cwr", no_argument, &tro.tcp_cwr, 1},
    {"ecn", no_argument, &tro.tcp_ecn, 1},
    {"sysctl",no_argument,&tro.tcp_sysctl, 1},
    {"mss", required_argument, 0, 'S'},
    {"info", no_argument, &tro.tcp_reply_info, 1},
    {NULL, 0, 0, 0}
};
char *host = NULL;
int parse_arg(int argc, char *argv[])
{
    int c;
    int optindex;
    while ((c = getopt_long(argc, argv, "a:w:u:s:j46P:M:z:q:i:m:p:s:FITUnreAVf:g:N:",long_options, &optindex)) != -1)
    {
        switch(c)
        {
        case '4':
            tro.is_af_inet4 = 1;
            break;
        case '6':
            tro.is_af_inet4 = 0;
        case 'T':
            tro.trace_method = strdup("tcp");
            break;
        case 'F':
            tro.ip_no_frag = 1;
            break;
        case 'f':
            tro.ip_first_ttl = atoi(optarg);
            break;
        case  'g':
            tro.ip_gateway = strdup(optarg);
            break;
        case 'i':
            tro.interface = strdup(optarg);
            break;
        case 'm':
            tro.ip_max_ttl = atoi(optarg);
            break;
        case 'N':
            tro.squeries = atoi(optarg);
            break;
        case 'n':
            tro.map_to_hostname = 0;
            break;
        case 'p':
            tro.dest_port = atoi(optarg);
            break;
        case 't':
            tro.tos = atoi(optarg);
            break;
        case 'w':
            tro.waittime = atoi(optarg);
            break;
        case 'q':
            tro.nqueries = atoi(optarg);
            break;
        case 'r':
            tro.bypass_normal_routing_table = 1;
        case 's':
            tro.src_addr = strdup(optarg);
            break;
        case 'z':
            tro.send_wait = atoi(optarg);
            break;
        case 'e':
            tro.show_icmp_ext = 1;
            break;
        case 'A':
            tro.perform_path_lookups = 1;
            break;
        case 'V':
            printf("\n%s\n", get_version());
            return 1;
        case 'P':
             tro.trace_method = strdup("raw");
             break;
        case 'a': // concocted for long option "sport"
             tro.source_port = atoi(optarg);
             break;
        case 'K': // concocted for long option "fwmark"
             tro.firewall_mask = strdup(optarg);
             break;
        case 'j': // concocted for long option "UL"
             tro.trace_method = strdup("udplite");
             break;
        case 'U':
             tro.trace_method = strdup("udp");
             break;
        case 'H': // concocted for long option "help"
             help = 1;
             return 0;
        case 'u': // concocted for long option mtu
             break;
        case 'S':// added for tcp_mss
             tro.tcp_mss = atoi(optarg);
             break;
        default: 
            printf("\nunsupported arguments\n");
            return 1;
        }
    }
    host = strdup(argv[optind]);
    if (host == NULL)
        return 1;
    return 0;
}

void run_traceroute(int argc, char *argv[])
{
   init_options();
   parse_arg(argc, argv);
   unique_ptr<traceroute> upt = traceroute::get_instance(host, tro);
   upt->init();
   upt->do_work(); 
} 


