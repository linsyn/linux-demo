#ifndef __CGI_CONN_HEADER__
#define __CGI_CONN_HEADER__
#include <stdio.h>
#include <stdlib.h>
class cgi_conn {
	public: 
		cgi_conn(){
			
		}
	public:
		void init(int epoll_fd, int sock_fd, const sockaddr_in &sock_addr) {
			printf("cgi connection initialize: client addr(%s) port(%d)\n", inet_ntoa(sock_addr.sin_addr), ntohs(sock_addr.sin_port));	
			m_epollfd  = epoll_fd;
			m_sockfd = sock_fd;
			memset(m_buf, 0, 1024);
			m_readidx = 0;
		}	
		void process(){
			printf("cgi connection process\n");
			int idx = 1;
			int ret  = -1;
			while(true) {
				ret = recv(m_sockfd, m_buf, 1024, 0);
			
				if (ret < 0) {
					remove_fd(m_sockfd, m_epollfd);
					break;
				} else if (ret == 0) {
					 remove_fd(m_sockfd, m_epollfd);
                                        break;
				} else {
					m_readidx += ret;
					printf("recv data: %s\n", m_buf);
					for(;idx < m_readidx; ++idx) {
						if(idx >= 1 && m_buf[idx - 1] == '\r' && m_buf[idx] == '\n'){
							printf("matched \n");
							break;
						}
					}
					printf("aaaa %d %d\n", idx, m_readidx);
					if (idx == m_readidx) {
						continue;
					}
					m_buf[idx - 1] = 0;

					char *file_name = m_buf;
					if (access(file_name, F_OK) == -1) {
						 remove_fd(m_sockfd, m_epollfd);
						printf("no find files\n");
						break;
					}
					printf("find excutation\n");
					ret = fork();
					if (ret == -1) {
						break;
					}
					else if (ret > 0){
						 remove_fd(m_sockfd, m_epollfd);
                                                break;
					} else {
						close(STDOUT_FILENO);
						dup(m_sockfd);
						execl(m_buf, m_buf, 0);
						exit(0);
					}
				}
			}
		}
		int m_epollfd;
		int m_sockfd;
		struct sockaddr_in m_sockaddr;
		char m_buf[1024];
		int m_readidx;
		
};
#endif
