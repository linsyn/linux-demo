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

#define FD_LIMIT 65535

int setnonblocking(int fd){
	return 1;	
}

typedef struct __client_data {
	int fd;
	struct sockaddr_in  sock;
}client_data;

int start_listen(const char *ip_addr, int port) {
	int fd_server = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_server > 0 ) {
		struct sockaddr_in sock_server;
		bzero(&sock_server, sizeof(sock_server));
		sock_server.sin_family = AF_INET;
		sock_server.sin_port = htons(port);
		inet_pton(AF_INET, ip_addr, &sock_server.sin_addr);
		int ret = bind(fd_server, (struct sockaddr *)&sock_server, sizeof(sock_server));
		if (ret < 0) {
			printf("bind error");
			return -1;
		}
		ret = listen(fd_server, 50);
		if (ret < 0) {
			printf("listen error");
			return -1;
		}
	}
	return fd_server;
}

int main(int argc, char *argv[]){
	printf("bbs server\n");
	if (argc <= 2) {
		printf("usage: listen_ip port ");
		return -1;
	}
	client_data clients[FD_LIMIT + 1];
	memset(clients, 0, sizeof(clients));
	int fd_server = start_listen(argv[1], atoi(argv[2]));
	int client_amount = 0;
	if (fd_server > 0) {
		struct pollfd fds[6];
		memset(fds, 0, sizeof(fds));
		fds[0].fd = fd_server;
		fds[0].events = POLLIN;
		do {
			int ret = poll(fds, 6, NULL);
			for(int i = 0; i <  6; i ++) {
				if (fds[i].revents == 0) {
					continue;
				}
				if (fds[i].fd == fd_server && (fds[i].revents & POLLIN)) {
					printf("ready to accept client\n");
					struct sockaddr_in sock_client;
					bzero(&sock_client, sizeof(sock_client));
					socklen_t sock_len = sizeof(sock_client);
					int client_fd = accept(fd_server, (struct sockaddr *)&sock_client, &sock_len);
					printf("accepted: %d\n", client_fd);
					if (client_amount >= 6) {//
						printf("connects full, disconnect\n");
						close(client_fd);
						continue;
					}
					if (client_fd >= 0) {
						clients[client_fd].sock =  sock_client;
						printf("client %s %d connected\n", inet_ntoa(clients[client_fd].sock.sin_addr), ntohs(clients[client_fd].sock.sin_port));
						fds[client_amount + 1].fd = client_fd;
						fds[client_amount + 1].events = POLLIN |POLLHUP | POLLERR;
						fds[client_amount + 1].revents = 0;
						clients[client_fd].fd = client_fd;
						client_amount ++;
					}
					break;
				} else  {      
					if(fds[i].revents & POLLIN)  {
						char sz_buffer[1024] = {0};
						int recv_len  = recv(fds[i].fd, sz_buffer, 1024, 0);
						if (recv_len > 0 ) {
							printf("revc data: %s\n",  sz_buffer);
						} else if (recv_len == 0) { //client disconnect
							
							printf("client recv %s %d disconnected\n", inet_ntoa(clients[fds[i].fd].sock.sin_addr), ntohs(clients[fds[i].fd].sock.sin_port));
							clients[fds[i].fd].fd = -1;
							close(fds[i].fd);
							fds[i] = fds[client_amount];
							fds[client_amount].fd = -1;
							client_amount --;
						} else {
							printf("client error\n");
						}
					} 
					else if(fds[i].revents & POLLOUT)  {
						printf("client writeable\n");
					} 
					else if(fds[i].revents & POLLHUP)  {
						printf("client %s %d disconnected\n", inet_ntoa(clients[fds[i].fd].sock.sin_addr), ntohs(clients[fds[i].fd].sock.sin_port));
						clients[fds[i].fd].fd = -1;
						close(fds[i].fd);
						fds[i] = fds[client_amount];
						fds[client_amount].fd = -1;
						client_amount --;
					}
				}
			}
		} while(true);
	}
	return 1;
}