/*
 * PCBQueue.h
 *
 *  Created on: Apr 10, 2020
 *      Author: OS1
 */

#ifndef PCBQUEUE_H_
#define PCBQUEUE_H_

#include "PCB.h"

class PCBQueue {
private:
	struct Elem {
		PCB* pcb;
		int time;
		Time timeSlice;
		Elem* next;
		Elem(PCB* p, int t, Time s, Elem* n = 0) {
			pcb = p;
			time = t;
			timeSlice = s;
			next = n;
		}
	};

	Elem* first;
	Elem* last;
	int num;


public:

	PCBQueue();
	~PCBQueue();

	void blockPCB(PCB* p, int t, Time timeS);

	PCB* unblockPCB();

	int isEmpty();

	int numOfElem();

	void checkTime();

};



#endif /* PCBQUEUE_H_ */
