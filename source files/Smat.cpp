/*
 * Smat.cpp
 *
 *  Created on: Apr 26, 2020
 *      Author: OS1
 */

#include "Smat.h"
#include <stdio.h>

typedef unsigned SignalId;


SignalMatList::SignalMatList() {
	lock
	for(int i = 0; i < 16; signals[i++] = 0);
	unlock
}


void SignalMatList::registerHandler(SignalId ID, SignalHandler handler) { // dodavanje na kraj
lock
	if(signals[ID] == 0) {
		signals[ID] = new Elem(handler);
		unlock
		return;
	}

	Elem* curr = signals[ID];
	while(curr->next != 0)
		curr = curr->next;
	curr->next = new Elem(handler);
	unlock
}

SignalMatList::~SignalMatList() {
	lock
	for(int i = 0; i < 16; i++){
		while(signals[i] != 0) {
			Elem* old = signals[i];
			signals[i] = signals[i]->next;
			old->next = 0;
			delete old;
		}
	}
	unlock
}

int SignalMatList::call(SignalId ID, int block) { // pozivanje sa pocetka



	if(PCB::blockedSignalsGlobally[ID] == 1)
		return 0;
	if(block == 1)
		return 0;


	Elem* curr = signals[ID];
	while(curr != 0) {
		curr->signalHandler();
		curr = curr->next;
	}

	return 1;
}

void SignalMatList::unregisterAllHandlers(SignalId ID) {
lock
	while(signals[ID] != 0) {
			Elem* old = signals[ID];
			signals[ID] = old->next;
			old->next = 0;
			delete old;
		}
unlock
}

void SignalMatList::swap(SignalId ID, SignalHandler hand1, SignalHandler hand2) {
lock
	Elem* found1 = signals[ID];
	Elem* found2 = signals[ID];
	int found = 0;

	while(found1 != 0) {
		if(found1->signalHandler == hand1) {
			++found;
			break;
		}
		else
			found1 = found1->next;
	}

	while(found2 != 0) {
			if(found2->signalHandler == hand2) {
				++found;
				break;
			}
			else
				found2 = found2->next;
		}

	if(found != 2) {
		unlock
		return;
	}

	SignalHandler temp = found1->signalHandler;
	found1->signalHandler = found2->signalHandler;
	found2->signalHandler = temp;
	temp = 0;
	unlock
}

void SignalMatList::blockSignalGlobally(SignalId ID) {
	PCB::blockedSignalsGlobally[ID] = 1;
}

void SignalMatList::unblockSignalGlobally(SignalId ID) {
	PCB::blockedSignalsGlobally[ID] = 0;
}

SignalMatList::SignalMatList(const SignalMatList& original) {
	lock

	for(int i = 0; i < 16; signals[i++] = 0);

	for(int j = 0; j < 16; j++) {
		Elem* curr = original.signals[j];
		while(curr != 0) {
			registerHandler(j, curr->signalHandler);
			curr = curr->next;
		}
	}

unlock
}






