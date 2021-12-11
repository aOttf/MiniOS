/*
 * pcb.h
 *
 *  Created on: Feb. 14, 2021
 *      Author: Wang Jingyu
 */
#include <stdio.h>
#ifndef PCB_H_
#define PCB_H_
struct PCB {
	int PC; //PC = pagetable[PC_page] * 4
	int pageTable[10];//indexes are page no.; values are corresponding frame no.
	int PC_page;//page no. currently at
	int PC_offset;
	int pages_max;
	FILE* f;//pointing to the file in the backing store
};

struct PCB* makePCB(int pages_max, FILE* f);
void updatePC(struct PCB*);
void freePCB(struct PCB*);
#endif /* PCB_H_ */
