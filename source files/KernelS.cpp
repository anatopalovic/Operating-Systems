/*
 * _KernelSem.cpp
 *
 *  Created on: Apr 10, 2020
 *      Author: OS1
 */

#include "KernelS.h"
#include "PCBQueue.h"
#include "SCHEDULE.h"
#include "KSList.h"
#include "PCB.h"
#include <stdio.h>

extern void dispatch();

KernelSemList KernelSem::allKernelSems;

KernelSem::KernelSem(int init) {

	value = init;
	allKernelSems.addKernelSem(this);
	blockedPCBs = new PCBQueue();

}

int KernelSem::signal(int n) {


	if(n == 0) {
		if(value++ < 0) {
			PCB* temp = blockedPCBs->unblockPCB();
			temp->isSignaled = 1;
			Scheduler::put(temp);
		}
		return 0;
	}
	else if(n > 0) {

		int i = 0;
		while(i < n && !blockedPCBs->isEmpty()) {
			PCB* curr = blockedPCBs->unblockPCB();
			curr->isSignaled = 1;
			Scheduler::put(curr);
			++i;
		}
		value += n;
		return i;
	}
	else {
		return n;
	}
}

int KernelSem::wait(Time maxTimeToWait) {


	if(--value < 0 ) {

		if(maxTimeToWait != 0) {
			blockedPCBs->blockPCB(PCB::running, 1, maxTimeToWait);
			dispatch();
		}
		else {
			blockedPCBs->blockPCB(PCB::running, 0, 0);
			dispatch();
			++value;
		}
	}

	if(PCB::running->isSignaled == 1){
		PCB::running->isSignaled = 0;
		return 1;
	}
	return 0;
}

int KernelSem::val() {
	return value;
}



KernelSem::~KernelSem() {

	delete blockedPCBs;

}


