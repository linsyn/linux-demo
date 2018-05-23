#include <sys/epoll.h>

#include "epoll_event.h"
#define EPOLL_EVENTS_MAX_NUM 64
int s_epoll_fd = -1;
struct epoll_event events[EPOLL_EVENTS_MAX_NUM];
void init_epoll_context() {
	s_epoll_fd = epoll_create(EPOLL_EVENTS_MAX_NUM);
}

void add_epoll_event(int fd, int (*event_cb)(void *)) {
	struct epoll_event event;
	cus_epoll_event *p_cus_e = new cus_epoll_event();
	p_cus_e->sock_fd = fd;
	p_cus_e->event_cb = event_cb;
	event.data.ptr = p_cus_e;
	event.events =  EPOLLIN;
	epoll_ctl(s_epoll_fd, EPOLL_CTL_ADD, fd, &event);
}
void delete_epoll_event(int fd){
	epoll_ctl(s_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
}
void process_epoll_event(){
	while(true) {
		int ev_num = epoll_wait(s_epoll_fd, events, EPOLL_EVENTS_MAX_NUM, -1);
		if (ev_num < 0) {
			/* code */
			break;
		} else if (ev_num > 0) {
			for (int i = 0; i < ev_num; ++i) {
				if (events[i].events & EPOLLIN) {
					cus_epoll_event *p_ev = (cus_epoll_event *)(events[i].data.ptr);
					if (p_ev) {
						if (p_ev->event_cb) {
							event_cb->event_cb(&p_ev->sock_fd);
						}
					}
				}
			}
		}
	}
}