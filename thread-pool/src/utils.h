#ifndef __UTILS_HEADER__
#define __UTILS_HEADER__
#ifndef MAC
int add_epoll_fd(int epoll_fd, int fd);
int remove_epoll_fd(int epoll_fd, int fd);
#endif
int start_listen(int port);
#endif