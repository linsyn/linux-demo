#ifndef __UTILS_HEADER__
#define __UTILS_HEADER__

#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
extern int s_pipe[2];
int set_non_blocking(int fd) {
	printf("set_non_blocking %d\n", fd);
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return old_option;
}


int add_fd(int fd, int epoll_fd) {
	epoll_event ep_event;
	ep_event.data.fd = fd;
	ep_event.events = EPOLLIN|EPOLLET;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ep_event);
	return 1;
}


int remove_fd(int fd, int epoll_fd) {
	epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	return 1;
}

void signal_hadler(int signal) {
	int save_errno = errno;
	int msg = signal;
	send(s_pipe[0], (char *)&msg, 1, 0);
	errno = save_errno;

}

void set_signal_handler(int signal, void (*handler)(int), bool restart) {
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handler;
	sa.sa_flags |= restart;
	sigfillset(&sa.sa_mask);
	sigaction(signal, &sa, NULL);
	
}

#endif
