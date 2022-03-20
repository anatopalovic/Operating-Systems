/*
 * KernelEv.cpp
 *
 *  Created on: Apr 19, 2020
 *      Author: OS1
 */
#include "KernelEv.h"
#include "IVTEntry.h"
#include "SCHEDULE.H"
#include "PCB.h"
#include <stdio.h>


void dispatch();

KernelEv::KernelEv(IVTNo No) {

	myNo = No;
	owner = PCB::running;
	available = 1;

	IVTEntry::IVTentries[myNo]->myKernelEv = this;
}

void KernelEv::wait() {

	if(PCB::running != owner)
		return;


	if(available == 0) {
		owner->state = PCB::BLOCKED;
		dispatch();
	}
	else
		available = 0;
}

void KernelEv::signal() {

	if(available == 0) {
		owner->state = PCB::NOTFINISHED;
		Scheduler::put(owner);
		available = 1;
	}

}

KernelEv::~KernelEv() {

	IVTEntry::IVTentries[myNo]->myKernelEv = 0;
	owner = 0;

}
