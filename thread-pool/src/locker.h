#ifndef __THREAD_POOL_LOCKER_HEADER__
#define __THREAD_POOL_LOCKER_HEADER__
#include <pthread.h>
class locker
{
public:
	locker(){};
	~locker(){};
public:
		void lock() {
			printf("pthread lock \n");
			pthread_mutex_lock(&m_service_map_mutex);
		}
		void unlock(){
			printf("pthread unlock \n");
			pthread_mutex_unlock(&m_service_map_mutex);
		}
private:

	pthread_mutex_t m_service_map_mutex;
};
#endif