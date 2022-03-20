/*
 * IVTEntry.cpp
 *
 *  Created on: Apr 19, 2020
 *      Author: OS1
 */

#include "IVTEntry.h"
#include "KernelEv.h"
#include "PCB.h"
#include <dos.h>

IVTEntry* IVTEntry::IVTentries[256] = {0};


IVTEntry::IVTEntry(pInterrupt newRoutine, IVTNo No, int callOldRoutine) {

	lock

	myNo = No;
	callOld = callOldRoutine;
	IVTentries[myNo] = this;


#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(No);
	setvect(No, newRoutine);
#endif

	unlock

}

IVTEntry::~IVTEntry() {
	lock

#ifndef BCC_BLOCK_IGNORE
	setvect(myNo, oldRoutine);
#endif
	myKernelEv = 0;

	unlock
}

void IVTEntry::callOldRoutine() {
	lock
	oldRoutine();
	unlock
}


void IVTEntry::signal() {
	lock
	if(myKernelEv != 0)
		myKernelEv->signal();
	unlock
}

void IVTEntry::callOldRoutineS(IVTNo No) {
	lock
	IVTentries[No] -> callOldRoutine();
	unlock
}
