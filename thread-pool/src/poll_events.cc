
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "poll_events.h"

struct pollfd pfds[1024];
int pfd_idx = 0;
poll_event * poll_events[65535];

void init_context() {
	printf("%lu\n",  sizeof(poll_events));
	memset(poll_events, 0, sizeof(poll_events));

}

int add_poll_event(int fd, int (* event_cb)(void *data)) {
	if (fd > 0) {
		pfds[pfd_idx].fd = fd;
		pfds[pfd_idx].events = POLLIN | POLLHUP;
		pfds[pfd_idx].revents = 0;
		poll_event *ptr = new poll_event(fd, event_cb, pfd_idx);
		if (ptr && poll_events[fd] == NULL) {
			/* code */
			poll_events[fd] = ptr;
			pfd_idx ++;
		}
		return pfd_idx;
	}
	return -1;
}


int delete_poll_event(int fd) {
	if (fd >= 0 && fd < 65535) {
		poll_event *ptr =  poll_events[fd];
		if (ptr) {
			int index = ptr->get_ev_index();
			if (index >= 0 && index < 1024) {
				pfds[index] = pfds[pfd_idx - 1];
				if (poll_events[pfds[index].fd]) {
					poll_events[pfds[index].fd]->set_ev_index(index);
				}
			}
			delete ptr;
			poll_events[fd] = NULL;
			pfd_idx --;
		}
	}
	return 1;
}

void process_poll_event() {
	while(true) {
		int ret = poll(pfds, pfd_idx, -1);
		if (ret == 0) {
			continue;
		} else if (ret == -1) {
			printf("poll error occurs\n");
			continue;
		} else if (ret > 0) {
			for (int i = 0; i < pfd_idx; ++i) {
				if (pfds[i].revents & POLLIN) {
					if (poll_events[pfds[i].fd] && poll_events[pfds[i].fd]->event_cb != NULL) {
						/* code */
						poll_events[pfds[i].fd]->event_cb(&pfds[i].fd); 
					}
				}
			}
		}
	}
}