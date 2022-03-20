/*
 * IdleT.h
 *
 *  Created on: Apr 19, 2020
 *      Author: OS1
 */

#ifndef IDLET_H_
#define IDLET_H_
#include "Thread.h"

class IdleThread : public Thread {
public:

	IdleThread();

	void run();


	PCB* idlePCB;

};



#endif /* IDLET_H_ */
