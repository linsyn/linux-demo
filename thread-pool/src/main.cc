#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#include "utils.h"
#include "poll_events.h"
#include "thread_pool.h"
#include "epoll_event.h"


int on_data_connect(void *pdata);
int on_client_connect(void *pdata){
	int fd = *(int *)pdata;
	printf("client accept process: %d\n", fd);

	struct sockaddr_in client_sock;
	bzero(&client_sock, sizeof(client_sock));
	socklen_t sock_len = sizeof(client_sock);
	int client_fd = accept(fd, (struct sockaddr *)&client_sock, &sock_len);
	if (client_fd > 0)
	{
		printf("client addr(%s) port(%d) connected\n", inet_ntoa(client_sock.sin_addr), ntohs(client_sock.sin_port));
	#ifdef __APPLE__	
		add_poll_event(client_fd, on_data_connect);
	#elif __linux__
		add_epoll_event(client_fd, on_data_connect);
	#endif
	}
	return 0;
}

int on_data_connect(void *pdata){
	int fd = *(int *)pdata;
	char buff[1024] = {0};
	int recv_len = recv(fd, buff, 1024, 0);
	printf("data recv: %s\n",  buff);
	return 0;

}
int main(int argc, char *argv[]) {
	printf("thread pool demo\n");
	
	int fd = start_listen(8899);
	if (fd > 0) {
		thread_pool<int> *p_pool = new thread_pool<int>(1,1);
	#ifdef __APPLE__
		init_context();
		add_poll_event(fd, on_client_connect);
		process_poll_event();
	#elif __linux__
		init_epoll_context();
		add_epoll_event(fd, on_client_connect);
		process_epoll_event();
	#endif
	}
	return 1;
}
