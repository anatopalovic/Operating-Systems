/*
 * Event.cpp
 *
 *  Created on: Apr 19, 2020
 *      Author: OS1
 */

#include "Event.h"
#include "KernelEv.h"
#include "PCB.h"



Event::Event(IVTNo ivtNo) {
	lock

	myImpl = new KernelEv(ivtNo);

	unlock
}

void Event::wait() {
	lock

	myImpl->wait();

	unlock
}

void Event::signal() {
	lock

	myImpl->signal();

	unlock
}

Event::~Event() {
	lock

	delete myImpl;

	unlock
}


