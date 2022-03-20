/*
 * MainT.h
 *
 *  Created on: Apr 28, 2020
 *      Author: OS1
 */

#ifndef MAINT_H_
#define MAINT_H_

#include "Thread.h"

class MainThread : public Thread {

public:
	MainThread();

	PCB* mainPCB;
};



#endif /* MAINT_H_ */
