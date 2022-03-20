/*
 * IdleT.cpp
 *
 *  Created on: Apr 19, 2020
 *      Author: OS1
 */

#include "IdleT.h"
#include "PCBList.h"


IdleThread::IdleThread() : Thread(defaultStackSize, 1) {
	idlePCB = PCB::PCBlist.getPCB(getId());
}


void IdleThread::run() {
	while(1);
}


