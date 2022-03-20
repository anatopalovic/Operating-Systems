/*
 * IVTEntry.h
 *
 *  Created on: Apr 19, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "Event.h"
#include <stdio.h>

extern void dispatch();



typedef void interrupt (*pInterrupt)(...);


class IVTEntry {
public:

	static IVTEntry* IVTentries[256];

	friend class KernelEv;

	IVTEntry(pInterrupt newRoutine, IVTNo No, int callOldRoutine);

	~IVTEntry();


	void callOldRoutine();

	static void callOldRoutineS(IVTNo No);



	IVTNo myNo;

	void signal();
	pInterrupt oldRoutine;
	KernelEv* myKernelEv;
	int callOld;

};

#define PREPAREENTRY(No, flag)\
	void interrupt intr##No(...);\
	IVTEntry ivtEntry##No(intr##No,No,flag);\
	void interrupt intr##No(...){\
	IVTEntry::IVTentries[No]->signal();\
		if (flag == 1) IVTEntry::callOldRoutineS(No);\
	dispatch();\
}



#endif /* IVTENTRY_H_ */
