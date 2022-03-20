/*
 * KernelEv.h
 *
 *  Created on: Apr 19, 2020
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "Event.h"

class PCB;

class KernelEv {

public:
	KernelEv(IVTNo No);
	~KernelEv();


	void wait();
	void signal();

private:

	IVTNo myNo;
	PCB* owner;
	int available;
};



#endif /* KERNELEV_H_ */
