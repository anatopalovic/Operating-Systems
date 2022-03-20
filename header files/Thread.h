/*
 * Thread
 *
 *  Created on: Apr 5, 2020
 *      Author: OS1
 */

#ifndef THREAD_
#define THREAD_

#include <dos.h>
#include "PCB.h"
#include "SCHEDULE.h"


class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);


protected:
	friend class PCB;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run();
private:
	PCB* myPCB;


	//SIGNALI

public:
	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);
};







#endif /* THREAD_ */
