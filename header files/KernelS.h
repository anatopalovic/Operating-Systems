/*
 * _KernelSem.h
 *
 *  Created on: Apr 10, 2020
 *      Author: OS1
 */

#ifndef KERNELS_H_
#define KERNELS_H_

typedef unsigned int Time;

class PCBQueue;
class KernelSemList;

class KernelSem {
public:
	KernelSem(int init);
	~KernelSem();

	int wait (Time maxTimeToWait);
	int signal(int n=0);
	int val ();

	static KernelSemList allKernelSems;
	static void checkPCBs();


private:
	int value;
	PCBQueue* blockedPCBs;

	friend class KernelSemList;
};


#endif /* KERNELS_H_ */
