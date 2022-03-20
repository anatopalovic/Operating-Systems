/*
 * MainT.cpp
 *
 *  Created on: Apr 28, 2020
 *      Author: OS1
 */


#include "MainT.h"
#include "PCBList.h"

MainThread::MainThread() : Thread() {
	mainPCB = PCB::PCBlist.getPCB(getId());
}



