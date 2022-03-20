/*
 * main.cpp
 *
 *  Created on: Apr 23, 2020
 *      Author: OS1
 */
#include <stdio.h>
#include "System.h"
#include "Smat.h"
#include "PCB.h"
#include "KernelS.h"
#include "Semaphor.h"
#include "Nit.h"

extern int userMain(int argc, char* argv[]);



int main(int argc, char* argv[]) {

	init();

	int ret = userMain(argc, argv);


//	Nit* n = new Nit();
	//n->start();
	//dispatch();
	//dispatch();

	restore();

	return ret;
	//return 0;
}

