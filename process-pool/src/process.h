#ifndef __NGINX_PROCESS_HEADER__
#define __NGINX_PROCESS_HEADER__
#include <sys/socket.h>
class process {
	public:
		process(){
			m_processID = 0;
		}
		void init(int processID) {
			m_processID = processID;
			socketpair(AF_UNIX, SOCK_STREAM, 0, m_pipefd);
	
			printf("socket pair: %d %d\n", m_pipefd[0], m_pipefd[1]);
		}
		int get_pipe(int index){
			if (index >= 0 && index <=1) {
				return m_pipefd[index];
			}
			return -1;
		}
		int set_processID(int processID){
			m_processID = processID;
		}	
	public:
		int m_processID;
		int m_pipefd[2];
};

#endif
