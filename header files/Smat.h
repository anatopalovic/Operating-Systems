/*
 * Smat.h
 *
 *  Created on: Apr 25, 2020
 *      Author: OS1
 */

#ifndef SMAT_H_
#define SMAT_H_

#include "Thread.h"


class SignalMatList {


	struct Elem{
		SignalHandler signalHandler;
		Elem* next;
		Elem(SignalHandler s, Elem* n = 0) {
			signalHandler = s;
			next = n;
		}
	};

	Elem* signals[16];

public:

	SignalMatList();
	~SignalMatList();
	SignalMatList(const SignalMatList&);

	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void blockSignalGlobally(SignalId ID);
	void unblockSignalGlobally(SignalId signal);

	int call(SignalId ID, int blocked);

};




#endif /* SMAT_H_ */
