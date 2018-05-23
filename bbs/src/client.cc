#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
int connect_server(const char *server_ip, int port) {

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd > 0) {
		struct sockaddr_in sock_server;
		bzero(&sock_server, sizeof(sock_server));
					sock_server.sin_port = htons(port);
			sock_server.sin_family = AF_INET;
		int ret = inet_pton(AF_INET, server_ip, &(sock_server.sin_addr)) ;
		printf("%d\n",  ret);
		if (ret >= 0) {

			ret = connect(fd, (struct sockaddr *)&sock_server, sizeof(sock_server));
			if (ret == 0) {
				printf("connect server success\n");
				//return fd;
			} else {
				printf("connect error %s\n", strerror(errno));
				return -1;
			}
		} else {
			printf("set sock error %s\n", strerror(errno));
			return -1;
		}
	}
	return fd;
}
int main(int argc, char *argv[]) {
	printf("chat room client working\n");
	if (argc < 3)
	{
		printf("error param\n");
		return -1;
	}
	printf("server %s port %s\n", argv[1], argv[2]);
	int fd = connect_server(argv[1], atoi(argv[2]));
	if (fd > 0) {
		pollfd fds[2];
		fds[0].fd = 0;
		fds[0].events = POLLIN;
		fds[0].revents = 0;
		fds[1].fd = fd;
		fds[1].events = POLLIN | POLLHUP;
		fds[1].revents = 0;
		int pipefds[2];
		pipe(pipefds);
		do {
			int ret = poll(fds, 2, -1);
			printf("poll events\n");
			if (ret < 0 ){
				printf("poll error\n");
				break;
			}
			if (fds[1].revents & POLLHUP) {
				printf("server disconnect\n");
				break;
			} else if (fds[1].revents & POLLIN) {
				char sz_buffer[1024] = {0};
				recv(fds[1].fd, sz_buffer, 1024, 0);
				printf("revc : %s\n", sz_buffer);
			} else if (fds[0].revents & POLLIN) {
	
#ifndef MAC
				ret = splice(0, NULL, pipefds[1], NULL, 32768, 5);
				ret = splice(pipefds[0], NULL, fd , NULL, 32768, 5);
#else 	
				printf("111\n");
				char sz_buffer[1024] = {0};
				recv(fds[0].fd, sz_buffer, 1024, 0);
#endif
				
			}
		} while (true);
	}
	return 1;
}