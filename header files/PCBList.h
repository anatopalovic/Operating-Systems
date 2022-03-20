/*
 * PCBList.h
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#ifndef PCBLIST_H_
#define PCBLIST_H_

class PCB;

typedef int ID;

class PCBList {
private:
	struct Elem {
		PCB* pcb;
		Elem* next;
		Elem(PCB* p, Elem* n = 0) {
			pcb = p;
			next = n;
		}
	};

	Elem* first;
	int num;

public:

	PCBList();
	~PCBList();

	void addPCB(PCB* p);

	PCB* getPCB(ID id);

	void removePCB(ID id);

};


#endif /* PCBLIST_H_ */
