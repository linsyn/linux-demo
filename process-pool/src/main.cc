#include <stdio.h>
#include "process_pool.h"
#include "utils.h"
#include <iostream>
#include <sys/socket.h>
#include "cgi_conn.h"
#include <arpa/inet.h>
using namespace std;
int s_pipe[2];

int main(int argc, char *argv[]) {
	printf("procees pool demo\n");
	socketpair(AF_UNIX,SOCK_STREAM, 0,s_pipe);
	printf("s_pipe %d %d\n", s_pipe[0], s_pipe[1]);
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	set_non_blocking(fd);
	struct sockaddr_in sock_addr;
	bzero(&sock_addr, sizeof(sockaddr));
	sock_addr.sin_port = htons(8099);
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret = bind(fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr));
	listen(fd, 64);
	process_pool<cgi_conn> *pool = process_pool<cgi_conn>::get_instance(fd);
	pool->run();
	return 1;
}
