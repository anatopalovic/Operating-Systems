/*
 * SignalQ.cpp
 *
 *  Created on: Apr 29, 2020
 *      Author: OS1
 */


#include "SignalQ.h"
#include "Smat.h"
#include "PCB.h"
#include <stdio.h>


SignalQueue::SignalQueue() {
	lock

	first = 0;
	last = 0;
	num = 0;

	unlock
}

void SignalQueue::addSignal(SignalId ID) {
	lock

		last = (!first ? first : last->next) = new Elem(ID);
		++num;

	unlock
}

void SignalQueue::callSignals(int* blocked) {

		Elem* curr = first;
		Elem* prev = 0;

		while(curr != 0) {

			int called = PCB::running->registeredSignals->call(curr->id, blocked[curr->id]);
			if(PCB::running->getState() == PCB::KILLED)
				return;
			if(called == 1) {

				Elem* old = curr;
				curr = curr->next;
				old->next = 0;
				if(prev == 0)
					first = curr;
				else
					prev->next = curr;
				if(last == old)
					last = prev;
				delete old;
				--num;
			}
			else {
				prev = curr;
				curr = curr->next;
			}
		}
}

int SignalQueue::isEmpty() {
	if(num == 0)
		return 1;
	return 0;
}

int SignalQueue::numOfElem() {
	return num;
}

SignalQueue::~SignalQueue() {

lock

	while(first != 0) {
		Elem* old = first;
		first = first->next;
		delete old;
	}
	num = 0;

unlock

}



