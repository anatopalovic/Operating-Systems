/*
 * PCB.cpp
 *
 *  Created on: Apr 7, 2020
 *      Author: OS1
 */

#include "PCB.h"
#include "Thread.h"
#include "PCBList.h"
#include "PCBQueue.h"
#include "Semaphor.h"
#include "Smat.h"
#include "KernelS.h"
#include "SignalQ.h"

#include <dos.h>
#include <stdio.h>

extern void dispatch();

PCB* PCB::running = 0;
ID PCB::ThreadID = -2;
PCBList PCB::PCBlist;

// SIGNALI

int PCB::blockedSignalsGlobally[16] = {0};

extern volatile int allowContextSwitch;

extern void signal0Handler();



void PCB::wrapper() {


	running->state = NOTFINISHED;

	running->myThread->run();

	running->state = FINISHED;

	lock

	while(!running->waitFinish->isEmpty()) {
		Scheduler::put(running->waitFinish->unblockPCB());
	}

	allowContextSwitch = 0;
	forceSetPSWI
	PCB::running->callSignal(2);
	restorePSWI
	allowContextSwitch = 1;
	if(PCB::running->myParent != 0) {
		PCB::running->myParent->myPCB->signalQueue->addSignal(1);
	}
	unlock

	dispatch();
}

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* my) {



	//allowContextSwitch = 0;


	state = CREATED;
	myThread = my;
	isSignaled = 0;
	myThreadId = ++PCB::ThreadID;
	myThreadTimeSlice = timeSlice;
	if(timeSlice == 0)
		infiniteTime = 1;
	else
		infiniteTime = 0;

	if(stackSize > 32768)
		stackSize = 32768;


	createStack(stackSize);


	PCBlist.addPCB(this);
	waitFinish = new PCBQueue();


	// SIGNALI

	if(myThreadId != 0 && myThreadId != -1) {
		myParent = PCB::running->myThread;
		for(int j = 0; j < 16; j++)
			blockedSignals[j] = myParent->myPCB->blockedSignals[j];
		registeredSignals = new SignalMatList(*(myParent->myPCB->registeredSignals));
	}
	else {
		myParent = 0;
		for(int i = 0; i < 16; i++)
			blockedSignals[i] = 0;
		registeredSignals = new SignalMatList();
		if(myThreadId == 0)
			registeredSignals->registerHandler(0, signal0Handler);
	}

	signalQueue = new SignalQueue();

	//allowContextSwitch = 1;
}

void PCB::createStack(StackSize stackSize) {

	stackSize /= sizeof(unsigned int);

	unsigned int* st = new unsigned int[stackSize];
	#ifndef BCC_BLOCK_IGNORE

		st[stackSize - 1] = 0x200;
		st[stackSize - 2] = FP_SEG(PCB::wrapper);
		st[stackSize - 3] = FP_OFF(PCB::wrapper);
		ss = FP_SEG(st + stackSize - 12);
		sp = FP_OFF(st + stackSize - 12);
		bp = sp;
	#endif

		stack = st;
}

PCB::State PCB::getState() {
	return state;
}

PCB::~PCB() {

//	allowContextSwitch = 0;

	PCBlist.removePCB(myThreadId);

	delete stack;
	delete waitFinish;
	delete signalQueue;
	delete registeredSignals;


	//allowContextSwitch = 1;

}

ID PCB::getMyThreadId() {
	return myThreadId;
}


void PCB::setState(State newState) {
	state = newState;
}


//SIGNALI


void PCB::signal(SignalId ID) {
	if(ID == 2 || ID == 1)
		return;
	signalQueue->addSignal(ID);
}

void PCB::blockSignal(SignalId ID) {
	blockedSignals[ID] = 1;
}

void PCB::unblockSignal(SignalId ID) {
	blockedSignals[ID] = 0;
}

int PCB::callSignal(SignalId ID) {
	return registeredSignals->call(ID, blockedSignals[ID]);
}

void PCB::callIfSignaled() {

	allowContextSwitch = 0;
	forceSetPSWI

	signalQueue->callSignals(blockedSignals);

	restorePSWI
	allowContextSwitch = 1;
}

void PCB::handle0() {


	if(myThreadId == 0)
		return;

	while(!running->waitFinish->isEmpty())
		Scheduler::put(running->waitFinish->unblockPCB());

	PCB::running->callSignal(2);

	if(myParent != 0) {
		PCB::running->myParent->myPCB->signalQueue->addSignal(1);
	}

	delete waitFinish;
	waitFinish = 0;
	delete stack;
	stack = 0;
	delete signalQueue;
	signalQueue = 0;
	state = KILLED;
}





