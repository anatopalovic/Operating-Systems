/*
 * SignalQ.h
 *
 *  Created on: Apr 29, 2020
 *      Author: OS1
 */

#ifndef SIGNALQ_H_
#define SIGNALQ_H_

#include "PCB.h"

class SignalQueue {
private:
	struct Elem {
		SignalId id;
		Elem* next;
		Elem(int d, Elem* n = 0) {
			id = d;
			next = n;
		}
	};

	Elem* first;
	Elem* last;
	int num;

public:

	SignalQueue();
	~SignalQueue();

	void addSignal(SignalId ID);

	void callSignals(int* blocked);


	int isEmpty();

	int numOfElem();

};



#endif /* SIGNALQ_H_ */
