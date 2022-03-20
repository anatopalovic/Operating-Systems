/*
 * Thread.cpp


 *
 *  Created on: Apr 7, 2020
 *      Author: OS1
 */

#include "Thread.h"
#include "SCHEDULE.h"
#include "PCBList.h"
#include "PCBQueue.h"
#include "Smat.h"
#include <stdio.h>

extern void dispatch();


Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock

	myPCB = new PCB(stackSize, timeSlice, this);

	unlock
}

void Thread::run() {}


Thread::~Thread() {
	lock

	waitToComplete();
	delete myPCB;

	unlock
}

ID Thread::getId() {
	lock
	ID id = myPCB->getMyThreadId();
	unlock
	return id;
}

ID Thread::getRunningId() {
	lock
	ID id = PCB::running->getMyThreadId();
	unlock
	return id;
}

Thread* Thread::getThreadById(ID id) {
	lock
	Thread* t = PCB::PCBlist.getPCB(id)->myThread;
	unlock
	return t;

}


void Thread::start() {

	lock

	myPCB->state = PCB::STARTED;
	Scheduler::put(myPCB);

	unlock
}

void Thread::waitToComplete() {
	if(PCB::running->myThread == this)
		return;
	if(myPCB->state != PCB::FINISHED && myPCB->state != PCB::KILLED) {
		myPCB->waitFinish->blockPCB(PCB::running, 0, 0);
		dispatch();
	}
}

//SIGNALI

void Thread::signal(SignalId ID) {
	lock
	myPCB->signal(ID);
	unlock
}

void Thread::registerHandler(SignalId ID, SignalHandler handler) {
	lock
	myPCB->registeredSignals->registerHandler(ID, handler);
	unlock
}

void Thread::unregisterAllHandlers(SignalId ID) {
	lock
	myPCB->registeredSignals->unregisterAllHandlers(ID);
	unlock
}

void Thread::swap(SignalId ID, SignalHandler hand1, SignalHandler hand2) {
	lock
	myPCB->registeredSignals->swap(ID, hand1, hand2);
	unlock
}

void Thread::blockSignal(SignalId ID) {
	lock
	myPCB->blockSignal(ID);
	unlock
}

void Thread::unblockSignal(SignalId ID) {
	lock
	myPCB->unblockSignal(ID);
	unlock
}

void Thread::blockSignalGlobally(SignalId ID) {
	lock
	PCB::blockedSignalsGlobally[ID] = 1;
	unlock
}

void Thread::unblockSignalGlobally(SignalId ID) {
	lock
	PCB::blockedSignalsGlobally[ID] = 0;
	unlock
}






