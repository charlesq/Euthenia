#ifndef __OBSERVER_H__
#define __OBSERVER_H__
#include <stddef.h>
#include <sys/time.h>
#include <vector>
#include <queue>
using namespace std;
struct message
{
   int who; // socket descriptor, this field is defined for debugging purpose
   int what; /* 0 designates timeout;
                1 indicating containing data unit attached to
                   the  icmp EXCELL LIMIT message, excluding the ip header */
   struct timeval tv;
   vector<char> msg;//use a container to hold msg data

};
class observer
{
  queue<const struct message*> &msgq;
public:
    observer(queue<const struct message *> &q):msgq(q){};
    void notify(const struct message *m) {msgq.push(m);};  //observer is responsible for releasing message memory
};

#endif
