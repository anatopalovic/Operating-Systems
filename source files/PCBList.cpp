/*
 * PCBList.cpp
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#include "PCBList.h"
#include "PCB.h";
#include <stdio.h>

PCBList::PCBList() {
	lock
	first = 0;
	num = 0;
	unlock
}

void PCBList::addPCB(PCB* p) {
	lock
		first = new Elem(p, first);
		++num;
	unlock
}

PCB* PCBList::getPCB(ID id) {
	lock
		for(PCBList::Elem* curr = first; curr != 0; curr = curr->next)
			if(curr->pcb->myThreadId == id) {
				unlock
				return curr->pcb;
			}
	unlock
		return 0;
}

void PCBList::removePCB(ID id) {
	lock
		PCBList::Elem* help = 0;
		PCBList::Elem* curr = first;
		while(curr->pcb->myThreadId != id && curr != 0) {
			help = curr;
			curr = curr->next;
		}
		if(curr == first)
			first = curr->next;
		else
			help->next = curr->next;
		curr->next = 0;
		delete curr;
		--num;
		unlock
}

PCBList::~PCBList() {
	lock

	while(first != 0) {
		Elem* old = first;
		first = first->next;
		delete old;
	}
	num = 0;
	unlock
}
