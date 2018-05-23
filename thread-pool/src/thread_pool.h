#ifndef __THREAD_POOL_HEADER__
#define __THREAD_POOL_HEADER__

#include <pthread.h>
#include <vector>
#include <list>
#include "locker.h"
#include "sem.h"
template<class T>
class thread_pool
{
public:
	thread_pool(int thread_num, int max_req_num);
	virtual ~thread_pool();
public:
	bool append(T *req);
private:
	static void * worker(void *arg);
	void run();
private:
	int m_thread_num;
	int m_max_req;
	pthread_t * m_threads;
	std::list<T *> m_tasks;
	locker m_task_locker;
	sem m_task_stat;
	bool m_runing;
};
template<class T>
void  * thread_pool<T>::worker(void *arg) {
	printf("thread_pool worker\n");
	if (arg)
	{
		thread_pool<T> * ptr = (thread_pool<T> * )arg;
		ptr->run();
		/* code */
	}
	return NULL;
	
}
template<class T>
thread_pool<T>::thread_pool(int thread_num, int max_req_num) {
	printf("thread_pool construct\n");
	m_thread_num  = thread_num;
	m_max_req  = max_req_num;
	m_runing = true;
	m_threads = new pthread_t[thread_num];
	
	if (m_threads) {
		for (int i = 0; i < m_thread_num; ++i) {
			
			if (pthread_create(m_threads + i, NULL, worker, this) != 0) {
				delete [] m_threads;
				break;
			} 
			printf("%p %d  %d\n", m_threads + i, m_threads[i] , *( m_threads + i));
			if (pthread_detach(m_threads[i])) {
				delete [] m_threads;
				break;
			}
		}
	}

}

template<class T>
thread_pool<T>::~thread_pool() {
	printf("thread_pool deconstruct\n");
	delete [] m_threads;
	m_runing = false;
}

template<class T>
void thread_pool<T>::run() {
	printf("thread_pool run\n");
	int i = 0;
	while(m_runing) {

		m_task_stat.wait();
		m_task_locker.lock();
		printf("thread process\n");
		T *req = NULL;
		if (!m_tasks.empty()) {
			req = m_tasks.front();
			m_tasks.pop_front();
		}
		m_task_locker.unlock();
		if (req) {
			//req->process();
		}

		m_runing = i < 10;
		i ++;
	}
	
}



template<class T>
bool thread_pool<T>::append(T *req) {
	printf("thread_pool append\n");
	if (req == NULL) {
		return false;
	}
	bool b_ret = false;
	m_task_locker.lock();
	if (m_tasks.size() < m_max_req) {
		/* code */
		m_tasks.push_back(req);
		b_ret = true;
	}
	m_task_locker.unlock();
	m_task_stat.post();
	return b_ret;
	
}
#endif

