/*
 * Semaphor.cpp
 *
 *  Created on: Apr 10, 2020
 *      Author: OS1
 */

#include "Semaphor.h"
#include "PCB.h"
#include "KernelS.h"



Semaphore::Semaphore(int init) {
	lock

	myImpl = new KernelSem(init);

	unlock
}

Semaphore::~Semaphore() {
	lock

	delete myImpl;

	unlock
}

int Semaphore::wait(Time maxTimeToWait) {
	lock

	int ret = myImpl->wait(maxTimeToWait);

	unlock

	return ret;
}

int Semaphore::signal(int n) {
	lock

	int ret = myImpl->signal(n);

	unlock

	return ret;
}

int Semaphore::val() const {
	return myImpl->val();
}
