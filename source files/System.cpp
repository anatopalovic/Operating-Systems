/*
 * System.cpp
 *
 *  Created on: Apr 15, 2020
 *      Author: OS1
 */

//#include "System.h"
#include <stdio.h>
#include "PCB.h"
#include "SCHEDULE.H"
#include "KernelS.h"
#include "KSList.h"
#include "IdleT.h"
#include "KernelEv.h"
#include "MainT.h"
#include <dos.h>
#include "PCBList.h"
#include <iostream.h>
#include "Smat.h"



void signal0Handler() {
	PCB::running->handle0();
}

volatile int contextOnDemand = 0;
volatile unsigned int timerCounter = 0;
MainThread* mainThread = 0;
IdleThread* idleThread = 0;

volatile unsigned int tsp1;
volatile unsigned int tss1;
volatile unsigned int tbp1;

volatile int callOldTimer = 0;

unsigned oldTimerOFF, oldTimerSEG;

typedef void interrupt (*pInterrupt)(...);

pInterrupt oldTimer;

volatile int allowContextSwitch = 1; // SIGNALI


extern void tick();


void interrupt timer(...) {

	if(contextOnDemand == 0) {
		tick();
		if(PCB::running->infiniteTime == 0)
			--timerCounter;

		KernelSem::allKernelSems.checkTime();

		callOldTimer = 1;
	}

	if(allowContextSwitch == 0) {
		if(contextOnDemand == 0) {
			callOldTimer = 0;
			asm int 60h
		}
		contextOnDemand = 0;
		return;
	}

	if((timerCounter <= 0 && PCB::running->infiniteTime == 0) || contextOnDemand == 1) {


			contextOnDemand = 0;

			asm {
				mov tsp1, sp
				mov tss1, ss
				mov tbp1, bp
			}

			PCB::running->sp = tsp1;
			PCB::running->ss = tss1;
			PCB::running->bp = tbp1;


			if(PCB::running->getState() != PCB::FINISHED && PCB::running->getState() != PCB::BLOCKED && PCB::running!=idleThread->idlePCB) {
				Scheduler::put(PCB::running);
			}

			PCB::running = Scheduler::get();

			if(PCB::running == 0)
				PCB::running = idleThread->idlePCB;
			else
				PCB::running->callIfSignaled();

			while(PCB::running->getState() == PCB::KILLED) {
				PCB::running = Scheduler::get();
				if(PCB::running == 0)
					PCB::running = idleThread->idlePCB;
				else
					PCB::running->callIfSignaled();
			}

			tsp1 = PCB::running->sp;
			tss1 = PCB::running->ss;
			tbp1 = PCB::running->bp;

			timerCounter = PCB::running->myThreadTimeSlice;

			asm {
				mov sp,tsp1
				mov ss,tss1
				mov bp,tbp1
			}
	}

	if(callOldTimer == 1) {
		callOldTimer = 0;
		asm int 60h
	}
}

void dispatch() {


	lock
	contextOnDemand = 1;
		timer();
	unlock
}

void init() {

	idleThread = new IdleThread();
	mainThread = new MainThread();
	timerCounter = defaultTimeSlice;
	PCB::running = mainThread->mainPCB;

lock
	asm{
			push es
			push ax

			mov ax,0
			mov es,ax

			mov ax, word ptr es:0022h
			mov word ptr oldTimerSEG, ax
			mov ax, word ptr es:0020h
			mov word ptr oldTimerOFF, ax

			mov word ptr es:0022h, seg timer
			mov word ptr es:0020h, offset timer

			mov ax, oldTimerSEG
			mov word ptr es:0182h, ax
			mov ax, oldTimerOFF
			mov word ptr es:0180h, ax

			pop ax
			pop es
		}

unlock
}

void restore() {

	allowContextSwitch = 0;
	mainThread->mainPCB->callSignal(2);
	allowContextSwitch = 1;
	delete idleThread->idlePCB;
	delete mainThread->mainPCB;

lock
	asm {
			push es
			push ax

			mov ax,0
			mov es,ax

			mov ax, word ptr oldTimerSEG
			mov word ptr es:0022h, ax
			mov ax, word ptr oldTimerOFF
			mov word ptr es:0020h, ax

			pop ax
			pop es
		}
unlock
}


