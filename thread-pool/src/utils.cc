#include "utils.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#ifndef MAC
int add_epoll_fd(int epoll_fd, int fd) {
	return 1;
}
int remove_epoll_fd(int epoll_fd, int fd){
	return 1;
}
#endif

int start_listen(int port){
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd > 0)
	{
		/* code */
		struct  sockaddr_in  sock_addr;
		bzero(&sock_addr, sizeof(sock_addr));
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_port = htons(port);
		sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		int  reuseport = 1;

                if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT,
                               (const void *) &reuseport, sizeof(int))
                    == -1)
                {
			printf("reset error \n");
                }
		int ret = bind(fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr));
		if (ret == 0) {
			ret = listen(fd, 64);
			if (ret == 0) {
				return fd;
			}
		}
		return -1;
		
	}

	return fd;
}
