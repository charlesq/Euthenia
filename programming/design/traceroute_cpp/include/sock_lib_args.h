#ifndef __SOCK_LIB_ARGS_H__
#define __SOCK_LIB_ARGS_H__
#include <unistd.h>
#include <stddef.h>
#include <any_addr.h>
class probe;
class spoll;
struct sock_lib_args
{
    int af;
    size_t *data_len;
    size_t *header_len; 
    int err;
    probe *pb;  
    any_addr *from;
    spoll *sp;
    char * bufp;
    size_t n;
    int extension;
    int mtudisc;
    char *rtbuf;
    size_t rtbuf_len;
    any_addr *dest_addr;
    any_addr *src_addr;
    
};


#endif
