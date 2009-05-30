/*
 * Mutex.h
 *
 *  Created on: 23/04/2009
 *      Author: ale
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h>

class Mutex {
public:
	Mutex();
	virtual ~Mutex();
	int lock();
	int unlock();
private:
	pthread_mutex_t mutex;
};

#endif /* MUTEX_H_ */
