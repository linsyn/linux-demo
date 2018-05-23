#ifndef __LINUX_EPOLL_EVENT_HEADER__
#define __LINUX_EPOLL_EVENT_HEADER__

struct epoll_event_s
{
	int sock_fd;
	int int (*event_cb)(void *);
};

void init_epoll_context();
void add_epoll_event(int fd, int (*event_cb)(void *));
void delete_epoll_event(int fd);
void process_epoll_event();

#endif