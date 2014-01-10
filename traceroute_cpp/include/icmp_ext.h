#ifndef __ICMP_EXT_H__
#define __ICMP_EXT_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sock_lib.hpp>


struct icmp_ext_header {
#if __BYTE_ORDER == __BIG_ENDIAN
	unsigned int version:4;
	unsigned int reserved:4;
#else
	unsigned int reserved:4;
	unsigned int version:4;
#endif
	u_int8_t reserved1;
	u_int16_t checksum;
} __attribute__ ((packed));


struct icmp_ext_object {
	u_int16_t length;
	u_int8_t class_t;
	u_int8_t c_type;
	u_int8_t data[0];
};

#define MPLS_CLASS 1
#define MPLS_C_TYPE 1


#define do_snprintf(CURR, END, FMT, ARGS...)	\
	do {					\
	    CURR += snprintf (CURR, END - CURR, (FMT), ## ARGS);\
	    if (CURR > END)  CURR = END;			\
	} while (0)
#endif
