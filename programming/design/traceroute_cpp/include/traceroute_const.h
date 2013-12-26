#ifndef __TRACEROUTE_CONST_H__
#define __TRACEROUTE_CONST_H__

#define MAX_HOPS	255
#define MAX_PROBES	10
#define MAX_GATEWAYS_4	8
#define MAX_GATEWAYS_6	127
#define DEF_HOPS	30
#define DEF_SIM_PROBES	16	/*  including several hops   */
#define DEF_NUM_PROBES	3
#define DEF_WAIT_SECS	5.0
#define DEF_SEND_SECS	0
#define DEF_DATA_LEN	40	/*  all but IP header...  */
#define MAX_PACKET_LEN	65000

#define DEF_START_PORT	33434	/*  start for traditional udp method   */
#define DEF_UDP_PORT	53	/*  dns   */
#define DEF_TCP_PORT	80	/*  web   */
#define DEF_DCCP_PORT	DEF_START_PORT	/*  is it a good choice?...  */
#define DEF_RAW_PROT	253	/*  for experimentation and testing, rfc3692  */


#endif
