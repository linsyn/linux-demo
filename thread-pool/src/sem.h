#ifndef __THREAD_POOL_SEM_HEADER__
#define __THREAD_POOL_SEM_HEADER__
#include <semaphore.h>
#include <errno.h>
class sem {
public:
	sem(){
		#ifdef __APPLE__
		semph =  sem_open("test",O_CREAT, 0644,0);
		#elif __linux__

		#endif
		printf("%s\n", strerror(errno) );
	};
	~sem(){
		sem_close(semph);
	}
public:
	void post(){
		 sem_post(semph);  
	};
	void wait(){
		printf("wait\n");
		sem_wait(semph);  
	};
private:
	sem_t *semph;
};
#endif