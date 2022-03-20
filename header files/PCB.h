/*
 * PCB.h
 *
 *  Created on: Apr 7, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_


#define lock {\
	asm pushf;\
	asm cli;\
}

#define unlock asm popf

#define forceSetPSWI  {\
	asm pushf;\
	asm sti;\
}

#define restorePSWI asm popf



typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time;  // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

typedef void (*SignalHandler)();   //SIGNALI
typedef unsigned SignalId;         //SIGNALI

class Thread;
class PCBList;
class PCBQueue;
class SignalMatList; // SIGNALI
class SignalQueue; // SIGNALI



class PCB {

public:
	PCB(StackSize stackSize = defaultStackSize, Time timeSlice1 = defaultTimeSlice, Thread* my = 0);
	~PCB();

	static PCB* running;
	static PCBList PCBlist;
	static ID ThreadID;
	static void wrapper();
	static int blockedSignalsGlobally[16]; // SIGNALI
	enum State{CREATED, STARTED, NOTFINISHED, FINISHED, BLOCKED, KILLED};
	State getState();
	void setState(State newState);
	ID getMyThreadId();


private:
	ID myThreadId;
	Thread* myThread;
	PCBQueue* waitFinish;
	Time myThreadTimeSlice;
	State state;
	unsigned int sp;
	unsigned int ss;
	unsigned int bp;
	unsigned int* stack;
	int infiniteTime;
	int isSignaled;

	void createStack(StackSize stackSize);

	//SIGNALI

	int blockedSignals[16];
	void signal(SignalId ID);
	void callIfSignaled();
	void blockSignal(SignalId ID);
	void unblockSignal(SignalId ID);
	int callSignal(SignalId ID);
	void handle0();
	Thread* myParent;
	SignalQueue* signalQueue;
	SignalMatList* registeredSignals;



	friend class Thread;
	friend class Nit;
	friend class PCBList;
	friend class PCBQueue;
	friend class KernelSem;
	friend class KernelEv;
	friend class System;
	friend class SignalQueue;
	friend void interrupt timer(...);
	friend void restore();
	friend void signal0Handler();



};



#endif /* PCB_H_ */
