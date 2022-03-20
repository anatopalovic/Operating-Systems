/*
 * KSList.cpp
 *
 *  Created on: May 5, 2020
 *      Author: OS1
 */

#include "PCB.h"
#include "PCBQueue.h"
#include "KSList.h"
#include "KernelS.h"
#include <stdio.h>

extern volatile int allowContextSwitch;

KernelSemList::KernelSemList() {
	lock

	first = 0;
	num = 0;

	unlock
}

void KernelSemList::addKernelSem(KernelSem* k) {
	lock
		first = new Elem(k, first);
		++num;
	unlock
}


void KernelSemList::checkTime() {
	lock

	for(Elem* curr = first; curr != 0; curr = curr->next) {
		curr->ker->blockedPCBs->checkTime();
	}
	unlock
}


KernelSemList::~KernelSemList() {
	lock

	while(first != 0) {
		Elem* old = first;
		first = first->next;
		delete old;
	}
	num = 0;

	unlock
}


