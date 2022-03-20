/*
 * KSList.h
 *
 *  Created on: May 5, 2020
 *      Author: OS1
 */

#ifndef KSLIST_H_
#define KSLIST_H_


class KernelSemList {

private:
	struct Elem {
		KernelSem* ker;
		Elem* next;
		Elem(KernelSem* k, Elem* n = 0) {
			ker = k;
			next = n;
		}
	};

	Elem* first;
	int num;

public:

	KernelSemList();
	~KernelSemList();

	void addKernelSem(KernelSem* k);

	void checkTime();

	void unblockThatThread(ID id);

};





#endif /* KSLIST_H_ */
