#ifndef __ANY_ADDR_H__
#define __ANY_ADDR_H__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
typedef union common_addr any_addr;
#define TH_FLAGS(TH)    (((u_int8_t *) (TH))[13])
union common_addr
{
    struct sockaddr sa;
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;
};

#endif
