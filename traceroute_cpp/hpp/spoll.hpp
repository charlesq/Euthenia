#ifndef __SPOLL_HPP__
#define __SPOLL_HPP__
#include <poll.h>
#include <stddef.h>
#include <stdlib.h>
#include <utility>
#include <vector>
#include <spoll.hpp>
#include <iostream>
using namespace std;
typedef std::vector<std::pair<int,short>> poll_result; 
class spoll 
{
    struct pollfd *fds; 
    size_t num; 
    size_t size;
public:
    spoll(void):fds(NULL), num(0), size(0){}; 
    void cleanup_polls(void)
    {
        for (size_t i = 0; i < num; i ++)
        {
            if ((fds + i)->fd == -1)
            {
                 if ((fds + num -1)->fd != -1)
                 {
                     (fds + i)->fd = (fds+num -1)->fd;
                      (fds+i)->events = (fds+num -1)->events;
                       num --;
                 }
                 else
                 {
                     num--;
                 }
            } 
        }
    }

    poll_result  do_poll(int ms)
    {
        poll_result pr; 
        int r;
        r = poll(fds, num, ms);
        if (r <= 0) 
        {
            return pr;
        }
        for (size_t i = 0; i < num; i ++)
        {
            if ((fds+ i)->fd >= 0 && (fds+i)->revents != 0)
                pr.push_back(std::make_pair((fds+i)->fd, (fds+i)->revents));
        }
        return pr;
        
    };
    void add_poll(int fd, short evts)
    {
       for(size_t i = 0; i < size; i ++)
       {
           if ( (fds + i)->fd == -1)
           {
               (fds+i)->fd = fd;
               (fds+i)->events = evts;
                if (num < i)
                    num = i + 1; 
                return; 
           }
       }
       fds = static_cast<struct pollfd *>(realloc(fds, (size + 1)* sizeof(struct pollfd)));
       (fds + size)->fd = fd;
       (fds + size)->events = evts; 
       size ++;
       num = size ;
    };
    void remove_poll(int fd)
    {
       for (size_t i = 0; i < num; i++)
       {
           if ((fds + i)->fd == fd)
           {
               if (num > 0 && num -1 != i)
               {
                   if ((fds +num -1)->fd != -1)
                   {
                       (fds + i)->fd = (fds + num -1)->fd;
                       (fds + i)->events = (fds + num -1)->events;
                        (fds + num -1)->fd = -1;
                       num --; 
                       break;
                   }
               }
               num --;
           }
       } 
    }; 
    ~spoll(void)
    {
        free(fds);
    };  
};

#endif
