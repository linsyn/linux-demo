#ifndef __POLL_EVENTS_HEADER__
#define __POLL_EVENTS_HEADER__
#include <sys/poll.h>


class poll_event{
	public:
		poll_event(int fd, int (* cb)(void *data), int index){
			sock_fd = fd;
			event_cb = cb;
			ev_index = index;
		}
	public:
		int get_ev_index(){
			return this->ev_index;
		}

		void set_ev_index(int index) {
			this->ev_index = index;
		}

	public:
		int sock_fd;//监听的套接字
		int (* event_cb)(void *data);//事件的回调
		int ev_index;//在poll事件列表中的索引
};
void init_context();
int add_poll_event(int fd, int (* event_cb)(void *data));
int delete_poll_event(int fd);
void process_poll_event();
#endif