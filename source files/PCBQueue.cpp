/*
 * PCBQueue.cpp
 *
 *  Created on: Apr 10, 2020
 *      Author: OS1
 */

#include "PCBQueue.h"
#include "KernelS.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include <stdio.h>

PCBQueue::PCBQueue() {
	lock

	first = 0;
	last = 0;
	num = 0;

	unlock
}

void PCBQueue::blockPCB(PCB* p, int t, Time timeS) {
	lock

		last = (!first ? first : last->next) = new Elem(p, t, timeS);
		p->state = PCB::BLOCKED;
		++num;

	unlock
}

PCB* PCBQueue::unblockPCB() {
	lock

	if(first == last) {
		Elem* old = first;
		first = last = 0;
		PCB* ret = old -> pcb;
		delete old;
		--num;
		ret -> state = PCB::NOTFINISHED;
		unlock
		return ret;
	}

	Elem* curr = first;
	while(curr->next != last)
		curr = curr -> next;
	Elem* old = last;
	curr -> next = 0;
	last = curr;
	PCB* ret = old->pcb;
	delete old;
	--num;
	unlock
	return ret;

}

int PCBQueue::isEmpty() {
	if(num == 0)
		return 1;
	else
		return 0;
}

int PCBQueue::numOfElem() {
	return num;
}

PCBQueue::~PCBQueue() {
	lock

	while(first != 0) {
		Elem* old = first;
		first = first->next;
		delete old;
	}
	num = 0;

	unlock
}


void PCBQueue::checkTime() {

	Elem* prev = 0;
	Elem* curr = first;

	while(curr != 0) {

		if(curr->time == 0) {
			prev = curr;
			curr = curr->next;
		}
		else if(--(curr->timeSlice) > 0) {
			prev = curr;
			curr = curr->next;
		}
		else {

			Elem* old = curr;
			curr = curr->next;

			if(prev == 0)
				first = first->next;
			else
				prev->next = curr;

			if(last == old)
				last = prev;

			old->pcb->setState(PCB::NOTFINISHED);
			Scheduler::put(old->pcb);
			--num;
			delete old;
		}
	}
}






