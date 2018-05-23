#ifndef __PROCESS_POOL_HEADER__
#define __PROCESS_POOL_HEADER__
#include "process.h"
#include "utils.h"
#include <unistd.h>
#include <arpa/inet.h>
#define MAX_PROCESS_NUM 3
#define MAX_EVENTS_NUM 64
template<class T>
class process_pool {
	public:
		static process_pool<T> * get_instance( int fd);
		void run();
	private:
		process_pool(int listen_fd, int num);
		void run_parent();
		void run_child();
	private:
		int m_subProcessIndex ;
		bool m_running;
		int m_sockfd;
		int m_processNum;
		int m_currentIndex;
		process *m_processes;
		static process_pool<T> *s_instance;
};

template<class T>
process_pool<T> * process_pool<T>::s_instance = NULL;

template<class T>
process_pool<T> * process_pool<T>::get_instance(int fd){
	if (s_instance == NULL) {
		s_instance = new process_pool(fd, MAX_PROCESS_NUM);
	}
	return s_instance;
}
template<class T>
process_pool<T>:: process_pool(int listen_fd, int num) {
	printf("construct currentPID:%d\n", getpid());
	m_processNum = num;
	m_currentIndex = -1;
	m_running = true;
	m_subProcessIndex = 0;
	m_sockfd = listen_fd;
	if (num > 0) {
		m_processes = new process[num];
		for (int i = 0; i < m_processNum; i++) {
			m_processes[i].init(-1);
			int processID = fork();
			int val = 0;
			m_processes[i].m_processID = processID;
			if (processID > 0) {//perant process
		 		printf("parent close %d \n", m_processes[i].m_pipefd[0]);
				close(m_processes[i].m_pipefd[0]);
				continue;
			} else {//sub process
				 printf("child close %d \n", m_processes[i].m_pipefd[1]);
				close(m_processes[i].m_pipefd[1]);
				m_currentIndex = i;
				break;	
			}
		}
	} else {
		m_processes = NULL;
	}
}

template<class T>
void process_pool<T>::run(){
	if ( m_currentIndex == -1) {
		run_parent();
	} else {
		run_child();
	}
}

template<class T>
void process_pool<T>::run_parent(){
	printf("parent run\n");
	int val = 0;
	int epoll_fd = epoll_create(5);
	add_fd(m_sockfd, epoll_fd);
	epoll_event events[MAX_EVENTS_NUM];
	do {
		int events_num = epoll_wait(epoll_fd, events, MAX_EVENTS_NUM, -1);
		if (events_num < 0)
		{
			printf("parent error:%s\n", strerror(errno));
			break;
		
		}
		for (int i = 0; i < events_num; i ++) {
			if (events[i].data.fd == m_sockfd && (events[i].events & EPOLLIN)) {
				printf("client connected\n");
				int m = m_subProcessIndex;
				if (m_processes == NULL) {
					printf("not sub process \n");
					continue;
					m_running = false;	
				} else {
					while(m_processes[m].m_processID == -1) {
						m ++;
					}
					int new_conn = 1;
					m_subProcessIndex = (m+1) % m_processNum;
					send(m_processes[m].m_pipefd[1], (char *)&new_conn, sizeof(new_conn), 0 );
					printf("send request to %d %d\n", m, m_processes[m].m_pipefd[1]);
				}
			}	
		}
	}while(m_running);
} 


template<class T>
void process_pool<T>::run_child(){
		int val = 0;

	printf("child:%d run accept:%d\n", getpid(), m_processes[m_currentIndex].m_pipefd[0]);
	int epoll_fd = epoll_create(5);
      	 add_fd(m_processes[m_currentIndex].m_pipefd[0], epoll_fd);
	 epoll_event events[MAX_EVENTS_NUM];
	T *connections = new T[65535];
        do {
                int events_num = epoll_wait(epoll_fd, events, MAX_EVENTS_NUM, -1);
         //       printf("child events\n");
		 if (events_num < 0) {
                        printf("child error:%s\n", strerror(errno));
                        break;
                }
                for (int i = 0; i < events_num; i ++) {
			printf("child events:%d fd(%d)\n", events_num, events[i].data.fd);
			if (events[i].data.fd == m_processes[m_currentIndex].m_pipefd[0] && (events[i].events & EPOLLIN)) {
				struct sockaddr_in sock_client;
				bzero(&sock_client, sizeof(sock_client));
				socklen_t len = sizeof(sock_client);
				printf(" %d parent assign connection\n",getpid());
				int client_fd = accept(m_sockfd, (struct sockaddr *)&sock_client, &len);
				if (client_fd >= 0) {
					printf("accept client:%d addr(%s) port(%d)\n", client_fd, inet_ntoa(sock_client.sin_addr), ntohs(sock_client.sin_port));
					add_fd( client_fd, epoll_fd);
					connections[client_fd].init(epoll_fd, client_fd, sock_client);
				}
			} else if (events[i].data.fd == s_pipe[1]){
				printf("signal event\n");		
			} else if (events[i].events & EPOLLIN) {
				printf("data received \n");
				connections[events[i].data.fd].process();
			}
                }
        }while(m_running);
}


#endif
